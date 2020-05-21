#include <DotMG.h>
#include "space.h"
#include "ship.h"

#define DEBUG 1

DotMG dmg;

double x = WIDTH/2, y = HEIGHT/2;
double acc = 200, accX, accY, speedX, speedY;
int bg = 0;
bool alpha = true, invert;
const Color bgs[] = {
  COLOR_BLACK,
  COLOR_ORANGE,
  COLOR_BLUE,
  COLOR_MAGENTA,
};

void setup() {
  dmg.setBackgroundColor(bgs[bg]);
  dmg.setBackgroundImage(space, spaceWidth, spaceHeight);
  dmg.setFrameRate(60);
  dmg.begin();
  dmg.setTextSize(1);
  dmg.setTextColor(COLOR_CLEAR, BLEND_NONE);
  dmg.setTextBackground(Color(0xF, 0, 0, 0x5), BLEND_NONE);
}

Color blend_invert(Color a, Color b)
{
  Color c = b.complement();
  c.a(a.a());
  return (alpha ? BLEND_ALPHA : BLEND_NONE)(c, b);
}

void loop() {
  if (!dmg.nextFrame())
    return;

  double dt = dmg.actualFrameDurationMs() / 1000.0;

  dmg.pollButtons();

  if (dmg.pressed(LEFT_BUTTON))
  {
    accX = -acc;
  }
  else if (dmg.pressed(RIGHT_BUTTON))
  {
    accX = acc;
  }
  else
  {
    accX = 0;
  }


  if (dmg.pressed(UP_BUTTON))
  {
    accY = -acc;
  }
  else if (dmg.pressed(DOWN_BUTTON))
  {
    accY = acc;
  }
  else
  {
    accY = 0;
  }

  speedX += accX * dt;
  speedY += accY * dt;

  if (x >= 0 && x < WIDTH)
  {
    x += speedX * dt;
  }
  else
  {
    speedX = 0;
    accX = 0;
    if (x < 0) x = 0;
    if (x >= WIDTH) x = WIDTH-1;
  }

  if (y >= 0 && y < HEIGHT)
  {
    y += speedY * dt;
  }
  else
  {
    speedY = 0;
    accY = 0;
    if (y < 0) y = 0;
    if (y >= HEIGHT) y = HEIGHT-1;
  }

  if (dmg.justPressed(SELECT_BUTTON))
  {
    invert = !invert;
  }

  if (dmg.justPressed(START_BUTTON))
  {
    alpha = !alpha;
  }

  if (dmg.pressed(A_BUTTON))
  {
    dmg.fillCircle(x, y - shipHeight, shipWidth/2, Color(0xF, 0xF, 0xA, 0x8));
  }

  if (dmg.justPressed(B_BUTTON))
  {
    bg = (bg + 1) % (sizeof(bgs)/sizeof(Color));
    dmg.setBackgroundColor(bgs[bg]);
  }

  Color c = COLOR_RED;
  c.a(0x3);
  // dmg.drawTriangle(x, y, 10, 125, 120, 125, c);
  dmg.fillTriangle(x, y, 10, 125, 120, 100, c);
  // dmg.drawBitmap(x - shipWidth/2, y - shipWidth/2, ship, shipWidth, shipHeight, invert ? blend_invert : alpha ? BLEND_ALPHA : BLEND_NONE);

  // dmg.setCursor(0, 0);
#if DEBUG
  dmg.print(F("fps:  "));
  dmg.println(dmg.actualFrameRate());
  dmg.print(F("spf:  "));
  dmg.println(dt);
  dmg.print(F("load: "));
  dmg.print(dmg.cpuLoad());
  dmg.println(F("%"));
#endif
  dmg.print(F("inv:   "));
  dmg.println(invert);
  dmg.print(F("alpha: "));
  dmg.println(alpha);

  dmg.display(true);
}
