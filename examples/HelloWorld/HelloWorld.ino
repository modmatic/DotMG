#include <DotMG.h>


DotMG dmg;

int speed = 2;
int x = WIDTH/2, y = HEIGHT/2;
// float x = WIDTH/2, y = HEIGHT/2;
Color color = Color(0x0, 0xa, 0xf, 0x8);
bool filled;

void setup() {
  // while (!Serial);
  dmg.setBackgroundColor(COLOR_CYAN);
  dmg.setFrameRate(60);
  dmg.begin();
  // dmg.blank();
  // while(1);
  dmg.setTextSize(1);
}

void loop() {
  if (!dmg.nextFrame())
    return;

  // float dt = dmg.actualFrameDurationMs() / 1000.0;

  dmg.pollButtons();

  if (dmg.pressed(UP_BUTTON) && y >= 0)
  {
    y -= 1; //speed * dt;
  }

  if (dmg.pressed(DOWN_BUTTON) && y < HEIGHT)
  {
    y += 1; //speed * dt;
  }

  if (dmg.pressed(LEFT_BUTTON) && x >= 0)
  {
    x -= 1; //speed * dt;
  }

  if (dmg.pressed(RIGHT_BUTTON) && x < WIDTH)
  {
    x += 1; //speed * dt;
  }

  if (dmg.pressed(A_BUTTON) && speed < 255)
  {
    speed += 2;
  }

  if (dmg.pressed(B_BUTTON) && speed > 0)
  {
    speed -= 2;
  }

  if ((dmg.justPressed(START_BUTTON) || dmg.justPressed(SELECT_BUTTON)))
  {
    filled = !filled;
  }

  // dmg.fillScreen(COLOR_GREEN);
  // dmg.fillScreen(COLOR_WHITE);
  // dmg.fillScreen(COLOR_BLACK);


  // dmg.fillCircle(60, 60, 10, COLOR_RED);
  if (filled)
  {
    dmg.fillCircle(x, y, speed/2, color);
  }
  else
  {
    dmg.drawCircle(x, y, speed/2, color);
  }
  // dmg.fillRoundRect(80, 10, 20, 40, 7, color);
  // dmg.drawRoundRect(120, 10, 20, 40, 7, color);

  // dmg.drawTriangle(10, 100, 50, 90, 30, 120, color);

  // dmg.fillCircle(x, y, 10, COLOR_GREEN);
  // dmg.fillRect(x, y, 10, 10, COLOR_BLACK);

  // Debug info
  dmg.setCursor(0, 0);
  dmg.setTextColor(COLOR_RED);
  dmg.print(F("fps:  "));
  dmg.println(dmg.actualFrameRate());
  // dmg.print(F("s:  "));
  // dmg.println(dt);
  dmg.print(F("load: "));
  dmg.print(dmg.cpuLoad());
  dmg.println(F("%"));
  dmg.print(F("v: "));
  dmg.print(speed);

  dmg.display();
}
