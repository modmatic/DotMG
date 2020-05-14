#include <DotMG.h>


DotMG dmg;

int x = WIDTH/2, y = HEIGHT/2;
int r = 10;
Color color = Color(0x0, 0xa, 0xf, 0x8);
bool filled;

void setup() {
  // while (!Serial);
  dmg.setFrameRate(60);
  dmg.begin();
  dmg.setTextSize(1);
}

void loop() {
  if (!dmg.nextFrame())
    return;

  dmg.pollButtons();

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

  if (dmg.pressed(A_BUTTON) && r < 255)
  {
    r++;
  }

  if (dmg.pressed(B_BUTTON) && r > 0)
  {
    r--;
  }

  if ((dmg.justPressed(START_BUTTON) || dmg.justPressed(SELECT_BUTTON)))
  {
    filled = !filled;
  }

  // dmg.fillScreen(COLOR_GRAY);
  // dmg.fillScreen(COLOR_WHITE);
  // dmg.fillScreen(COLOR_BLACK);


  dmg.fillCircle(60, 60, 10, COLOR_RED);
  if (filled)
  {
    dmg.fillCircle(x, y, r, color);
  }
  else
  {
    dmg.drawCircle(x, y, r, color);
  }
  dmg.fillRoundRect(80, 10, 20, 40, 7, color);
  dmg.drawRoundRect(120, 10, 20, 40, 7, color);

  // dmg.fillCircle(x, y, 10, COLOR_GREEN);
  // dmg.fillRect(x, y, 10, 10, COLOR_BLACK);

  // Debug info
  dmg.setCursor(0, 0);
  dmg.setTextColor(COLOR_RED);
  dmg.print(F("fps:  "));
  dmg.println(dmg.actualFrameRate());
  dmg.print(F("ms:  "));
  dmg.println(dmg.actualFrameDurationMs());
  dmg.print(F("load: "));
  dmg.print(dmg.cpuLoad());
  dmg.println(F("%"));
  dmg.print(F("r: "));
  dmg.print(r);

  dmg.display();
}
