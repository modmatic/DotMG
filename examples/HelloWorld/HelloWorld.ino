#include <DotMG.h>


DotMG dmg;

uint8_t x = WIDTH/2 - 6*3, y = HEIGHT/2 - 4;
Color color = COLOR_WHITE;

void setup() {
  dmg.begin();
}

void loop() {
  // dmg.clear();

  if (dmg.pressed(UP_BUTTON) && y > 0)
  {
    y -= 1;
  }

  if (dmg.pressed(DOWN_BUTTON) && y < HEIGHT)
  {
    y += 1;
  }

  if (dmg.pressed(LEFT_BUTTON) && x > 0)
  {
    x -= 1;
  }

  if (dmg.pressed(RIGHT_BUTTON) && x < WIDTH)
  {
    x += 1;
  }

  dmg.setCursor(x, y+5);
  dmg.setTextColor(color);
  dmg.print(F("Hello!"));

  dmg.drawPixel(x, y, color);
  dmg.drawPixel(x-1, y, color);
  dmg.drawPixel(x+1, y, color);
  dmg.drawPixel(x, y-1, color);
  dmg.drawPixel(x, y+1, color);

  dmg.display(CLEAR_BUFFER);
}
