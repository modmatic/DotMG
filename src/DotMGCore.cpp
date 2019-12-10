/**
 * @file DotMGCore.cpp
 * \brief
 * The DotMGCore class for hardware initilization and control.
 */

#include "DotMGCore.h"
#include <SPI.h>

static uint8_t MADCTL = ST77XX_MADCTL_MV | ST77XX_MADCTL_MY;
static bool inverted = false;

static const uint16_t stageLen = WIDTH*HEIGHT*12/8; // 12 bits/px, 8 bits/byte
static uint8_t buf1[stageLen];
static uint8_t buf2[stageLen];
uint8_t *DotMGCore::stage = buf1;
static uint8_t *stage2 = buf2;

static SPIClass dispSPI(
    &PERIPH_SPI_DISP,
    PIN_SPI_DISP_MISO,
    PIN_SPI_DISP_SCK,
    PIN_SPI_DISP_MOSI,
    PAD_SPI_DISP_TX,
    PAD_SPI_DISP_RX
);

// Forward declarations

static void bootPins();
static void bootDisplay();

static void displayDataMode();
static void displayCommandMode();
static void sendDisplayCommand(uint8_t command);

static void beginDisplaySPI();
static void setWriteRegion();


DotMGCore::DotMGCore() { }

void DotMGCore::boot()
{
  bootPins();
  bootDisplay();
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

  pinMode(PIN_SPEAKER, OUTPUT);
}

void bootDisplay()
{
  pinMode(PIN_DISP_DC, OUTPUT);
  pinMode(PIN_DISP_LED, OUTPUT);

  // Activate display SPI slave
  pinMode(PIN_SPI_DISP_SS, OUTPUT);
  digitalWrite(PIN_SPI_DISP_SS, LOW);

  dispSPI.begin();

  beginDisplaySPI();

  sendDisplayCommand(ST77XX_SWRESET);  // Software reset
  delay(150);

  DotMGCore::displayOn();

  sendDisplayCommand(ST7735_FRMCTR1);  // Framerate ctrl - normal mode
  dispSPI.transfer(0x01);               // Rate = fosc/(1x2+40) * (LINE+2C+2D)
  dispSPI.transfer(0x2C);
  dispSPI.transfer(0x2D);

  sendDisplayCommand(ST77XX_MADCTL);  // Set initial orientation
  dispSPI.transfer(MADCTL);

  sendDisplayCommand(ST77XX_COLMOD);  // Set color mode (12-bit)
  dispSPI.transfer(0x03);

  sendDisplayCommand(ST7735_GMCTRP1);  // Gamma Adjustments (pos. polarity)
  dispSPI.transfer(0x02);
  dispSPI.transfer(0x1c);
  dispSPI.transfer(0x07);
  dispSPI.transfer(0x12);
  dispSPI.transfer(0x37);
  dispSPI.transfer(0x32);
  dispSPI.transfer(0x29);
  dispSPI.transfer(0x2D);
  dispSPI.transfer(0x29);
  dispSPI.transfer(0x25);
  dispSPI.transfer(0x2B);
  dispSPI.transfer(0x39);
  dispSPI.transfer(0x00);
  dispSPI.transfer(0x01);
  dispSPI.transfer(0x03);
  dispSPI.transfer(0x10);

  sendDisplayCommand(ST7735_GMCTRN1);  // Gamma Adjustments (neg. polarity)
  dispSPI.transfer(0x03);
  dispSPI.transfer(0x1D);
  dispSPI.transfer(0x07);
  dispSPI.transfer(0x06);
  dispSPI.transfer(0x2E);
  dispSPI.transfer(0x2C);
  dispSPI.transfer(0x29);
  dispSPI.transfer(0x2D);
  dispSPI.transfer(0x2E);
  dispSPI.transfer(0x2E);
  dispSPI.transfer(0x37);
  dispSPI.transfer(0x3F);
  dispSPI.transfer(0x00);
  dispSPI.transfer(0x00);
  dispSPI.transfer(0x02);
  dispSPI.transfer(0x10);

  DotMGCore::blank();

  beginDisplaySPI();  // SPI ended by blank() call above, so begin again
  sendDisplayCommand(ST77XX_DISPON); //  Turn screen on
  delay(100);
}

void DotMGCore::blit()
{
  // Prepare for sending
  beginDisplaySPI();
  setWriteRegion();

  // Send bytes asychronously
  dispSPI.transfer(stage, NULL, stageLen, false);

  // Meanwhile, prepare next stage buffer
  uint8_t *tmp = stage;
  stage = stage2;
  stage2 = tmp;
}

void DotMGCore::blank()
{
  memset(stage, 0, stageLen);
  blit();
}

void DotMGCore::invert(bool inverse)
{
  if (inverse == inverted)
    return;

  inverted = inverse;

  beginDisplaySPI();
  sendDisplayCommand(inverse ? ST77XX_INVON : ST77XX_INVOFF);
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
  dispSPI.transfer(MADCTL);
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
  dispSPI.transfer(MADCTL);
}

void DotMGCore::allPixelsOn(bool on)
{
  beginDisplaySPI();
  sendDisplayCommand(on ? ST77XX_DISPOFF : ST77XX_DISPON);
  delay(100);
}

void DotMGCore::displayOff()
{
  *portOutputRegister(PORT_DISP_DC_LED) &= ~MASK_DISP_LED;
  beginDisplaySPI();
  sendDisplayCommand(ST77XX_SLPIN);
  delay(150);
}

void DotMGCore::displayOn()
{
  beginDisplaySPI();
  sendDisplayCommand(ST77XX_SLPOUT);
  delay(150);
  *portOutputRegister(PORT_DISP_DC_LED) |= MASK_DISP_LED;
}

uint8_t DotMGCore::buttonsState()
{
  uint32_t st_sel_up_rt = ~(*portInputRegister(PORT_ST_SEL_UP_RT));
  uint32_t a_b_dn_lf = ~(*portInputRegister(PORT_A_B_DN_LF));
  return (
    (((a_b_dn_lf & MASK_BUTTON_A) != 0) << A_BUTTON_BIT) |
    (((a_b_dn_lf & MASK_BUTTON_B) != 0) << B_BUTTON_BIT) |
    (((a_b_dn_lf & MASK_BUTTON_DOWN) != 0) << DOWN_BUTTON_BIT) |
    (((a_b_dn_lf & MASK_BUTTON_LEFT) != 0) << LEFT_BUTTON_BIT) |
    (((st_sel_up_rt & MASK_BUTTON_UP) != 0) << UP_BUTTON_BIT) |
    (((st_sel_up_rt & MASK_BUTTON_RIGHT) != 0) << RIGHT_BUTTON_BIT) |
    (((st_sel_up_rt & MASK_BUTTON_START) != 0) << START_BUTTON_BIT) |
    (((st_sel_up_rt & MASK_BUTTON_SELECT) != 0) << SELECT_BUTTON_BIT)
  );
}

void displayDataMode()
{
  *portOutputRegister(PORT_DISP_DC_LED) |= MASK_DISP_DC;
}

void displayCommandMode()
{
  *portOutputRegister(PORT_DISP_DC_LED) &= ~MASK_DISP_DC;
}

void sendDisplayCommand(uint8_t command)
{
  displayCommandMode();
  dispSPI.transfer(command);
  displayDataMode();
}

void beginDisplaySPI()
{
  dispSPI.waitForTransfer();  // Block until any DMA transfers finish
  dispSPI.endTransaction();  // End any previous transaction
  dispSPI.beginTransaction(SPI_SETTINGS_DISP); // Start new transaction
}

static void setWriteRegion()
{
  // Set column addresses
  sendDisplayCommand(ST77XX_CASET);
  dispSPI.transfer(0);
  dispSPI.transfer(0);
  dispSPI.transfer(0);
  dispSPI.transfer(WIDTH-1);

  // Set row addresses
  sendDisplayCommand(ST77XX_RASET);
  dispSPI.transfer(0);
  dispSPI.transfer(0);
  dispSPI.transfer(0);
  dispSPI.transfer(HEIGHT-1);

  // Initialize write to display RAM
  sendDisplayCommand(ST77XX_RAMWR);
}
