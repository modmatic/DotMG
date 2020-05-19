#include <DotMG.h>
#include "space.h"
#include "ship.h"

DotMG dmg;

float x = WIDTH/2, y = HEIGHT/2;
float acc = 200, accX, accY, speedX, speedY;
int mode = 0;
bool alpha = true, invert;

void setup() {
  dmg.setBackgroundColor(COLOR_RED);
  dmg.setBackgroundImage(space, spaceWidth, spaceHeight);
  dmg.setFrameRate(60);
  dmg.begin();
  dmg.setTextSize(1);
}

Color blend_invert(Color a, Color b)
{
  return b.complement();
}

void loop() {
  if (!dmg.nextFrame())
    return;

  float dt = dmg.actualFrameDurationMs() / 1000.0;

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
    alpha = !alpha;
  }

  if (dmg.justPressed(START_BUTTON))
  {
    invert = !invert;
  }

  if (dmg.pressed(A_BUTTON))
  {
    dmg.fillCircle(x, y - shipHeight, shipWidth/2, Color(0xF, 0xF, 0xA, 0x8), invert ? blend_invert : BLEND_ALPHA);
  }

  dmg.drawBitmap(x - shipWidth/2, y - shipWidth/2, ship, shipWidth, shipHeight, alpha ? BLEND_ALPHA : BLEND_NONE);

  // Debug info
  dmg.setCursor(0, 0);
  dmg.setTextColor(COLOR_RED);
  dmg.print(F("fps:  "));
  dmg.println(dmg.actualFrameRate());
  dmg.print(F("spf:  "));
  dmg.println(dt);
  dmg.print(F("load: "));
  dmg.print(dmg.cpuLoad());
  dmg.println(F("%"));
  dmg.print(F("vx: "));
  dmg.println(speedX);
  dmg.print(F("vy: "));
  dmg.println(speedY);
  dmg.print(F("acc: "));
  dmg.println(acc);

  dmg.display(true);
}
