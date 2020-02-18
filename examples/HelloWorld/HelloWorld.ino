#include <DotMG.h>


DotMG dmg;

uint8_t x, y;

void setup() {
  dmg.setFrameRate(60);
  dmg.begin();
  dmg.setTextSize(3);
}

void loop() {
  // if (!dmg.nextFrame())
  //   return;

  dmg.clear();

  if (dmg.pressed(UP_BUTTON) && y > 0)
  {
    y -= 1;
  }

  if (dmg.pressed(DOWN_BUTTON) && y < HEIGHT-1)
  {
    y += 1;
  }

  if (dmg.pressed(LEFT_BUTTON) && x > 0)
  {
    x -= 1;
  }

  if (dmg.pressed(RIGHT_BUTTON) && x < WIDTH-1)
  {
    x += 1;
  }

  dmg.fillScreen(COLOR_GRAY);

  dmg.setCursor(x, y);
  dmg.setTextColor(COLOR_RED);
  dmg.setTextBackground(COLOR_BLUE);
  dmg.print(F("Hello!"));

  // dmg.drawPixel(x, y, color);
  // dmg.drawPixel(x-1, y, color);
  // dmg.drawPixel(x+1, y, color);
  // dmg.drawPixel(x, y-1, color);
  // dmg.drawPixel(x, y+1, color);


  // dmg.fillCircle(x, y, 10, COLOR_GREEN);
  // dmg.fillRect(x, y, 10, 10, COLOR_BLACK);

  dmg.display();
}
