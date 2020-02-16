#include <DotMG.h>


DotMG dmg;

uint8_t x = WIDTH/2 - 6*3, y = HEIGHT/2 - 4;
uint8_t color = COLOR_A;

void setup() {
  dmg.begin();
}

void loop() {
  dmg.clear();

  if (dmg.pressed(UP_BUTTON) && y > 0)
  {
    y -= 1;
  }

  if (dmg.pressed(DOWN_BUTTON) && y < HEIGHT-8)
  {
    y += 1;
  }

  if (dmg.pressed(LEFT_BUTTON) && x > 0)
  {
    x -= 1;
  }

  if (dmg.pressed(RIGHT_BUTTON) && x < WIDTH-6*6)
  {
    x += 1;
  }

  if (dmg.pressed(A_BUTTON))
  {
    color += 1;
    if (color > COLOR_C)
      color = COLOR_A;
  }

  if (dmg.pressed(B_BUTTON))
  {
    color -= 1;
    if (color < COLOR_A)
      color = COLOR_C;
  }

  // dmg.setCursor(x, y);
  // dmg.setTextColor(color);
  // dmg.print(F("Hello!"));

  dmg.drawPixel(x, y, color);

  dmg.display();
}
