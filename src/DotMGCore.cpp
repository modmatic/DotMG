/**
 * @file DotMGCore.cpp
 * \brief
 * The DotMGCore class for hardware initilization and control.
 */

#include "DotMGCore.h"
#include <SPI.h>

#define TIMER1         TC1
#define TIMER_GCLK_ID1 TC1_GCLK_ID
#define TIMER_IRQ1     TC1_IRQn
#define TIMER1_HANDLER void TC1_Handler()

#define TIMER2         TC2
#define TIMER_GCLK_ID2 TC2_GCLK_ID
#define TIMER_IRQ2     TC2_IRQn
#define TIMER2_HANDLER void TC2_Handler()

static uint8_t MADCTL = ST77XX_MADCTL_MV | ST77XX_MADCTL_MY;
static bool inverted = false;

static const uint16_t stageLen = DISP_WIDTH*DISP_HEIGHT*12/8; // 12 bits/px, 8 bits/byte
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
static void bootAudio();

static void displayDataMode();
static void displayCommandMode();
static void sendDisplayCommand(uint8_t command);

static void beginDisplaySPI();
static void setWriteRegion();

static void timer_init(Tc *TCx, unsigned int clkId, IRQn_Type irqn);
static void toggle(uint8_t chan) __attribute__((always_inline));


DotMGCore::DotMGCore() { }

void DotMGCore::boot()
{
  bootPins();
  bootDisplay();
  bootAudio();
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

void bootAudio()
{
  pinMode(PIN_SPEAKER, OUTPUT);
  DotMGCore::enableAudio(true);
  timer_init(TIMER1, TIMER_GCLK_ID1, TIMER_IRQ1);
  timer_init(TIMER2, TIMER_GCLK_ID2, TIMER_IRQ2);
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
  dispSPI.transfer(DISP_WIDTH-1);

  // Set row addresses
  sendDisplayCommand(ST77XX_RASET);
  dispSPI.transfer(0);
  dispSPI.transfer(0);
  dispSPI.transfer(0);
  dispSPI.transfer(DISP_HEIGHT-1);

  // Initialize write to display RAM
  sendDisplayCommand(ST77XX_RAMWR);
}

void timer_init(Tc *TCx, unsigned int clkId, IRQn_Type irqn)
{
  // Enable GCLK for timer
  GCLK->PCHCTRL[clkId].reg = GCLK_PCHCTRL_GEN_GCLK0_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);

  // Disable counter
  TCx->COUNT16.CTRLA.bit.ENABLE = 0;
  while (TCx->COUNT16.SYNCBUSY.bit.ENABLE);

  // Reset counter
  TCx->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (TCx->COUNT16.SYNCBUSY.bit.ENABLE);
  while (TCx->COUNT16.CTRLA.bit.SWRST);

  // Set to match frequency mode
  TCx->COUNT16.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;

  // Set to 16-bit counter, clk/16 prescaler
  TCx->COUNT16.CTRLA.reg = (
    TC_CTRLA_MODE_COUNT16 |
    TC_CTRLA_PRESCALER_DIV64
  );
  while (TCx->COUNT16.SYNCBUSY.bit.ENABLE);

  // Configure interrupt request
  NVIC_DisableIRQ(irqn);
  NVIC_ClearPendingIRQ(irqn);
  NVIC_SetPriority(irqn, 0);
  NVIC_EnableIRQ(irqn);

  // Enable interrupt request
  TCx->COUNT16.INTENSET.bit.MC0 = 1;
  while (TCx->COUNT16.SYNCBUSY.bit.ENABLE);
}

static void timer_tone(Tc *TCx, float freq)
{
  // Set counter based on desired frequency
  TCx->COUNT16.CC[0].reg = (uint16_t)((F_CPU / 64 / 2 / freq) - 1);

  // Enable counter
  TCx->COUNT16.CTRLA.bit.ENABLE = 1;
  while (TCx->COUNT16.SYNCBUSY.bit.ENABLE);
}

static void timer_stop(Tc *TCx)
{
  // Disable counter
  TCx->COUNT16.CTRLA.bit.ENABLE = 0;
  while (TCx->COUNT16.SYNCBUSY.bit.ENABLE);
}

static volatile int64_t toggleCount[2];
static volatile bool wave[2];
static Tc *timer[] = {TIMER1, TIMER2};

void DotMGCore::enableAudio(bool enable)
{
  while (DAC->SYNCBUSY.bit.ENABLE || DAC->SYNCBUSY.bit.SWRST);
  DAC->CTRLA.bit.ENABLE = 0;     // disable DAC

  while (DAC->SYNCBUSY.bit.ENABLE || DAC->SYNCBUSY.bit.SWRST);
  DAC->DACCTRL[DAC_CH_SPEAKER].bit.ENABLE = enable;  // enable/disable channel

  while (DAC->SYNCBUSY.bit.ENABLE || DAC->SYNCBUSY.bit.SWRST);
  DAC->CTRLA.bit.ENABLE = 1;     // enable DAC

  if (enable)
  {
    while (!DAC_READY);
    while (DAC_DATA_BUSY);
    DAC->DATA[DAC_CH_SPEAKER].reg = 0;
    delay(10);
  }
}

bool DotMGCore::audioEnabled()
{
  return DAC->DACCTRL[DAC_CH_SPEAKER].bit.ENABLE;
}

void DotMGCore::tone(uint8_t chan, float freq)
{
  tone(chan, freq, 0);
}

void DotMGCore::tone(uint8_t chan, float freq, uint16_t dur)
{
  toggleCount[chan] = (dur > 0 ? freq * dur * 2 / 1000UL : -1LL);
  timer_tone(timer[chan], freq);
}

void DotMGCore::stopTone(uint8_t chan)
{
  timer_stop(timer[chan]);
  wave[chan] = false;
}

bool DotMGCore::tonePlaying(uint8_t chan)
{
  return timer[chan]->COUNT16.CTRLA.bit.ENABLE;
}

void toggle(uint8_t chan)
{
  if (toggleCount[chan] > 0)
  {
    toggleCount[chan]--;

    if (!DotMGCore::audioEnabled())
      return;

    while (!DAC_READY);
    while (DAC_DATA_BUSY);
    wave[chan] = !wave[chan];
    DAC->DATA[DAC_CH_SPEAKER].reg = (wave[TONE_CH1] ? 2047 : 0) + (wave[TONE_CH2] ? 2047 : 0);
  }
  else
  {
    DotMGCore::stopTone(chan);
  }
}

TIMER1_HANDLER
{
  toggle(TONE_CH1);
  TIMER1->COUNT16.INTFLAG.bit.MC0 = 1;  // Clear interrupt
}

TIMER2_HANDLER
{
  toggle(TONE_CH2);
  TIMER2->COUNT16.INTFLAG.bit.MC0 = 1;  // Clear interrupt
}
