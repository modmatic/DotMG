#include <DotMG.h>


DotMG dmg;

int x = WIDTH/2, y = HEIGHT/2;
Color color = Color(0x0, 0x8, 0xf, 0x8);

void setup() {
  // while (!Serial);
  dmg.setFrameRate(60);
  dmg.begin();
  dmg.setTextSize(1);
}

void loop() {
  if (!dmg.nextFrame())
    return;

  // dmg.clear();

  if (dmg.pressed(UP_BUTTON) && y >= 0)
  {
    y -= 1;
  }

  if (dmg.pressed(DOWN_BUTTON) && y < HEIGHT)
  {
    y += 1;
  }

  if (dmg.pressed(LEFT_BUTTON) && x >= 0)
  {
    x -= 1;
  }

  if (dmg.pressed(RIGHT_BUTTON) && x < WIDTH)
  {
    x += 1;
  }

  // dmg.fillScreen(COLOR_GRAY);
  // dmg.fillScreen(COLOR_WHITE);
  // dmg.fillScreen(COLOR_BLACK);


  dmg.fillCircle(60, 60, 10, COLOR_RED);
  dmg.fillCircle(x, y, 10, color);

  // dmg.fillCircle(x, y, 10, COLOR_GREEN);
  // dmg.fillRect(x, y, 10, 10, COLOR_BLACK);

  // Debug info
  dmg.setCursor(0, 0);
  dmg.setTextColor(COLOR_RED);
  dmg.print(F("fps:  "));
  dmg.println(dmg.actualFrameRate());
  dmg.print(F("load: "));
  dmg.print(dmg.cpuLoad());
  dmg.print(F("%"));

  dmg.display(CLEAR_BUFFER);
}
