#include <DotMG.h>


DotMG dmg;

uint8_t x = WIDTH/2 - 6*3, y = HEIGHT/2 - 4;
Color color = COLOR_WHITE;
Color bg = COLOR_BLACK;

void setup() {
  dmg.setFrameRate(60);
  dmg.begin();
  dmg.setTextSize(2);
}

void loop() {
  if (!dmg.nextFrame())
    return;

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

  dmg.setCursor(x, y);
  dmg.setTextColor(bg);
  dmg.setTextBackground(color);
  dmg.print(F("Hello!"));

  dmg.drawPixel(x, y, color);
  dmg.drawPixel(x-1, y, color);
  dmg.drawPixel(x+1, y, color);
  dmg.drawPixel(x, y-1, color);
  dmg.drawPixel(x, y+1, color);
  dmg.drawPixel(x+1, y-1, bg);
  dmg.drawPixel(x+2, y-1, bg);
  dmg.drawPixel(x+3, y-1, bg);

  dmg.display(CLEAR_BUFFER);
}
