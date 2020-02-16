/**
 * @file DotMG.cpp
 * \brief
 * The DotMGBase and DotMG classes and support objects and definitions.
 */

#include "DotMG.h"
#include "ab_logo.c"
#include "glcdfont.c"

//================================
//========== class Rect ==========
//================================

Rect::Rect(int16_t x, int16_t y, uint8_t width, uint8_t height)
 : x(x), y(y), width(width), height(height)
{
}

//=================================
//========== class Point ==========
//=================================

Point::Point(int16_t x, int16_t y)
 : x(x), y(y)
{
}

//========================================
//========== class DotMGBase ==========
//========================================

uint8_t DotMGBase::sBuffer[];

DotMGBase::DotMGBase()
{
  currentButtonState = 0;
  previousButtonState = 0;
  // frame management
  setFrameDuration(16);
  frameCount = 0;
  justRendered = false;
}

// functions called here should be public so users can create their
// own init functions if they need different behavior than `begin`
// provides by default
void DotMGBase::begin()
{
  boot(); // raw hardware

  flashlight(); // light the RGB LED and screen if UP button is being held.

  // check for and handle buttons held during start up for system control
  systemButtons();

  audio.begin();

  bootLogo();
  // alternative logo functions. Work the same as bootLogo() but may reduce
  // memory size if the sketch uses the same bitmap drawing function
//  bootLogoCompressed();
//  bootLogoSpritesSelfMasked();
//  bootLogoSpritesOverwrite();
//  bootLogoSpritesBSelfMasked();
//  bootLogoSpritesBOverwrite();

  waitNoButtons(); // wait for all buttons to be released
}

void DotMGBase::flashlight()
{
  if (!pressed(UP_BUTTON)) {
    return;
  }

  allPixelsOn(true);

  while (true) {
    idle();
  }
}

void DotMGBase::systemButtons()
{
  while (pressed(B_BUTTON)) {
    digitalWriteRGB(BLUE_LED, RGB_ON); // turn on blue LED
    sysCtrlSound(UP_BUTTON + B_BUTTON, GREEN_LED, 0xff);
    sysCtrlSound(DOWN_BUTTON + B_BUTTON, RED_LED, 0);
    delay(200);
  }

  digitalWriteRGB(BLUE_LED, RGB_OFF); // turn off blue LED
}

void DotMGBase::sysCtrlSound(uint8_t buttons, uint8_t led, uint8_t eeVal)
{
  if (pressed(buttons)) {
    digitalWriteRGB(BLUE_LED, RGB_OFF); // turn off blue LED
    delay(200);
    digitalWriteRGB(led, RGB_ON); // turn on "acknowledge" LED
    EEPROM.update(EEPROM_AUDIO_ON_OFF, eeVal);
    delay(500);
    digitalWriteRGB(led, RGB_OFF); // turn off "acknowledge" LED

    while (pressed(buttons)) { } // Wait for button release
  }
}

void DotMGBase::bootLogo()
{
  bootLogoShell(drawLogoBitmap);
}

void DotMGBase::drawLogoBitmap(int16_t y)
{
  drawBitmap(20, y, arduboy_logo, 88, 16);
}

void DotMGBase::bootLogoCompressed()
{
  bootLogoShell(drawLogoCompressed);
}

void DotMGBase::drawLogoCompressed(int16_t y)
{
  drawCompressed(20, y, arduboy_logo_compressed);
}

void DotMGBase::bootLogoSpritesSelfMasked()
{
  bootLogoShell(drawLogoSpritesSelfMasked);
}

void DotMGBase::drawLogoSpritesSelfMasked(int16_t y)
{
  Sprites::drawSelfMasked(20, y, arduboy_logo_sprite, 0);
}

void DotMGBase::bootLogoSpritesOverwrite()
{
  bootLogoShell(drawLogoSpritesOverwrite);
}

void DotMGBase::drawLogoSpritesOverwrite(int16_t y)
{
  Sprites::drawOverwrite(20, y, arduboy_logo_sprite, 0);
}

void DotMGBase::bootLogoSpritesBSelfMasked()
{
  bootLogoShell(drawLogoSpritesBSelfMasked);
}

void DotMGBase::drawLogoSpritesBSelfMasked(int16_t y)
{
  SpritesB::drawSelfMasked(20, y, arduboy_logo_sprite, 0);
}

void DotMGBase::bootLogoSpritesBOverwrite()
{
  bootLogoShell(drawLogoSpritesBOverwrite);
}

void DotMGBase::drawLogoSpritesBOverwrite(int16_t y)
{
  SpritesB::drawOverwrite(20, y, arduboy_logo_sprite, 0);
}

// bootLogoText() should be kept in sync with bootLogoShell()
// if changes are made to one, equivalent changes should be made to the other
void DotMGBase::bootLogoShell(void (*drawLogo)(int16_t))
{
  bool showLEDs = readShowBootLogoLEDsFlag();

  if (!readShowBootLogoFlag()) {
    return;
  }

  if (showLEDs) {
    digitalWriteRGB(RED_LED, RGB_ON);
  }

  for (int16_t y = -16; y <= 24; y++) {
    if (pressed(RIGHT_BUTTON)) {
      digitalWriteRGB(RGB_OFF, RGB_OFF, RGB_OFF); // all LEDs off
      return;
    }

    if (showLEDs && y == 4) {
      digitalWriteRGB(RED_LED, RGB_OFF);    // red LED off
      digitalWriteRGB(GREEN_LED, RGB_ON);   // green LED on
    }

    // Using display(CLEAR_BUFFER) instead of clear() may save code space.
    // The extra time it takes to repaint the previous logo isn't an issue.
    display(CLEAR_BUFFER);
    (*drawLogo)(y); // call the function that actually draws the logo
    display();
    delay(15);
  }

  if (showLEDs) {
    digitalWriteRGB(GREEN_LED, RGB_OFF);  // green LED off
    digitalWriteRGB(BLUE_LED, RGB_ON);    // blue LED on
  }
  delay(400);
  digitalWriteRGB(BLUE_LED, RGB_OFF);

  bootLogoExtra();
}

// Virtual function overridden by derived class
void DotMGBase::bootLogoExtra() { }

// wait for all buttons to be released
void DotMGBase::waitNoButtons() {
  do {
    delay(50); // simple button debounce
  } while (buttonsState());
}


/* Frame management */

void DotMGBase::setFrameRate(uint8_t rate)
{
  eachFrameMillis = 1000 / rate;
}

void DotMGBase::setFrameDuration(uint8_t duration)
{
  eachFrameMillis = duration;
}

bool DotMGBase::everyXFrames(uint8_t frames)
{
  return frameCount % frames == 0;
}

bool DotMGBase::nextFrame()
{
  uint8_t now = (uint8_t) millis();
  uint8_t frameDurationMs = now - thisFrameStart;

  if (justRendered) {
    lastFrameDurationMs = frameDurationMs;
    justRendered = false;
    return false;
  }
  else if (frameDurationMs < eachFrameMillis) {
    // Only idle if at least a full millisecond remains, since idle() may
    // sleep the processor until the next millisecond timer interrupt.
    if (++frameDurationMs < eachFrameMillis) {
      idle();
    }

    return false;
  }

  // pre-render
  justRendered = true;
  thisFrameStart = now;
  frameCount++;

  return true;
}

int DotMGBase::cpuLoad()
{
  return lastFrameDurationMs*100 / eachFrameMillis;
}

unsigned long DotMGBase::generateRandomSeed()
{
  return micros();
}

void DotMGBase::initRandomSeed()
{
  randomSeed(generateRandomSeed());
}


/* Graphics */

void DotMGBase::clear()
{
  fillScreen(BLACK);
}

void DotMGBase::drawPixel(int16_t x, int16_t y, uint8_t color)
{
  #ifdef PIXEL_SAFE_MODE
  if (x < 0 || x > (WIDTH-1) || y < 0 || y > (HEIGHT-1))
  {
    return;
  }
  #endif

  uint16_t row_offset;
  uint8_t bit;

  bit = 1 << (y & 7);
  row_offset = y / 8 * WIDTH + x;
  uint8_t data = sBuffer[row_offset] | bit;
  if (!(color & bit(0))) data ^= bit;
  sBuffer[row_offset] = data;
}

uint8_t DotMGBase::getPixel(uint8_t x, uint8_t y)
{
  uint8_t row = y / 8;
  uint8_t bit_position = y % 8;
  return (sBuffer[(row*WIDTH) + x] & bit(bit_position)) >> bit_position;
}

void DotMGBase::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0, y0+r, color);
  drawPixel(x0, y0-r, color);
  drawPixel(x0+r, y0, color);
  drawPixel(x0-r, y0, color);

  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

void DotMGBase::drawCircleHelper
(int16_t x0, int16_t y0, uint8_t r, uint8_t corners, uint8_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    if (corners & 0x4) // lower right
    {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    }
    if (corners & 0x2) // upper right
    {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (corners & 0x8) // lower left
    {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (corners & 0x1) // upper left
    {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

void DotMGBase::fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

void DotMGBase::fillCircleHelper
(int16_t x0, int16_t y0, uint8_t r, uint8_t sides, int16_t delta,
 uint8_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x < y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    if (sides & 0x1) // right side
    {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }

    if (sides & 0x2) // left side
    {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

void DotMGBase::drawLine
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
  // bresenham's algorithm - thx wikpedia
  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1)
  {
    ystep = 1;
  }
  else
  {
    ystep = -1;
  }

  for (; x0 <= x1; x0++)
  {
    if (steep)
    {
      drawPixel(y0, x0, color);
    }
    else
    {
      drawPixel(x0, y0, color);
    }

    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}

void DotMGBase::drawRect
(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void DotMGBase::drawFastVLine
(int16_t x, int16_t y, uint8_t h, uint8_t color)
{
  int end = y+h;
  for (int a = max(0,y); a < min(end,HEIGHT); a++)
  {
    drawPixel(x,a,color);
  }
}

void DotMGBase::drawFastHLine
(int16_t x, int16_t y, uint8_t w, uint8_t color)
{
  int16_t xEnd; // last x point + 1

  // Do y bounds checks
  if (y < 0 || y >= HEIGHT)
    return;

  xEnd = x + w;

  // Check if the entire line is not on the display
  if (xEnd <= 0 || x >= WIDTH)
    return;

  // Don't start before the left edge
  if (x < 0)
    x = 0;

  // Don't end past the right edge
  if (xEnd > WIDTH)
    xEnd = WIDTH;

  // calculate actual width (even if unchanged)
  w = xEnd - x;

  // buffer pointer plus row offset + x offset
  register uint8_t *pBuf = sBuffer + ((y / 8) * WIDTH) + x;

  // pixel mask
  register uint8_t mask = 1 << (y & 7);

  switch (color)
  {
    case WHITE:
      while (w--)
      {
        *pBuf++ |= mask;
      }
      break;

    case BLACK:
      mask = ~mask;
      while (w--)
      {
        *pBuf++ &= mask;
      }
      break;
  }
}

void DotMGBase::fillRect
(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
  // stupidest version - update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++)
  {
    drawFastVLine(i, y, h, color);
  }
}

void DotMGBase::fillScreen(uint8_t color)
{
  if (color != BLACK)
  {
    color = 0xFF; // all pixels on
  }
  memset(sBuffer, color, WIDTH * HEIGHT / 8);
}

void DotMGBase::drawRoundRect
(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
  // smarter version
  drawFastHLine(x+r, y, w-2*r, color); // Top
  drawFastHLine(x+r, y+h-1, w-2*r, color); // Bottom
  drawFastVLine(x, y+r, h-2*r, color); // Left
  drawFastVLine(x+w-1, y+r, h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r, y+r, r, 1, color);
  drawCircleHelper(x+w-r-1, y+r, r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r, y+h-r-1, r, 8, color);
}

void DotMGBase::fillRoundRect
(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r, y+r, r, 2, h-2*r-1, color);
}

void DotMGBase::drawTriangle
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

void DotMGBase::fillTriangle
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

  int16_t a, b, y, last;
  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1)
  {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2)
  {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1)
  {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2)
  { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)
    {
      a = x1;
    }
    else if(x1 > b)
    {
      b = x1;
    }
    if(x2 < a)
    {
      a = x2;
    }
    else if(x2 > b)
    {
      b = x2;
    }
    drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t dx01 = x1 - x0,
      dy01 = y1 - y0,
      dx02 = x2 - x0,
      dy02 = y2 - y0,
      dx12 = x2 - x1,
      dy12 = y2 - y1,
      sa = 0,
      sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2)
  {
    last = y1;   // Include y1 scanline
  }
  else
  {
    last = y1-1; // Skip it
  }


  for(y = y0; y <= last; y++)
  {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if(a > b)
    {
      swap(a,b);
    }

    drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);

  for(; y <= y2; y++)
  {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if(a > b)
    {
      swap(a,b);
    }

    drawFastHLine(a, y, b-a+1, color);
  }
}

void DotMGBase::drawBitmap
(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h,
 uint8_t color)
{
  // no need to draw at all if we're offscreen
  if (x+w < 0 || x > WIDTH-1 || y+h < 0 || y > HEIGHT-1)
    return;

  int yOffset = abs(y) % 8;
  int sRow = y / 8;
  if (y < 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }
  int rows = h/8;
  if (h%8!=0) rows++;
  for (int a = 0; a < rows; a++) {
    int bRow = sRow + a;
    if (bRow > (HEIGHT/8)-1) break;
    if (bRow > -2) {
      for (int iCol = 0; iCol<w; iCol++) {
        if (iCol + x > (WIDTH-1)) break;
        if (iCol + x >= 0) {
          if (bRow >= 0) {
            if (color == WHITE)
              sBuffer[(bRow*WIDTH) + x + iCol] |= pgm_read_byte(bitmap+(a*w)+iCol) << yOffset;
            else if (color == BLACK)
              sBuffer[(bRow*WIDTH) + x + iCol] &= ~(pgm_read_byte(bitmap+(a*w)+iCol) << yOffset);
            else
              sBuffer[(bRow*WIDTH) + x + iCol] ^= pgm_read_byte(bitmap+(a*w)+iCol) << yOffset;
          }
          if (yOffset && bRow<(HEIGHT/8)-1 && bRow > -2) {
            if (color == WHITE)
              sBuffer[((bRow+1)*WIDTH) + x + iCol] |= pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset);
            else if (color == BLACK)
              sBuffer[((bRow+1)*WIDTH) + x + iCol] &= ~(pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset));
            else
              sBuffer[((bRow+1)*WIDTH) + x + iCol] ^= pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset);
          }
        }
      }
    }
  }
}


void DotMGBase::drawSlowXYBitmap
(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color)
{
  // no need to draw at all of we're offscreen
  if (x+w < 0 || x > WIDTH-1 || y+h < 0 || y > HEIGHT-1)
    return;

  int16_t xi, yi, byteWidth = (w + 7) / 8;
  for(yi = 0; yi < h; yi++) {
    for(xi = 0; xi < w; xi++ ) {
      if(pgm_read_byte(bitmap + yi * byteWidth + xi / 8) & (128 >> (xi & 7))) {
        drawPixel(x + xi, y + yi, color);
      }
    }
  }
}


// Helper for drawCompressed()
struct BitStreamReader
{
  const uint8_t *source;
  uint16_t sourceIndex;
  uint8_t bitBuffer;
  uint8_t byteBuffer;

  BitStreamReader(const uint8_t *source)
    : source(source), sourceIndex(), bitBuffer(), byteBuffer()
  {
  }

  uint16_t readBits(uint16_t bitCount)
  {
    uint16_t result = 0;
    for (uint16_t i = 0; i < bitCount; i++)
    {
      if (this->bitBuffer == 0)
      {
        this->bitBuffer = 0x1;
        this->byteBuffer = pgm_read_byte(&this->source[this->sourceIndex]);
        ++this->sourceIndex;
      }

      if ((this->byteBuffer & this->bitBuffer) != 0)
        result |= (1 << i);

      this->bitBuffer <<= 1;
    }
    return result;
  }
};

void DotMGBase::drawCompressed(int16_t sx, int16_t sy, const uint8_t *bitmap, uint8_t color)
{
  // set up decompress state
  BitStreamReader cs = BitStreamReader(bitmap);

  // read header
  int width = (int)cs.readBits(8) + 1;
  int height = (int)cs.readBits(8) + 1;
  uint8_t spanColour = (uint8_t)cs.readBits(1); // starting colour

  // no need to draw at all if we're offscreen
  if ((sx + width < 0) || (sx > WIDTH - 1) || (sy + height < 0) || (sy > HEIGHT - 1))
    return;

  // sy = sy - (frame * height);
  int yOffset = abs(sy) % 8;
  int startRow = sy / 8;
  if (sy < 0) {
    startRow--;
    yOffset = 8 - yOffset;
  }
  int rows = height / 8;
  if ((height % 8) != 0)
    ++rows;

  int rowOffset = 0; // +(frame*rows);
  int columnOffset = 0;

  uint8_t byte = 0x00;
  uint8_t bit = 0x01;
  while (rowOffset < rows) // + (frame*rows))
  {
    uint16_t bitLength = 1;
    while (cs.readBits(1) == 0)
      bitLength += 2;

    uint16_t len = cs.readBits(bitLength) + 1; // span length

    // draw the span
    for (uint16_t i = 0; i < len; ++i)
    {
      if (spanColour != 0)
        byte |= bit;
      bit <<= 1;

      if (bit == 0) // reached end of byte
      {
        // draw
        int bRow = startRow + rowOffset;

        //if (byte) // possible optimisation
        if ((bRow <= (HEIGHT / 8) - 1) && (bRow > -2) &&
            (columnOffset + sx <= (WIDTH - 1)) && (columnOffset + sx >= 0))
        {
          int16_t offset = (bRow * WIDTH) + sx + columnOffset;
          if (bRow >= 0)
          {
            int16_t index = offset;
            uint8_t value = byte << yOffset;

            if (color != 0)
              sBuffer[index] |= value;
            else
              sBuffer[index] &= ~value;
          }
          if ((yOffset != 0) && (bRow < (HEIGHT / 8) - 1))
          {
            int16_t index = offset + WIDTH;
            uint8_t value = byte >> (8 - yOffset);

            if (color != 0)
              sBuffer[index] |= value;
            else
              sBuffer[index] &= ~value;
          }
        }

        // iterate
        ++columnOffset;
        if (columnOffset >= width)
        {
          columnOffset = 0;
          ++rowOffset;
        }

        // reset byte
        byte = 0x00;
        bit = 0x01;
      }
    }

    spanColour ^= 0x01; // toggle colour bit (bit 0) for next span
  }
}

void DotMGBase::display()
{
  paintScreen(sBuffer);
}

void DotMGBase::display(bool clear)
{
  paintScreen(sBuffer, clear);
}

uint8_t* DotMGBase::getBuffer()
{
  return sBuffer;
}

bool DotMGBase::pressed(uint8_t buttons)
{
  return (buttonsState() & buttons) == buttons;
}

bool DotMGBase::notPressed(uint8_t buttons)
{
  return (buttonsState() & buttons) == 0;
}

void DotMGBase::pollButtons()
{
  previousButtonState = currentButtonState;
  currentButtonState = buttonsState();
}

bool DotMGBase::justPressed(uint8_t button)
{
  return (!(previousButtonState & button) && (currentButtonState & button));
}

bool DotMGBase::justReleased(uint8_t button)
{
  return ((previousButtonState & button) && !(currentButtonState & button));
}

bool DotMGBase::collide(Point point, Rect rect)
{
  return ((point.x >= rect.x) && (point.x < rect.x + rect.width) &&
      (point.y >= rect.y) && (point.y < rect.y + rect.height));
}

bool DotMGBase::collide(Rect rect1, Rect rect2)
{
  return !(rect2.x                >= rect1.x + rect1.width  ||
           rect2.x + rect2.width  <= rect1.x                ||
           rect2.y                >= rect1.y + rect1.height ||
           rect2.y + rect2.height <= rect1.y);
}

uint16_t DotMGBase::readUnitID()
{
  return EEPROM.read(EEPROM_UNIT_ID) |
         (((uint16_t)(EEPROM.read(EEPROM_UNIT_ID + 1))) << 8);
}

void DotMGBase::writeUnitID(uint16_t id)
{
  EEPROM.update(EEPROM_UNIT_ID, (uint8_t)(id & 0xff));
  EEPROM.update(EEPROM_UNIT_ID + 1, (uint8_t)(id >> 8));
}

uint8_t DotMGBase::readUnitName(char* name)
{
  char val;
  uint8_t dest;
  uint8_t src = EEPROM_UNIT_NAME;

  for (dest = 0; dest < ARDUBOY_UNIT_NAME_LEN; dest++)
  {
    val = EEPROM.read(src);
    name[dest] = val;
    src++;
    if (val == 0x00 || (byte)val == 0xFF) {
      break;
    }
  }

  name[dest] = 0x00;
  return dest;
}

void DotMGBase::writeUnitName(char* name)
{
  bool done = false;
  uint8_t dest = EEPROM_UNIT_NAME;

  for (uint8_t src = 0; src < ARDUBOY_UNIT_NAME_LEN; src++)
  {
    if (name[src] == 0x00) {
      done = true;
    }
    // write character or 0 pad if finished
    EEPROM.update(dest, done ? 0x00 : name[src]);
    dest++;
  }
}

bool DotMGBase::readShowBootLogoFlag()
{
  return (EEPROM.read(EEPROM_SYS_FLAGS) & SYS_FLAG_SHOW_LOGO_MASK);
}

void DotMGBase::writeShowBootLogoFlag(bool val)
{
  uint8_t flags = EEPROM.read(EEPROM_SYS_FLAGS);
  bitWrite(flags, SYS_FLAG_SHOW_LOGO, val);
  EEPROM.update(EEPROM_SYS_FLAGS, flags);
}

bool DotMGBase::readShowUnitNameFlag()
{
  return (EEPROM.read(EEPROM_SYS_FLAGS) & SYS_FLAG_UNAME_MASK);
}

void DotMGBase::writeShowUnitNameFlag(bool val)
{
  uint8_t flags = EEPROM.read(EEPROM_SYS_FLAGS);
  bitWrite(flags, SYS_FLAG_UNAME, val);
  EEPROM.update(EEPROM_SYS_FLAGS, flags);
}

bool DotMGBase::readShowBootLogoLEDsFlag()
{
  return (EEPROM.read(EEPROM_SYS_FLAGS) & SYS_FLAG_SHOW_LOGO_LEDS_MASK);
}

void DotMGBase::writeShowBootLogoLEDsFlag(bool val)
{
  uint8_t flags = EEPROM.read(EEPROM_SYS_FLAGS);
  bitWrite(flags, SYS_FLAG_SHOW_LOGO_LEDS, val);
  EEPROM.update(EEPROM_SYS_FLAGS, flags);
}

void DotMGBase::swap(int16_t& a, int16_t& b)
{
  int16_t temp = a;
  a = b;
  b = temp;
}


//====================================
//========== class DotMG ==========
//====================================

DotMG::DotMG()
{
  cursor_x = 0;
  cursor_y = 0;
  textColor = 1;
  textBackground = 0;
  textSize = 1;
  textWrap = 0;
}

// bootLogoText() should be kept in sync with bootLogoShell()
// if changes are made to one, equivalent changes should be made to the other
void DotMG::bootLogoText()
{
  if (!readShowBootLogoFlag()) {
    return;
  }

  for (int16_t y = -16; y <= 24; y++) {
    // Using display(CLEAR_BUFFER) instead of clear() may save code space.
    // The extra time it takes to repaint the previous logo isn't an issue.
    display(CLEAR_BUFFER);
    cursor_x = 23;
    cursor_y = y;
    textSize = 2;
    print(F("ARDUBOY"));
    textSize = 1;
    display();
    delay(11);
  }

  delay(400);
  bootLogoExtra();
}

void DotMG::bootLogoExtra()
{
  uint8_t c;
  const uint8_t forText[] = "for dotMG";

  cursor_x = (WIDTH - (sizeof(forText)*6))/2;
  cursor_y = 44;
  for (int i = 0; i < sizeof(forText); i++) {
    write(forText[i]);
  }

  display();
  delay(1000);

  if (!readShowUnitNameFlag())
  {
    return;
  }

  c = EEPROM.read(EEPROM_UNIT_NAME);

  if (c != 0xFF && c != 0x00)
  {
    uint8_t i = EEPROM_UNIT_NAME;
    cursor_x = cursor_x = (WIDTH - ARDUBOY_UNIT_NAME_LEN*6)/2;
    cursor_y = 56;

    do
    {
      write(c);
      c = EEPROM.read(++i);
    }
    while (i < EEPROM_UNIT_NAME + ARDUBOY_UNIT_NAME_LEN);

    display();
    delay(1000);
  }
}

size_t DotMG::write(uint8_t c)
{
  if (c == '\n')
  {
    cursor_y += textSize * 8;
    cursor_x = 0;
  }
  else if (c == '\r')
  {
    // skip em
  }
  else
  {
    drawChar(cursor_x, cursor_y, c, textColor, textBackground, textSize);
    cursor_x += textSize * 6;
    if (textWrap && (cursor_x > (WIDTH - textSize * 6)))
    {
      // calling ourselves recursively for 'newline' is
      // 12 bytes smaller than doing the same math here
      write('\n');
    }
  }
  return 1;
}

void DotMG::drawChar
  (int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size)
{
  uint8_t line;
  bool draw_background = bg != color;
  const unsigned char* bitmap = font + c * 5;

  if ((x >= WIDTH) ||              // Clip right
      (y >= HEIGHT) ||             // Clip bottom
      ((x + 5 * size - 1) < 0) ||  // Clip left
      ((y + 8 * size - 1) < 0)     // Clip top
     )
  {
    return;
  }

  for (uint8_t i = 0; i < 6; i++ )
  {
    line = pgm_read_byte(bitmap++);
    if (i == 5) {
      line = 0x0;
    }

    for (uint8_t j = 0; j < 8; j++)
    {
      uint8_t draw_color = (line & 0x1) ? color : bg;

      if (draw_color || draw_background) {
        for (uint8_t a = 0; a < size; a++ ) {
          for (uint8_t b = 0; b < size; b++ ) {
            drawPixel(x + (i * size) + a, y + (j * size) + b, draw_color);
          }
        }
      }
      line >>= 1;
    }
  }
}

void DotMG::setCursor(int16_t x, int16_t y)
{
  cursor_x = x;
  cursor_y = y;
}

int16_t DotMG::getCursorX()
{
  return cursor_x;
}

int16_t DotMG::getCursorY()
{
  return cursor_y;
}

void DotMG::setTextColor(uint8_t color)
{
  textColor = color;
}

uint8_t DotMG::getTextColor()
{
  return textColor;
}

void DotMG::setTextBackground(uint8_t bg)
{
  textBackground = bg;
}

uint8_t DotMG::getTextBackground()
{
  return textBackground;
}

void DotMG::setTextSize(uint8_t s)
{
  // size must always be 1 or higher
  textSize = max(1, s);
}

uint8_t DotMG::getTextSize()
{
  return textSize;
}

void DotMG::setTextWrap(bool w)
{
  textWrap = w;
}

bool DotMG::getTextWrap()
{
  return textWrap;
}

void DotMG::clear()
{
    DotMGBase::clear();
    cursor_x = cursor_y = 0;
}
