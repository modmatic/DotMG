/**
 * @file DotMGCore.cpp
 * \brief
 * The DotMGCore class for hardware initilization and control.
 */

#include "DotMGCore.h"
#include <SPI.h>

static const int frameBufLen = WIDTH*HEIGHT*12/8; // 12 bits/px, 8 bits/byte

static uint8_t MADCTL = ST77XX_MADCTL_MV | ST77XX_MADCTL_MY;
static bool inverted = false;

// Forward declarations

static void bootPins();
static void bootSPI();
static void bootDisplay();

static void displayDataMode();
static void displayCommandMode();
static void sendDisplayCommand(uint8_t command);

static void beginDisplaySPI();
static void endDisplaySPI();
static void setWriteRegion();

// DMA

static volatile bool dmaBusy;
static void initDMA();
static void DMATransfer(uint8_t *data, uint16_t n);


DotMGCore::DotMGCore() { }

void DotMGCore::boot()
{
  bootPins();
  bootSPI();
  bootDisplay();
  bootPowerSaving();
}

void bootPins()
{
  pinMode(PIN_BUTTON_A, INPUT_PULLUP);
  pinMode(PIN_BUTTON_B, INPUT_PULLUP);
  pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
  pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
  pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_START, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SELECT, INPUT_PULLUP);
}

void bootSPI()
{
  SPI.begin();
  initDMA();
}

void bootDisplay()
{
  pinMode(PIN_DISP_SS, OUTPUT);
  pinMode(PIN_DISP_DC, OUTPUT);
  digitalWrite(PIN_DISP_SS, HIGH);

  beginDisplaySPI();

  sendDisplayCommand(ST77XX_SWRESET);  // Software reset
  delay(150);

  sendDisplayCommand(ST77XX_SLPOUT);  // Bring out of sleep mode
  delay(150);

  sendDisplayCommand(ST7735_FRMCTR1);  // Framerate ctrl - normal mode
  SPI.transfer(0x01);               // Rate = fosc/(1x2+40) * (LINE+2C+2D)
  SPI.transfer(0x2C);
  SPI.transfer(0x2D);

  sendDisplayCommand(ST77XX_MADCTL);  // Set initial orientation
  SPI.transfer(MADCTL);

  sendDisplayCommand(ST77XX_COLMOD);  // Set color mode (12-bit)
  SPI.transfer(0x03);

  sendDisplayCommand(ST7735_GMCTRP1);  // Gamma Adjustments (pos. polarity)
  SPI.transfer(0x02);
  SPI.transfer(0x1c);
  SPI.transfer(0x07);
  SPI.transfer(0x12);
  SPI.transfer(0x37);
  SPI.transfer(0x32);
  SPI.transfer(0x29);
  SPI.transfer(0x2D);
  SPI.transfer(0x29);
  SPI.transfer(0x25);
  SPI.transfer(0x2B);
  SPI.transfer(0x39);
  SPI.transfer(0x00);
  SPI.transfer(0x01);
  SPI.transfer(0x03);
  SPI.transfer(0x10);

  sendDisplayCommand(ST7735_GMCTRN1);  // Gamma Adjustments (neg. polarity)
  SPI.transfer(0x03);
  SPI.transfer(0x1D);
  SPI.transfer(0x07);
  SPI.transfer(0x06);
  SPI.transfer(0x2E);
  SPI.transfer(0x2C);
  SPI.transfer(0x29);
  SPI.transfer(0x2D);
  SPI.transfer(0x2E);
  SPI.transfer(0x2E);
  SPI.transfer(0x37);
  SPI.transfer(0x3F);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x02);
  SPI.transfer(0x10);

  // Clear entire display
  setWriteRegion(0, 0, DISP_WIDTH, DISP_HEIGHT);
  for (int i = 0; i < DISP_WIDTH*DISP_HEIGHT/2; i++) {
    SPI.transfer(bgColor >> 4);
    SPI.transfer(((bgColor & 0xF) << 4) | (bgColor >> 8));
    SPI.transfer(bgColor);
  }

  sendDisplayCommand(ST77XX_DISPON); //  Turn screen on
  delay(100);

  endDisplaySPI();

  drawBorder();
  blank();
}

void DotMGCore::paintScreen(const uint8_t *image)
{
  paintScreen((uint8_t *)image, false);
}

void DotMGCore::paintScreen(uint8_t image[], bool clear)
{
  beginDisplaySPI();
  setWriteRegion();
  DMATransfer(image, frameBufLen);

  if (clear)
    memset(image, 0, frameBufLen);
}

void DotMGCore::blank()
{
  beginDisplaySPI();
  setWriteRegion();
  for (int i = 0; i < frameBufLen; i ++)
  {
    SPI.transfer(0);
  }
  endDisplaySPI();
}

void DotMGCore::invert(bool inverse)
{
  if (inverse == inverted)
    return;

  inverted = inverse;

  uint16_t tmp = pixelColor;
  setPixelColor(bgColor);
  setBackgroundColor(tmp);
}

void DotMGCore::flipVertical(bool flipped)
{
  if (flipped)
  {
    MADCTL |= ST77XX_MADCTL_MX;
  }
  else
  {
    MADCTL &= ~ST77XX_MADCTL_MX;
  }
  beginDisplaySPI();
  sendDisplayCommand(ST77XX_MADCTL);
  SPI.transfer(MADCTL);
  endDisplaySPI();
}

void DotMGCore::flipHorizontal(bool flipped)
{
  if (flipped)
  {
    MADCTL &= ~ST77XX_MADCTL_MY;
  }
  else
  {
    MADCTL |= ST77XX_MADCTL_MY;
  }
  beginDisplaySPI();
  sendDisplayCommand(ST77XX_MADCTL);
  SPI.transfer(MADCTL);
  endDisplaySPI();
}

void DotMGCore::allPixelsOn(bool on)
{
  beginDisplaySPI();
  sendDisplayCommand(on ? ST77XX_DISPOFF : ST77XX_DISPON);
  endDisplaySPI();
  delay(100);
}

void DotMGCore::displayOff()
{
  beginDisplaySPI();
  sendDisplayCommand(ST77XX_SLPIN);
  endDisplaySPI();
  delay(150);
}

void DotMGCore::displayOn()
{
  beginDisplaySPI();
  sendDisplayCommand(ST77XX_SLPOUT);
  endDisplaySPI();
  delay(150);
}

uint8_t DotMGCore::buttonsState()
{
  uint32_t btns = ~(*portInputRegister(IO_PORT));
  return (
    (((btns & MASK_BUTTON_A) != 0) << A_BUTTON_BIT) |
    (((btns & MASK_BUTTON_B) != 0) << B_BUTTON_BIT) |
    (((btns & MASK_BUTTON_UP) != 0) << UP_BUTTON_BIT) |
    (((btns & MASK_BUTTON_DOWN) != 0) << DOWN_BUTTON_BIT) |
    (((btns & MASK_BUTTON_LEFT) != 0) << LEFT_BUTTON_BIT) |
    (((btns & MASK_BUTTON_RIGHT) != 0) << RIGHT_BUTTON_BIT) |
    (((btns & MASK_BUTTON_START) != 0) << START_BUTTON_BIT) |
    (((btns & MASK_BUTTON_SELECT) != 0) << SELECT_BUTTON_BIT)
  );
}

void displayDataMode()
{
  *portOutputRegister(IO_PORT) |= MASK_DISP_DC;
}

void displayCommandMode()
{
  *portOutputRegister(IO_PORT) &= ~MASK_DISP_DC;
}

void sendDisplayCommand(uint8_t command)
{
  displayCommandMode();
  SPI.transfer(command);
  displayDataMode();
}

void beginDisplaySPI()
{
  *portOutputRegister(IO_PORT) &= ~MASK_DISP_SS;
  SPI.beginTransaction(SPI_SETTINGS);
}

void endDisplaySPI()
{
  SPI.endTransaction();
  *portOutputRegister(IO_PORT) |= MASK_DISP_SS;
}

static void setWriteRegion()
{
  sendDisplayCommand(ST77XX_CASET);  //  Column addr set
  SPI.transfer(0);
  SPI.transfer(0);                    //  x start
  SPI.transfer(0);
  SPI.transfer(WIDTH-1);              //  x end

  sendDisplayCommand(ST77XX_RASET);  //  Row addr set
  SPI.transfer(0);
  SPI.transfer(0);                    //  y start
  SPI.transfer(0);
  SPI.transfer(HEIGHT-1);             //  y end

  sendDisplayCommand(ST77XX_RAMWR);  //  Initialize write to display RAM
}


//  DMA code
// -------------------------------------------------------

typedef struct
{
  uint16_t btctrl;
  uint16_t btcnt;
  uint32_t srcaddr;
  uint32_t dstaddr;
  uint32_t descaddr;
} dmaDescriptor;

static volatile dmaDescriptor wrb[12] __attribute__ ((aligned(16)));
static dmaDescriptor descriptor_section[12] __attribute__ ((aligned(16)));
static dmaDescriptor descriptor __attribute__ ((aligned(16)));

static void initDMA()
{
  PM->AHBMASK.bit.DMAC_ = 1;
  PM->APBBMASK.bit.DMAC_ = 1;
  NVIC_EnableIRQ(DMAC_IRQn);

  // Assign descriptor/WRB addresses and enable DMA
  DMAC->BASEADDR.reg = (uint32_t)descriptor_section;
  DMAC->WRBADDR.reg = (uint32_t)wrb;
  DMAC->CTRL.reg = (
    DMAC_CTRL_DMAENABLE |
    DMAC_CTRL_LVLEN(0xF)
  );
}

static void DMATransfer(uint8_t *data, uint16_t n)
{
  dmaBusy = true;

  // Disable channel
  DMAC->CHID.reg = DMAC_CHID_ID(DMA_CHAN);
  DMAC->CHCTRLA.bit.ENABLE = 0;

  // Reset and configure
  DMAC->CHCTRLA.bit.SWRST = 1;
  DMAC->SWTRIGCTRL.reg &= ~bit(DMA_CHAN);
  DMAC->CHCTRLB.reg = (
    DMAC_CHCTRLB_LVL(0) |
    DMAC_CHCTRLB_TRIGSRC(DMA_TRIGGER_SRC) |
    DMAC_CHCTRLB_TRIGACT_BEAT
  );
  DMAC->CHINTENSET.bit.TCMPL = 1;  // enable completion interrupt

  // Configure descriptor
  descriptor.descaddr = 0;
  descriptor.dstaddr = (uint32_t)&SPI_SERCOM->SPI.DATA.reg;
  descriptor.btcnt = n;
  descriptor.srcaddr = (uint32_t)data + n;
  descriptor.btctrl = (
    DMAC_BTCTRL_VALID |
    DMAC_BTCTRL_SRCINC
  );
  memcpy(&descriptor_section[DMA_CHAN], &descriptor, sizeof(dmaDescriptor));

  // Enable channel
  DMAC->CHID.reg = DMAC_CHID_ID(DMA_CHAN);
  DMAC->CHCTRLA.bit.ENABLE = 1;

  // Wait to finish
  while (dmaBusy);
}

void DMAC_Handler()
{
  DMAC->CHID.reg = DMAC_CHID_ID(DMA_CHAN);
  if (DMAC->CHINTFLAG.bit.TCMPL)
  {
    // Disable DMA
    DMAC->CHID.reg = DMAC_CHID_ID(DMA_CHAN);
    DMAC->CHCTRLA.bit.ENABLE = 0;

    DotMGCore::endDisplaySPI();
    dmaBusy = false;

    // Clear interrupt flag
    DMAC->CHINTENCLR.bit.TCMPL = 1;
  }
}
