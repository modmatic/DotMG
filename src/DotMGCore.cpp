/**
 * @file DotMGCore.cpp
 * \brief
 * The DotMGCore class for dotMG hardware initilization and control.
 */

#include "DotMGCore.h"

#include <avr/wdt.h>

uint16_t DotMGCore::colors[] = {
  ST77XX_WHITE,
  ST77XX_GRAY,
  ST77XX_DARKGRAY,
  ST77XX_BLACK,
};
uint8_t DotMGCore::MADCTL = ST77XX_MADCTL_MV | ST77XX_MADCTL_MX;
bool DotMGCore::inverted = false;

DotMGCore::DotMGCore() { }

// Pins are set to the proper modes and levels for the specific hardware.
// This routine must be modified if any pins are moved to a different port
void DotMGCore::bootPins()
{
  // Set inputs
  DDR_RT_LFT_DN_UP_B_A_TX_RX &= ~(
    bit(BIT_BUTTON_RIGHT) |
    bit(BIT_BUTTON_LEFT)  |
    bit(BIT_BUTTON_DOWN)  |
    bit(BIT_BUTTON_UP)    |
    bit(BIT_BUTTON_B)     |
    bit(BIT_BUTTON_A)
  );
  DDR_CS_SPK_SEL_ST &= ~(
    bit(BIT_BUTTON_SELECT) |
    bit(BIT_BUTTON_START)
  );

  // Set input pullups
  PORT_RT_LFT_DN_UP_B_A_TX_RX |= (
    bit(BIT_BUTTON_RIGHT) |
    bit(BIT_BUTTON_LEFT)  |
    bit(BIT_BUTTON_DOWN)  |
    bit(BIT_BUTTON_UP)    |
    bit(BIT_BUTTON_B)     |
    bit(BIT_BUTTON_A)
  );
  PORT_CS_SPK_SEL_ST |= (
    bit(BIT_BUTTON_SELECT) |
    bit(BIT_BUTTON_START)
  );

  // Set outputs
  DDR_CS_SPK_SEL_ST |= (
    bit(BIT_TFT_CS)
  );

  DDR_SCK_MISO_MOSI_SS_DC_RST |= (
    bit(BIT_SPI_SCK)  |
    bit(BIT_SPI_MOSI) |
    bit(BIT_SPI_SS)   |
    bit(BIT_TFT_DC)   |
    bit(BIT_TFT_RST)
  );
}

void DotMGCore::bootTFT()
{
  // Reset display
  delay(5);  // Let display stay in reset
  bitSet(PORT_SCK_MISO_MOSI_SS_DC_RST, BIT_TFT_RST);  // Bring out of reset
  delay(5);

  // run our customized boot-up command sequence against the
  // TFT to initialize it properly for dotMG
  LCDCommandMode();

  startTransferSPI();

  sendLCDCommand(ST77XX_SWRESET);  // Software reset
  delay(150);

  sendLCDCommand(ST77XX_SLPOUT);  // Bring out of sleep mode
  delay(150);

  sendLCDCommand(ST7735_FRMCTR1);  // Framerate ctrl - normal mode
  transferSPI(0x01);               // Rate = fosc/(1x2+40) * (LINE+2C+2D)
  transferSPI(0x2C);
  transferSPI(0x2D);

  sendLCDCommand(ST77XX_MADCTL);  // Set initial orientation
  transferSPI(MADCTL);

  sendLCDCommand(ST77XX_COLMOD);  // Set color mode (12-bit)
  transferSPI(0x03);

  sendLCDCommand(ST7735_GMCTRP1);  // Gamma Adjustments (pos. polarity)
  transferSPI(0x02);
  transferSPI(0x1c);
  transferSPI(0x07);
  transferSPI(0x12);
  transferSPI(0x37);
  transferSPI(0x32);
  transferSPI(0x29);
  transferSPI(0x2D);
  transferSPI(0x29);
  transferSPI(0x25);
  transferSPI(0x2B);
  transferSPI(0x39);
  transferSPI(0x00);
  transferSPI(0x01);
  transferSPI(0x03);
  transferSPI(0x10);

  sendLCDCommand(ST7735_GMCTRN1);  // Gamma Adjustments (neg. polarity)
  transferSPI(0x03);
  transferSPI(0x1D);
  transferSPI(0x07);
  transferSPI(0x06);
  transferSPI(0x2E);
  transferSPI(0x2C);
  transferSPI(0x29);
  transferSPI(0x2D);
  transferSPI(0x2E);
  transferSPI(0x2E);
  transferSPI(0x37);
  transferSPI(0x3F);
  transferSPI(0x00);
  transferSPI(0x00);
  transferSPI(0x02);
  transferSPI(0x10);

  setWriteRegion();
  for (int i = 0; i < (TFT_WIDTH*TFT_HEIGHT)/2; i++) {
    transferSPI(colors[COLOR_D] >> 4);
    transferSPI(((colors[COLOR_D] & 0xF) << 4) | (colors[COLOR_D] >> 8));
    transferSPI(colors[COLOR_D]);
  }

  sendLCDCommand(ST77XX_DISPON); //  Turn screen on
  delay(100);

  endTransferSPI();

  LCDDataMode();
}

void DotMGCore::LCDDataMode()
{
  bitSet(PORT_SCK_MISO_MOSI_SS_DC_RST, BIT_TFT_DC);
}

void DotMGCore::LCDCommandMode()
{
  bitClear(PORT_SCK_MISO_MOSI_SS_DC_RST, BIT_TFT_DC);
}

// Initialize the SPI interface for the display
void DotMGCore::bootSPI()
{
  // master, mode 0, MSB first, CPU clock / 2 (8MHz)
  SPCR = bit(SPE) | bit(MSTR);
  SPSR = bit(SPI2X);
}

void DotMGCore::startTransferSPI()
{
  bitClear(PORT_CS_SPK_SEL_ST, BIT_TFT_CS);
}

void DotMGCore::endTransferSPI()
{
  bitSet(PORT_CS_SPK_SEL_ST, BIT_TFT_CS);
}

// Write to the SPI bus (MOSI pin)
void DotMGCore::transferSPI(uint8_t data)
{
  SPDR = data;
  /*
   * The following NOP introduces a small delay that can prevent the wait
   * loop form iterating when running at the maximum speed. This gives
   * about 10% more speed, even if it seems counter-intuitive. At lower
   * speeds it is unnoticed.
   */
  asm volatile(
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop\n\t"
    "nop"
  );
}

/* Power Management */

void DotMGCore::idle()
{
  SMCR = bit(SE); // select idle mode and enable sleeping
  sleep_cpu();
  SMCR = 0; // disable sleeping
}

// Shut down the display
void DotMGCore::displayOff()
{
  startTransferSPI();
  sendLCDCommand(ST77XX_SLPIN);
  endTransferSPI();
  delay(150);
}

// Restart the display after a displayOff()
void DotMGCore::displayOn()
{
  startTransferSPI();
  sendLCDCommand(ST77XX_SLPOUT);
  endTransferSPI();
  delay(150);
}


/* Drawing */

uint16_t DotMGCore::getColorValue(uint8_t color)
{
  return colors[color];
}

void DotMGCore::setColorValue(uint8_t color, uint16_t value)
{
  colors[color] = value;
}

void DotMGCore::paintScreen(const uint8_t *image)
{
  startTransferSPI();

  setWriteRegion();
  for (uint16_t r = 0; r < WIDTH*HEIGHT; r += WIDTH)
  {
    for (uint8_t i = 0; i < 2; i++)
    {
      for (uint16_t c = 0; c < WIDTH/4; c++)
      {
        const uint8_t pixels = pgm_read_byte(image + c + r);

        for (uint8_t sh = 6; sh >= 0; sh -= 2)
        {
          const uint16_t px = colors[(pixels >> sh) & 0b11];
          SPDR = px >> 4;
          asm volatile(
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop"
          );
          transferSPI(((px & 0xF) << 4) | (px >> 8));
          SPDR = px;
        }
      }
    }
  }

  endTransferSPI();
}

void DotMGCore::paintScreen(uint8_t image[], bool clear)
{
  startTransferSPI();

  setWriteRegion();
  for (uint16_t r = 0; r < WIDTH*HEIGHT; r += WIDTH)
  {
    for (uint8_t i = 0; i < 2; i++)
    {
      for (uint16_t c = 0; c < WIDTH/4; c++)
      {
        const uint8_t pixels = image[c + r];

        for (uint8_t sh = 6; sh >= 0; sh -= 2)
        {
          const uint16_t px = colors[(pixels >> sh) & 0b11];
          // SPDR = px >> 4;
          // asm volatile(
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop\n\t"
          //   "nop"
          // );
          transferSPI(px >> 4);
          transferSPI(((px & 0xF) << 4) | (px >> 8));
          // SPDR = px;
          transferSPI(px);
        }
      }
    }
  }

  endTransferSPI();

  if (clear)
  {
    memset(image, COLOR_D << 6 | COLOR_D << 4 | COLOR_D << 2 | COLOR_D, (WIDTH*HEIGHT)/4);
  }
}

void DotMGCore::blank()
{
  startTransferSPI();

  setWriteRegion();
  for (int i = 0; i < (TFT_WIDTH*TFT_HEIGHT)/2; i++)
  {
    transferSPI(colors[COLOR_D] >> 4);
    transferSPI(((colors[COLOR_D] & 0xF) << 4) | (colors[COLOR_D] >> 8));
    transferSPI(colors[COLOR_D]);
  }

  endTransferSPI();
}

void DotMGCore::sendLCDCommand(uint8_t command)
{
  LCDCommandMode();
  transferSPI(command);
  LCDDataMode();
}

void DotMGCore::setWriteRegion(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
  sendLCDCommand(ST77XX_CASET);  //  Column addr set
  transferSPI(0);
  transferSPI(x);                //  x start
  transferSPI(0);
  transferSPI(x + width - 1);   //  x end

  sendLCDCommand(ST77XX_RASET);  //  Row addr set
  transferSPI(0);
  transferSPI(y);                //  y start
  transferSPI(0);
  transferSPI(y + height - 1);    //  y end

  sendLCDCommand(ST77XX_RAMWR);  //  Initialize write to display RAM
}

// invert the display or set to normal
// when inverted, a pixel set to 0 will be on
void DotMGCore::invert(bool inverse)
{
  if (inverse == inverted)
    return;

  inverted = inverse;

  startTransferSPI();
  sendLCDCommand(inverse ? ST77XX_INVON : ST77XX_INVOFF);
  endTransferSPI();
}

// turn all display pixels on, ignoring buffer contents
// or set to normal buffer display
void DotMGCore::allPixelsOn(bool on)
{
  startTransferSPI();
  sendLCDCommand(on ? ST77XX_DISPOFF : ST77XX_DISPON);
  endTransferSPI();
  delay(100);
}

// flip the display vertically or set to normal
void DotMGCore::flipVertical(bool flipped)
{
  if (flipped)
  {
    MADCTL &= ~ST77XX_MADCTL_MX;
  }
  else
  {
    MADCTL |= ST77XX_MADCTL_MX;
  }
  startTransferSPI();
  sendLCDCommand(ST77XX_MADCTL);
  transferSPI(MADCTL);
  endTransferSPI();
}

// flip the display horizontally or set to normal
void DotMGCore::flipHorizontal(bool flipped)
{
  if (flipped)
  {
    MADCTL |= ST77XX_MADCTL_MY;
  }
  else
  {
    MADCTL &= ~ST77XX_MADCTL_MY;
  }
  startTransferSPI();
  sendLCDCommand(ST77XX_MADCTL);
  transferSPI(MADCTL);
  endTransferSPI();
}


/* Buttons */

uint8_t DotMGCore::buttonsState()
{
  return (
    (~PORTIN_RT_LFT_DN_UP_A_B_TX_RX & (
      bit(BIT_BUTTON_RIGHT) |
      bit(BIT_BUTTON_LEFT)  |
      bit(BIT_BUTTON_DOWN)  |
      bit(BIT_BUTTON_UP)    |
      bit(BIT_BUTTON_B)     |
      bit(BIT_BUTTON_A)
    )) >> 2
  ) | (
    (~PORTIN_CS_SPK_SEL_ST & (
      bit(BIT_BUTTON_SELECT) |
      bit(BIT_BUTTON_START)
    )) << 6
  );
}
