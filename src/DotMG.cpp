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

Rect::Rect(int16_t x, int16_t y, uint16_t width, uint16_t height)
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

static Color frameBuf[WIDTH*HEIGHT];
static bool overlapAvoid;
static bool *drawnPixels;
static uint16_t currFrame;
static uint16_t eachFrameMillis = 16;
static uint16_t thisFrameStart;
static bool justRendered;
static uint16_t lastFrameDurationMs;

static Color bgColor = COLOR_BLACK;
static Color *bgImage;
static uint16_t bgImageWidth;
static uint16_t bgImageHeight;
static BlendFunc bgImageBlend;

static int16_t cursor_x;
static int16_t cursor_y;

// Draw one or more "corners" of a circle.
static void drawCircleHelper(int16_t x0, int16_t y0, uint16_t r, uint8_t corners, Color color = COLOR_WHITE, BlendFunc blend = BLEND_ALPHA);

// Draw one or both vertical halves of a filled-in circle or rounded rectangle edge.
static void fillCircleHelper(int16_t x0, int16_t y0, uint16_t r, uint8_t sides, int16_t delta, Color color = COLOR_WHITE, BlendFunc blend = BLEND_ALPHA);

static void swap(int16_t &a, int16_t &b);
static Color blendBg(uint16_t x, uint16_t y) __attribute__((always_inline));

void DotMGBase::begin()
{
  boot();
  waitNoButtons();
}

/* Graphics */

Color blendBg(uint16_t x, uint16_t y)
{
  if (bgImage == NULL)
    return bgColor;

  uint16_t imgX = x % bgImageWidth;
  uint16_t imgY = y % bgImageHeight;

  return bgImageBlend(bgImage[imgY*bgImageWidth + imgX], bgColor);
}

void DotMGBase::clear()
{
  cursor_x = 0;
  cursor_y = 0;

  for (int y = 0, yw = 0; y < HEIGHT; y++, yw += WIDTH)
  {
    for (int x = 0; x < WIDTH; x++)
    {
      frameBuf[yw + x] = blendBg(x, y);
    }
  }
}

void DotMGBase::display(bool clear)
{
  if (clear)
  {
    cursor_x = 0;
    cursor_y = 0;
  }

  // Translate image to display stage
  for (int y = 0, yw = 0; y < HEIGHT; y++, yw += WIDTH)
  {
    for (int x = 0; x < WIDTH; x++)
    {
      int i_src = yw + x;

#ifdef DOTMG_PIXEL_SIZE_2X
      int i_dst = 3*i_src + 3*yw;  // 2*3*yw + 3*x
      uint16_t c = frameBuf[i_src] >> 4;  // Ignore alpha

      if (clear)
        frameBuf[i_src] = blendBg(x, y);

      // Add two adjacent 12-bit pixels of the same color
      stage[i_dst+0] = c >> 4;  // R, G channels
      stage[i_dst+1] = ((c & 0xF) << 4) | (c >> 8);  // B channel | R channel
      stage[i_dst+2] = c & 0xFF;  // G, B channels

      // Add same two pixels on next row
      stage[i_dst+0 + 3*WIDTH] = stage[i_dst+0];
      stage[i_dst+1 + 3*WIDTH] = stage[i_dst+1];
      stage[i_dst+2 + 3*WIDTH] = stage[i_dst+2];
#else
      int i_dst = (i_src * 3) >> 1;
      uint16_t c = frameBuf[i_src] >> 4;  // Ignore alpha

      if (clear)
        frameBuf[i_src] = blendBg(x, y);

      if (x & 0x1) // x odd
      {
        stage[i_dst+0] = (stage[i_dst] & 0xF0) | (c >> 8);  // R channel
        stage[i_dst+1] = c & 0xFF;  // G, B channels
      }
      else  // x even
      {
        stage[i_dst+0] = c >> 4;  // R, G channels
        stage[i_dst+1] = ((c & 0xF) << 4) | (stage[i_dst+1] & 0xF);  // B channel
      }
#endif
    }
  }

  blit();
}

void DotMGBase::setBackgroundColor(Color color)
{
  bgColor = color;
}

Color DotMGBase::backgroundColor()
{
  return bgColor;
}

void DotMGBase::setBackgroundImage(const Color image[], uint16_t width, uint16_t height, BlendFunc blend)
{
  bgImage = (Color *)image;

  if (bgImage != NULL)
  {
    bgImageWidth = width;
    bgImageHeight = height;
    bgImageBlend = blend;
  }
  else
  {
    bgImageWidth = 0;
    bgImageHeight = 0;
    bgImageBlend = BLEND_ALPHA;
  }
}

Color* DotMGBase::backgroundImage()
{
  return bgImage;
}

uint16_t DotMGBase::backgroundImageWidth()
{
  return bgImageWidth;
}

uint16_t DotMGBase::backgroundImageHeight()
{
  return bgImageHeight;
}

BlendFunc DotMGBase::backgroundImageBlendFunc()
{
  return bgImageBlend;
}

void DotMGBase::drawPixel(int16_t x, int16_t y, Color color, BlendFunc blend)
{
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
    return;

  int i = y*WIDTH + x;

  if (overlapAvoid)
  {
    if (!drawnPixels[i])
    {
      frameBuf[i] = blend(color, getPixel(x, y));
      drawnPixels[i] = true;
    }
    return;
  }

  frameBuf[i] = blend(color, getPixel(x, y));
}

Color DotMGBase::getPixel(int16_t x, int16_t y)
{
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
    return COLOR_CLEAR;

  return frameBuf[y*WIDTH + x];
}

void DotMGBase::drawCircle(int16_t x0, int16_t y0, uint16_t r, Color color, BlendFunc blend)
{
  if (r == 0)
  {
    drawPixel(x0, y0, color, blend);
    return;
  }

  drawPixel(x0, y0+r, color, blend);
  drawPixel(x0, y0-r, color, blend);
  drawPixel(x0+r, y0, color, blend);
  drawPixel(x0-r, y0, color, blend);

  drawCircleHelper(x0, y0, r, 0xF, color, blend);
}

void drawCircleHelper(int16_t x0, int16_t y0, uint16_t r, uint8_t corners, Color color, BlendFunc blend)
{
  int16_t f = -r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x < y-1)
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
      DotMGBase::drawPixel(x0 + x, y0 + y, color, blend);

      if (x != y)
        DotMGBase::drawPixel(x0 + y, y0 + x, color, blend);
    }

    if (corners & 0x2) // upper right
    {
      DotMGBase::drawPixel(x0 + x, y0 - y, color, blend);

      if (x != y)
        DotMGBase::drawPixel(x0 + y, y0 - x, color, blend);
    }

    if (corners & 0x8) // lower left
    {
      DotMGBase::drawPixel(x0 - y, y0 + x, color, blend);

      if (x != y)
        DotMGBase::drawPixel(x0 - x, y0 + y, color, blend);
    }

    if (corners & 0x1) // upper left
    {
      DotMGBase::drawPixel(x0 - y, y0 - x, color, blend);

      if (x != y)
        DotMGBase::drawPixel(x0 - x, y0 - y, color, blend);
    }
  }
}

void DotMGBase::fillCircle(int16_t x0, int16_t y0, uint16_t r, Color color, BlendFunc blend)
{
  if (r == 0)
  {
    drawPixel(x0, y0, color, blend);
    return;
  }

  drawFastVLine(x0, y0-r, 2*r+1, color, blend);

  if (r == 1)
  {
    drawPixel(x0-1, y0, color, blend);
    drawPixel(x0+1, y0, color, blend);
    return;
  }

  fillCircleHelper(x0, y0, r, 3, 0, color, blend);
}

void fillCircleHelper(int16_t x0, int16_t y0, uint16_t r, uint8_t sides, int16_t delta, Color color, BlendFunc blend)
{
  int16_t f = -r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x <= y)
  {
    uint16_t lastX = x;
    uint16_t lastY = y;

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
      if (lastX != 0)
        DotMGBase::drawFastVLine(x0+lastX, y0-lastY, 2*lastY+1+delta, color, blend);

      if (lastX != lastY && y != lastY)
        DotMGBase::drawFastVLine(x0+lastY, y0-lastX, 2*lastX+1+delta, color, blend);
    }

    if (sides & 0x2) // left side
    {
      if (lastX != 0)
        DotMGBase::drawFastVLine(x0-lastX, y0-lastY, 2*lastY+1+delta, color, blend);

      if (lastX != lastY && y != lastY)
        DotMGBase::drawFastVLine(x0-lastY, y0-lastX, 2*lastX+1+delta, color, blend);
    }
  }
}

void DotMGBase::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color color, BlendFunc blend)
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
      drawPixel(y0, x0, color, blend);
    }
    else
    {
      drawPixel(x0, y0, color, blend);
    }

    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}

void DotMGBase::drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, Color color, BlendFunc blend)
{
  drawFastHLine(x, y, w, color, blend);
  drawFastHLine(x, y+h-1, w, color, blend);
  drawFastVLine(x, y, h, color, blend);
  drawFastVLine(x+w-1, y, h, color, blend);
}

void DotMGBase::drawFastVLine(int16_t x, int16_t y, uint16_t h, Color color, BlendFunc blend)
{
  int end = y+h;
  for (int a = max(0, y); a < min(end, HEIGHT); a++)
  {
    drawPixel(x, a, color, blend);
  }
}

void DotMGBase::drawFastHLine(int16_t x, int16_t y, uint16_t w, Color color, BlendFunc blend)
{
  int end = x+w;
  for (int a = max(0, x); a < min(end, WIDTH); a++)
  {
    drawPixel(a, y, color, blend);
  }
}

void DotMGBase::fillRect(int16_t x, int16_t y, uint16_t w, uint16_t h, Color color, BlendFunc blend)
{
  for (int16_t i=x; i<x+w; i++)
  {
    drawFastVLine(i, y, h, color, blend);
  }
}

void DotMGBase::fillScreen(Color color, BlendFunc blend)
{
  fillRect(0, 0, WIDTH, HEIGHT, color, blend);
}

void DotMGBase::drawRoundRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t r, Color color, BlendFunc blend)
{
  // smarter version
  drawFastHLine(x+r, y, w-2*r, color, blend); // Top
  drawFastHLine(x+r, y+h-1, w-2*r, color, blend); // Bottom
  drawFastVLine(x, y+r, h-2*r, color, blend); // Left
  drawFastVLine(x+w-1, y+r, h-2*r, color, blend); // Right
  // draw four corners
  drawCircleHelper(x+r, y+r, r, 1, color, blend);
  drawCircleHelper(x+w-r-1, y+r, r, 2, color, blend);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color, blend);
  drawCircleHelper(x+r, y+h-r-1, r, 8, color, blend);
}

void DotMGBase::fillRoundRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t r, Color color, BlendFunc blend)
{
  // smarter version
  fillRect(x+r, y, w-2*r, h, color, blend);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color, blend);
  fillCircleHelper(x+r, y+r, r, 2, h-2*r-1, color, blend);
}

void DotMGBase::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, Color color, BlendFunc blend)
{
  // Reuse stage buffer to track drawn pixel locations while saving RAM
  // (it always has enough room, since it's at least 1.5x longer than we need)
  drawnPixels = (bool *)stage;
  memset(drawnPixels, 0, WIDTH*HEIGHT);
  overlapAvoid = true;

  drawLine(x0, y0, x1, y1, color, blend);
  drawLine(x1, y1, x2, y2, color, blend);
  drawLine(x2, y2, x0, y0, color, blend);

  overlapAvoid = false;
}

void DotMGBase::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, Color color, BlendFunc blend)
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
    drawFastHLine(a, y0, b-a+1, color, blend);
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

    drawFastHLine(a, y, b-a+1, color, blend);
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

    drawFastHLine(a, y, b-a+1, color, blend);
  }
}

void DotMGBase::drawBitmap(int16_t x, int16_t y, const Color bitmap[], uint16_t w, uint16_t h, BlendFunc blend)
{
  if (x+w < 0 || x >= WIDTH || y+h < 0 || y >= HEIGHT)
    return;

  for (uint16_t yi = 0; yi < h; yi++)
  {
    uint16_t yw = yi*w;
    for (uint16_t xi = 0; xi < w; xi++)
    {
      Color px = bitmap[yw + xi];

      if (px.a())
        drawPixel(x + xi, y + yi, px, blend);
    }
  }
}

Color* DotMGBase::frameBuffer()
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

uint16_t DotMGBase::frameCount()
{
  return currFrame;
}

void DotMGBase::setFrameRate(uint8_t rate)
{
  eachFrameMillis = 1000 / rate;
}

void DotMGBase::setFrameDuration(uint16_t duration)
{
  eachFrameMillis = duration;
}

bool DotMGBase::nextFrame()
{
  uint16_t now = (uint16_t) millis();
  uint16_t frameDurationMs = now - thisFrameStart;

  if (justRendered) {
    lastFrameDurationMs = frameDurationMs;
    justRendered = false;
    return false;
  }
  else if (frameDurationMs < eachFrameMillis)
  {
    return false;
  }

  // pre-render
  justRendered = true;
  thisFrameStart = now;
  currFrame++;

  return true;
}

bool DotMGBase::everyXFrames(uint16_t frames)
{
  return currFrame % frames == 0;
}

int DotMGBase::cpuLoad()
{
  return lastFrameDurationMs*100 / eachFrameMillis;
}

uint8_t DotMGBase::actualFrameRate()
{
  return 1000 / lastFrameDurationMs;
}

uint16_t DotMGBase::actualFrameDurationMs()
{
  return lastFrameDurationMs;
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

  static Color textColor = COLOR_WHITE;
  static Color textBackground = COLOR_CLEAR;
  static BlendFunc textBlendFunc = BLEND_ALPHA;
  static BlendFunc textBgBlendFunc = BLEND_ALPHA;
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

void DotMG::drawChar(int16_t x, int16_t y, unsigned char c, Color color, Color bg, uint8_t size, BlendFunc textBlend, BlendFunc bgBlend)
{
  uint8_t line;
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
    line = *(bitmap++);
    if (i == 5) {
      line = 0x0;
    }

    for (uint8_t j = 0; j < 8; j++)
    {
      Color draw_color = (line & 0x1) ? color : bg;
      BlendFunc blend = (line & 0x1) ? textBlend : bgBlend;

      if (draw_color.a()) {
        for (uint8_t a = 0; a < size; a++ ) {
          for (uint8_t b = 0; b < size; b++ ) {
            drawPixel(x + (i * size) + a, y + (j * size) + b, draw_color, blend);
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

void DotMG::setTextColor(Color color, BlendFunc blend)
{
  textColor = color;
  textBlendFunc = blend;
}

Color DotMG::getTextColor()
{
  return textColor;
}

BlendFunc DotMG::getTextBlendFunc()
{
  return textBlendFunc;
}

void DotMG::setTextBackground(Color color, BlendFunc blend)
{
  textBackground = color;
  textBgBlendFunc = blend;

}

Color DotMG::getTextBackground()
{
  return textBackground;
}

BlendFunc DotMG::getTextBackgroundBlendFunc()
{
  return textBgBlendFunc;
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
