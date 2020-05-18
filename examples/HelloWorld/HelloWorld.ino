#include <DotMG.h>
#include "space.h"

// const Color c = Color(0xF, 0x0, 0x0, 0x8);
// const Color bg[] = {
//   COLOR_GRAY, COLOR_GRAY, COLOR_BLACK, COLOR_BLACK, c, c,
//   COLOR_GRAY, COLOR_GRAY, COLOR_BLACK, COLOR_BLACK, c, c,
//   c, c, COLOR_GRAY, COLOR_GRAY, COLOR_BLACK, COLOR_BLACK,
//   c, c, COLOR_GRAY, COLOR_GRAY, COLOR_BLACK, COLOR_BLACK,
//   COLOR_BLACK, COLOR_BLACK, c, c, COLOR_GRAY, COLOR_GRAY,
//   COLOR_BLACK, COLOR_BLACK, c, c, COLOR_GRAY, COLOR_GRAY,
// };

DotMG dmg;

int r = 20;
// int x = WIDTH/2, y = HEIGHT/2;
float x = WIDTH/2, y = HEIGHT/2;
Color color = Color(0x0, 0xa, 0xf, 0xA);
bool filled = true;

void setup() {
  // while (!Serial);
  dmg.setBackgroundColor(COLOR_MAGENTA);
  dmg.setBackgroundImage(space, spaceWidth, spaceHeight);
  dmg.setFrameRate(60);
  dmg.begin();

  dmg.setTextSize(1);
  // dmg.setTextBackground(COLOR_WHITE);
}

void loop() {
  if (!dmg.nextFrame())
    return;

  float dt = dmg.actualFrameDurationMs() / 1000.0;

  dmg.pollButtons();

  if (dmg.pressed(UP_BUTTON) && y >= 0)
  {
    y -= 160 * dt; //r * dt;
  }

  if (dmg.pressed(DOWN_BUTTON) && y < HEIGHT)
  {
    y += 160 * dt; //r * dt;
  }

  if (dmg.pressed(LEFT_BUTTON) && x >= 0)
  {
    x -= 160 * dt; //r * dt;
  }

  if (dmg.pressed(RIGHT_BUTTON) && x < WIDTH)
  {
    x += 160 * dt; // r * dt;
  }

  if (dmg.pressed(A_BUTTON) && r < 255)
  {
    r += 2;
  }

  if (dmg.pressed(B_BUTTON) && r > 0)
  {
    r -= 2;
  }

  if (dmg.justPressed(START_BUTTON))
  {
    filled = !filled;
  }

  if (dmg.justPressed(SELECT_BUTTON))
  {
    color.a((color.a() == 0xF) ? 0xA : 0xF);
  }

  // dmg.fillScreen(COLOR_GREEN);

  dmg.fillCircle(60, 60, 10, COLOR_RED);

  if (filled)
  {
    dmg.fillCircle(x, y, r, color);
  }
  else
  {
    dmg.drawCircle(x, y, r, color);
  }

  dmg.fillRoundRect(80, 10, 20, 40, 7, Color(0xF, 0xF, 0x0, 0x8));
  dmg.drawRoundRect(120, 10, 20, 40, 7, Color(0xF, 0x0, 0xF, 0x8));

  // dmg.drawTriangle(10, 100, 50, 90, 30, 120, color);

  // dmg.fillCircle(x, y, 10, COLOR_GREEN);
  // dmg.fillRect(x, y, 10, 10, COLOR_BLACK);

  // Debug info
  dmg.setCursor(0, 0);
  dmg.setTextColor(COLOR_RED);
  dmg.print(F("fps:  "));
  dmg.println(dmg.actualFrameRate());
  dmg.print(F("s:  "));
  dmg.println(dt);
  dmg.print(F("load: "));
  dmg.print(dmg.cpuLoad());
  dmg.println(F("%"));
  dmg.print(F("r: "));
  dmg.print(r);

  dmg.display(true);
}
