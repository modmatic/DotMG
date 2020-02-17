/**
 * @file DotMG.cpp
 * \brief
 * The DotMGBase and DotMG classes and support objects and definitions.
 */

#include "DotMG.h"
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

static uint8_t currentButtonState;
static uint8_t previousButtonState;

static uint8_t frameBuf[frameBufLen];
static uint8_t currFrame;
static uint8_t eachFrameMillis = 16;
static uint8_t thisFrameStart;
static bool justRendered;
static uint8_t lastFrameDurationMs;

// Draw one or more "corners" of a circle.
static void drawCircleHelper(int16_t x0, int16_t y0, uint8_t r, uint8_t corners, Color color = COLOR_WHITE);

// Draw one or both vertical halves of a filled-in circle or rounded rectangle edge.
static void fillCircleHelper(int16_t x0, int16_t y0, uint8_t r, uint8_t sides, int16_t delta, uint16_t color = COLOR_WHITE);

static void swap(int16_t &a, int16_t &b);


void DotMGBase::begin()
{
  boot();
  waitNoButtons();
}

/* Graphics */

void DotMGBase::clear()
{
  // fillScreen(COLOR_BLACK);
}

void DotMGBase::display()
{
  paintScreen(frameBuf);
}

void DotMGBase::display(bool clear)
{
  paintScreen(frameBuf, clear);
}

void DotMGBase::drawPixel(int16_t x, int16_t y, Color color)
{
  if (x < 0 || x > (WIDTH-1) || y < 0 || y > (HEIGHT-1))
    return;

  // TODO
  // uint16_t row_offset;
  // uint8_t bit;

  // bit = 1 << (y & 7);
  // row_offset = y / 8 * WIDTH + x;
  // uint8_t data = sBuffer[row_offset] | bit;
  // if (!(color & bit(0))) data ^= bit;
  // sBuffer[row_offset] = data;
}

Color DotMGBase::getPixel(uint8_t x, uint8_t y)
{
  // TODO
  // uint8_t row = y / 8;
  // uint8_t bit_position = y % 8;
  // return (sBuffer[(row*WIDTH) + x] & bit(bit_position)) >> bit_position;
  return COLOR_CLEAR;
}

// void DotMGBase::drawCircle(int16_t x0, int16_t y0, uint8_t r, Color color)
// {
//   int16_t f = 1 - r;
//   int16_t ddF_x = 1;
//   int16_t ddF_y = -2 * r;
//   int16_t x = 0;
//   int16_t y = r;

//   drawPixel(x0, y0+r, color);
//   drawPixel(x0, y0-r, color);
//   drawPixel(x0+r, y0, color);
//   drawPixel(x0-r, y0, color);

//   while (x<y)
//   {
//     if (f >= 0)
//     {
//       y--;
//       ddF_y += 2;
//       f += ddF_y;
//     }

//     x++;
//     ddF_x += 2;
//     f += ddF_x;

//     drawPixel(x0 + x, y0 + y, color);
//     drawPixel(x0 - x, y0 + y, color);
//     drawPixel(x0 + x, y0 - y, color);
//     drawPixel(x0 - x, y0 - y, color);
//     drawPixel(x0 + y, y0 + x, color);
//     drawPixel(x0 - y, y0 + x, color);
//     drawPixel(x0 + y, y0 - x, color);
//     drawPixel(x0 - y, y0 - x, color);
//   }
// }

// void drawCircleHelper(int16_t x0, int16_t y0, uint8_t r, uint8_t corners, Color color)
// {
//   int16_t f = 1 - r;
//   int16_t ddF_x = 1;
//   int16_t ddF_y = -2 * r;
//   int16_t x = 0;
//   int16_t y = r;

//   while (x<y)
//   {
//     if (f >= 0)
//     {
//       y--;
//       ddF_y += 2;
//       f += ddF_y;
//     }

//     x++;
//     ddF_x += 2;
//     f += ddF_x;

//     if (corners & 0x4) // lower right
//     {
//       drawPixel(x0 + x, y0 + y, color);
//       drawPixel(x0 + y, y0 + x, color);
//     }
//     if (corners & 0x2) // upper right
//     {
//       drawPixel(x0 + x, y0 - y, color);
//       drawPixel(x0 + y, y0 - x, color);
//     }
//     if (corners & 0x8) // lower left
//     {
//       drawPixel(x0 - y, y0 + x, color);
//       drawPixel(x0 - x, y0 + y, color);
//     }
//     if (corners & 0x1) // upper left
//     {
//       drawPixel(x0 - y, y0 - x, color);
//       drawPixel(x0 - x, y0 - y, color);
//     }
//   }
// }

// void DotMGBase::fillCircle(int16_t x0, int16_t y0, uint8_t r, Color color)
// {
//   drawFastVLine(x0, y0-r, 2*r+1, color);
//   fillCircleHelper(x0, y0, r, 3, 0, color);
// }

// void fillCircleHelper(int16_t x0, int16_t y0, uint8_t r, uint8_t sides, int16_t delta, Color color)
// {
//   int16_t f = 1 - r;
//   int16_t ddF_x = 1;
//   int16_t ddF_y = -2 * r;
//   int16_t x = 0;
//   int16_t y = r;

//   while (x < y)
//   {
//     if (f >= 0)
//     {
//       y--;
//       ddF_y += 2;
//       f += ddF_y;
//     }

//     x++;
//     ddF_x += 2;
//     f += ddF_x;

//     if (sides & 0x1) // right side
//     {
//       drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
//       drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
//     }

//     if (sides & 0x2) // left side
//     {
//       drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
//       drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
//     }
//   }
// }

// void DotMGBase::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color color)
// {
//   // bresenham's algorithm - thx wikpedia
//   bool steep = abs(y1 - y0) > abs(x1 - x0);
//   if (steep) {
//     swap(x0, y0);
//     swap(x1, y1);
//   }

//   if (x0 > x1) {
//     swap(x0, x1);
//     swap(y0, y1);
//   }

//   int16_t dx, dy;
//   dx = x1 - x0;
//   dy = abs(y1 - y0);

//   int16_t err = dx / 2;
//   int8_t ystep;

//   if (y0 < y1)
//   {
//     ystep = 1;
//   }
//   else
//   {
//     ystep = -1;
//   }

//   for (; x0 <= x1; x0++)
//   {
//     if (steep)
//     {
//       drawPixel(y0, x0, color);
//     }
//     else
//     {
//       drawPixel(x0, y0, color);
//     }

//     err -= dy;
//     if (err < 0)
//     {
//       y0 += ystep;
//       err += dx;
//     }
//   }
// }

// void DotMGBase::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, Color color)
// {
//   drawFastHLine(x, y, w, color);
//   drawFastHLine(x, y+h-1, w, color);
//   drawFastVLine(x, y, h, color);
//   drawFastVLine(x+w-1, y, h, color);
// }

// void DotMGBase::drawFastVLine(int16_t x, int16_t y, uint8_t h, Color color)
// {
//   int end = y+h;
//   for (int a = max(0,y); a < min(end,HEIGHT); a++)
//   {
//     drawPixel(x,a,color);
//   }
// }

// void DotMGBase::drawFastHLine(int16_t x, int16_t y, uint8_t w, Color color)
// {
//   // TODO
//   // int16_t xEnd; // last x point + 1

//   // // Do y bounds checks
//   // if (y < 0 || y >= HEIGHT)
//   //   return;

//   // xEnd = x + w;

//   // // Check if the entire line is not on the display
//   // if (xEnd <= 0 || x >= WIDTH)
//   //   return;

//   // // Don't start before the left edge
//   // if (x < 0)
//   //   x = 0;

//   // // Don't end past the right edge
//   // if (xEnd > WIDTH)
//   //   xEnd = WIDTH;

//   // // calculate actual width (even if unchanged)
//   // w = xEnd - x;

//   // // buffer pointer plus row offset + x offset
//   // register uint8_t *pBuf = sBuffer + ((y / 8) * WIDTH) + x;

//   // // pixel mask
//   // register uint8_t mask = 1 << (y & 7);

//   // switch (color)
//   // {
//   //   case WHITE:
//   //     while (w--)
//   //     {
//   //       *pBuf++ |= mask;
//   //     }
//   //     break;

//   //   case BLACK:
//   //     mask = ~mask;
//   //     while (w--)
//   //     {
//   //       *pBuf++ &= mask;
//   //     }
//   //     break;
//   // }
// }

// void DotMGBase::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, Color color)
// {
//   // stupidest version - update in subclasses if desired!
//   for (int16_t i=x; i<x+w; i++)
//   {
//     drawFastVLine(i, y, h, color);
//   }
// }

// void DotMGBase::fillScreen(Color color)
// {
//   // TODO
//   // if (color != BLACK)
//   // {
//   //   color = 0xFF; // all pixels on
//   // }
//   // memset(sBuffer, color, WIDTH * HEIGHT / 8);
// }

// void DotMGBase::drawRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, Color color)
// {
//   // smarter version
//   drawFastHLine(x+r, y, w-2*r, color); // Top
//   drawFastHLine(x+r, y+h-1, w-2*r, color); // Bottom
//   drawFastVLine(x, y+r, h-2*r, color); // Left
//   drawFastVLine(x+w-1, y+r, h-2*r, color); // Right
//   // draw four corners
//   drawCircleHelper(x+r, y+r, r, 1, color);
//   drawCircleHelper(x+w-r-1, y+r, r, 2, color);
//   drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
//   drawCircleHelper(x+r, y+h-r-1, r, 8, color);
// }

// void DotMGBase::fillRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, Color color)
// {
//   // smarter version
//   fillRect(x+r, y, w-2*r, h, color);

//   // draw four corners
//   fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
//   fillCircleHelper(x+r, y+r, r, 2, h-2*r-1, color);
// }

// void DotMGBase::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, Color color)
// {
//   drawLine(x0, y0, x1, y1, color);
//   drawLine(x1, y1, x2, y2, color);
//   drawLine(x2, y2, x0, y0, color);
// }

// void DotMGBase::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, Color color)
// {

//   int16_t a, b, y, last;
//   // Sort coordinates by Y order (y2 >= y1 >= y0)
//   if (y0 > y1)
//   {
//     swap(y0, y1); swap(x0, x1);
//   }
//   if (y1 > y2)
//   {
//     swap(y2, y1); swap(x2, x1);
//   }
//   if (y0 > y1)
//   {
//     swap(y0, y1); swap(x0, x1);
//   }

//   if(y0 == y2)
//   { // Handle awkward all-on-same-line case as its own thing
//     a = b = x0;
//     if(x1 < a)
//     {
//       a = x1;
//     }
//     else if(x1 > b)
//     {
//       b = x1;
//     }
//     if(x2 < a)
//     {
//       a = x2;
//     }
//     else if(x2 > b)
//     {
//       b = x2;
//     }
//     drawFastHLine(a, y0, b-a+1, color);
//     return;
//   }

//   int16_t dx01 = x1 - x0,
//       dy01 = y1 - y0,
//       dx02 = x2 - x0,
//       dy02 = y2 - y0,
//       dx12 = x2 - x1,
//       dy12 = y2 - y1,
//       sa = 0,
//       sb = 0;

//   // For upper part of triangle, find scanline crossings for segments
//   // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
//   // is included here (and second loop will be skipped, avoiding a /0
//   // error there), otherwise scanline y1 is skipped here and handled
//   // in the second loop...which also avoids a /0 error here if y0=y1
//   // (flat-topped triangle).
//   if (y1 == y2)
//   {
//     last = y1;   // Include y1 scanline
//   }
//   else
//   {
//     last = y1-1; // Skip it
//   }


//   for(y = y0; y <= last; y++)
//   {
//     a   = x0 + sa / dy01;
//     b   = x0 + sb / dy02;
//     sa += dx01;
//     sb += dx02;

//     if(a > b)
//     {
//       swap(a,b);
//     }

//     drawFastHLine(a, y, b-a+1, color);
//   }

//   // For lower part of triangle, find scanline crossings for segments
//   // 0-2 and 1-2.  This loop is skipped if y1=y2.
//   sa = dx12 * (y - y1);
//   sb = dx02 * (y - y0);

//   for(; y <= y2; y++)
//   {
//     a   = x1 + sa / dy12;
//     b   = x0 + sb / dy02;
//     sa += dx12;
//     sb += dx02;

//     if(a > b)
//     {
//       swap(a,b);
//     }

//     drawFastHLine(a, y, b-a+1, color);
//   }
// }

// void DotMGBase::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, Color color)
// {
//   // no need to draw at all of we're offscreen
//   if (x+w < 0 || x > WIDTH-1 || y+h < 0 || y > HEIGHT-1)
//     return;

//   int16_t xi, yi, byteWidth = (w + 7) / 8;
//   for(yi = 0; yi < h; yi++) {
//     for(xi = 0; xi < w; xi++ ) {
//       if(pgm_read_byte(bitmap + yi * byteWidth + xi / 8) & (128 >> (xi & 7))) {
//         drawPixel(x + xi, y + yi, color);
//       }
//     }
//   }
// }

uint8_t* DotMGBase::frameBuffer()
{
  return frameBuf;
}


/* Random */

unsigned long DotMGBase::generateRandomSeed()
{
  return micros();
}

void DotMGBase::initRandomSeed()
{
  randomSeed(generateRandomSeed());
}


/* Frame management */

uint8_t DotMGBase::frameCount()
{
  return currFrame;
}

void DotMGBase::setFrameRate(uint8_t rate)
{
  eachFrameMillis = 1000 / rate;
}

void DotMGBase::setFrameDuration(uint8_t duration)
{
  eachFrameMillis = duration;
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
    return false;
  }

  // pre-render
  justRendered = true;
  thisFrameStart = now;
  currFrame++;

  return true;
}

bool DotMGBase::everyXFrames(uint8_t frames)
{
  return currFrame % frames == 0;
}

int DotMGBase::cpuLoad()
{
  return lastFrameDurationMs*100 / eachFrameMillis;
}


/* Buttons */

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

void DotMGBase::waitNoButtons() {
  do {
    delay(50); // simple button debounce
  } while (buttonsState());
}


/* Collision */

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

void swap(int16_t &a, int16_t &b)
{
  int16_t temp = a;
  a = b;
  b = temp;
}


//====================================
//========== class DotMG ==========
//====================================

  static int16_t cursor_x;
  static int16_t cursor_y;
  static Color textColor = COLOR_WHITE;
  static Color textBackground = COLOR_CLEAR;
  static uint8_t textSize = 1;
  static bool textWrap;


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

void DotMG::drawChar(int16_t x, int16_t y, unsigned char c, Color color, Color bg, uint8_t size)
{
  uint8_t line;
  bool draw_background = bg != color;  // TODO
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

void DotMG::setTextColor(Color color)
{
  textColor = color;
}

Color DotMG::getTextColor()
{
  return textColor;
}

void DotMG::setTextBackground(Color color)
{
  textBackground = color;
}

Color DotMG::getTextBackground()
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
