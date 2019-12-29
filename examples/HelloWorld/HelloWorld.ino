#include <DotMG.h>

// Create DotMG instance
DotMG dmg;

void setup() {
  // Initialize dotMG
  dmg.setColorValue(COLOR_D, ST77XX_GREEN);
  dmg.begin();
}

void loop() {
  // first we clear our screen to the background color
  // dmg.clear();
  // dmg.drawPixel()

  // // we set our cursor 5 pixels to the right and 10 down from the top
  // // (positions start at 0, 0)
  // dmg.setCursor(4, 9);

  // // then we print to screen what is in the quotation marks ""
  // dmg.print(F("Hello, world!"));

  // then we finaly we tell the dotMG to show what we just wrote to the display buffer
  dmg.display();
}
