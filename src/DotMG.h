/**
 * @file DotMG.h
 * \brief
 * The DotMGBase and DotMG classes and support objects and definitions.
 */

#ifndef DOTMG_H
#define DOTMG_H

#include <Arduino.h>
#include <EEPROM.h>
#include "DotMGCore.h"
#include "Sprites.h"
#include "SpritesB.h"
#include <Print.h>
#include "DotMGAudio.h"

#define CLEAR_BUFFER true /**< Value to be passed to `display()` to clear the screen buffer. */


//=============================================
//========== Rect (rectangle) object ==========
//=============================================

/** \brief
 * A rectangle object for collision functions.
 *
 * \details
 * The X and Y coordinates specify the top left corner of a rectangle with the
 * given width and height.
 *
 * \see DotMGBase::collide(Point, Rect) DotMGBase::collide(Rect, Rect)
 *      Point
 */
struct Rect
{
  int16_t x;      /**< The X coordinate of the top left corner */
  int16_t y;      /**< The Y coordinate of the top left corner */
  uint8_t width;  /**< The width of the rectangle */
  uint8_t height; /**< The height of the rectangle */

  /** \brief
   * The default constructor
   */
  Rect() = default;

  /** \brief
   * The fully initializing constructor
   *
   * \param x The X coordinate of the top left corner. Copied to variable `x`.
   * \param y The Y coordinate of the top left corner. Copied to variable `y`.
   * \param width The width of the rectangle. Copied to variable `width`.
   * \param height The height of the rectangle. Copied to variable `height`.
   */
  Rect(int16_t x, int16_t y, uint8_t width, uint8_t height);
};

//==================================
//========== Point object ==========
//==================================

/** \brief
 * An object to define a single point for collision functions.
 *
 * \details
 * The location of the point is given by X and Y coordinates.
 *
 * \see DotMGBase::collide(Point, Rect) Rect
 */
struct Point
{
  int16_t x; /**< The X coordinate of the point */
  int16_t y; /**< The Y coordinate of the point */

  /** \brief
   * The default constructor
   */
  Point() = default;

  /** \brief
   * The fully initializing constructor
   *
   * \param x The X coordinate of the point. Copied to variable `x`.
   * \param y The Y coordinate of the point. Copied to variable `y`.
   */
  Point(int16_t x, int16_t y);
};

//==================================
//========== DotMGBase ==========
//==================================

/** \brief
 * The main functions provided for writing sketches for the dotMG,
 * _minus_ text output.
 *
 * \details
 * This class in inherited by DotMG, so if text output functions are
 * required DotMG should be used instead.
 *
 * \see DotMG
 */
class DotMGBase : public DotMGCore
{
 public:
  /** \brief
   * Initialize the hardware.
   *
   * \details
   * This function should be called once near the start of the sketch,
   * usually in `setup()`, before using any other functions in this class.
   */
  static void begin();

  /** \brief
   * Clear the display buffer.
   *
   * \details
   * The entire contents of the screen buffer are cleared to `COLOR_BLACK`.
   *
   * \see display(bool)
   */
  static void clear();

  /** \brief
   * Copy the contents of the display buffer to the display.
   *
   * \details
   * The contents of the display buffer in RAM are copied to the display and
   * will appear on the screen.
   *
   * \see display(bool)
   */
  static void display();

  /** \brief
   * Copy the contents of the display buffer to the display. The display buffer
   * can optionally be cleared.
   *
   * \param clear If `true` the display buffer will be cleared to zero.
   * The defined value `CLEAR_BUFFER` should be used instead of `true` to make
   * it more meaningful.
   *
   * \details
   * Operation is the same as calling `display()` without parameters except
   * additionally the display buffer will be cleared if the parameter evaluates
   * to `true`. (The defined value `CLEAR_BUFFER` can be used for this)
   *
   * Using `display(CLEAR_BUFFER)` is faster and produces less code than
   * calling `display()` followed by `clear()`.
   *
   * \see display() clear()
   */
  static void display(bool clear);

  /** \brief
   * Set a single pixel in the display buffer to the specified color.
   *
   * \param x The X coordinate of the pixel.
   * \param y The Y coordinate of the pixel.
   * \param color The color of the pixel (optional; defaults to `COLOR_WHITE`).
   */
  static void drawPixel(int16_t x, int16_t y, Color color = COLOR_WHITE);

  /** \brief
   * Returns the color of the given pixel in the screen buffer.
   *
   * \param x The X coordinate of the pixel.
   * \param y The Y coordinate of the pixel.
   *
   * \details
   * The returned color represents the value that will be sent to the display
   * (if no additional drawing occurs at the pixel's location). In other words,
   * it represents the result of any previous blending that might have occurred
   * during a `draw*()` function.
   */
  Color getPixel(uint8_t x, uint8_t y);

  /** \brief
   * Draw a circle of a given radius.
   *
   * \param x0 The X coordinate of the circle's center.
   * \param y0 The Y coordinate of the circle's center.
   * \param r The radius of the circle in pixels.
   * \param color The circle's color (optional; defaults to `COLOR_WHITE`).
   */
  static void drawCircle(int16_t x0, int16_t y0, uint8_t r, Color color = COLOR_WHITE);

  /** \brief
   * Draw a filled-in circle of a given radius.
   *
   * \param x0 The X coordinate of the circle's center.
   * \param y0 The Y coordinate of the circle's center.
   * \param r The radius of the circle in pixels.
   * \param color The circle's color (optional; defaults to `COLOR_WHITE`).
   */
  static void fillCircle(int16_t x0, int16_t y0, uint8_t r, Color color = COLOR_WHITE);

  /** \brief
   * Draw a line between two specified points.
   *
   * \param x0,x1 The X coordinates of the line ends.
   * \param y0,y1 The Y coordinates of the line ends.
   * \param color The line's color (optional; defaults to `COLOR_WHITE`).
   *
   * \details
   * Draw a line from the start point to the end point using Bresenham's algorithm.
   * The start and end points can be at any location with respect to the other.
   */
  static void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color color = COLOR_WHITE);

  /** \brief
   * Draw a rectangle of a specified width and height.
   *
   * \param x The X coordinate of the upper left corner.
   * \param y The Y coordinate of the upper left corner.
   * \param w The width of the rectangle.
   * \param h The height of the rectangle.
   * \param color The color of the pixel (optional; defaults to `COLOR_WHITE`).
   */
  static void drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, Color color = COLOR_WHITE);

  /** \brief
   * Draw a vertical line.
   *
   * \param x The X coordinate of the upper start point.
   * \param y The Y coordinate of the upper start point.
   * \param h The height of the line.
   * \param color The color of the line (optional; defaults to `COLOR_WHITE`).
   */
  static void drawFastVLine(int16_t x, int16_t y, uint8_t h, Color color = COLOR_WHITE);

  /** \brief
   * Draw a horizontal line.
   *
   * \param x The X coordinate of the left start point.
   * \param y The Y coordinate of the left start point.
   * \param w The width of the line.
   * \param color The color of the line (optional; defaults to `COLOR_WHITE`).
   */
  static void drawFastHLine(int16_t x, int16_t y, uint8_t w, Color color = COLOR_WHITE);

  /** \brief
   * Draw a filled-in rectangle of a specified width and height.
   *
   * \param x The X coordinate of the upper left corner.
   * \param y The Y coordinate of the upper left corner.
   * \param w The width of the rectangle.
   * \param h The height of the rectangle.
   * \param color The color of the pixel (optional; defaults to `COLOR_WHITE`).
   */
  static void fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, Color color = COLOR_WHITE);

  /** \brief
   * Fill the screen buffer with the specified color.
   *
   * \param color The fill color (optional; defaults to `COLOR_WHITE`).
   */
  static void fillScreen(Color color = COLOR_WHITE);

  /** \brief
   * Draw a rectangle with rounded corners.
   *
   * \param x The X coordinate of the left edge.
   * \param y The Y coordinate of the top edge.
   * \param w The width of the rectangle.
   * \param h The height of the rectangle.
   * \param r The radius of the semicircles forming the corners.
   * \param color The color of the rectangle (optional; defaults to `COLOR_WHITE`).
   */
  static void drawRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, Color color = COLOR_WHITE);

  /** \brief
   * Draw a filled-in rectangle with rounded corners.
   *
   * \param x The X coordinate of the left edge.
   * \param y The Y coordinate of the top edge.
   * \param w The width of the rectangle.
   * \param h The height of the rectangle.
   * \param r The radius of the semicircles forming the corners.
   * \param color The color of the rectangle (optional; defaults to `COLOR_WHITE`).
   */
  static void fillRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, Color color = COLOR_WHITE);

  /** \brief
   * Draw a triangle given the coordinates of each corner.
   *
   * \param x0,x1,x2 The X coordinates of the corners.
   * \param y0,y1,y2 The Y coordinates of the corners.
   * \param color The triangle's color (optional; defaults to `COLOR_WHITE`).
   *
   * \details
   * A triangle is drawn by specifying each of the three corner locations.
   * The corners can be at any position with respect to each other.
   */
  static void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, Color color = COLOR_WHITE);

  /** \brief
   * Draw a filled-in triangle given the coordinates of each corner.
   *
   * \param x0,x1,x2 The X coordinates of the corners.
   * \param y0,y1,y2 The Y coordinates of the corners.
   * \param color The triangle's color (optional; defaults to `COLOR_WHITE`).
   *
   * \details
   * A triangle is drawn by specifying each of the three corner locations.
   * The corners can be at any position with respect to each other.
   */
  static void fillTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, Color color = COLOR_WHITE);

  /** \brief
   * Draw a bitmap from a horizontally-oriented array in program memory.
   *
   * \param x The X coordinate of the top left pixel affected by the bitmap.
   * \param y The Y coordinate of the top left pixel affected by the bitmap.
   * \param bitmap A pointer to the bitmap array in program memory.
   * \param w The width of the bitmap in pixels.
   * \param h The height of the bitmap in pixels.
   *
   * \details
   * Pixels are arranged in rows, from left to right.
   *
   * The input array must be located in program memory by declaring it as
   * a `const`.
   */
  static void drawBitmap(int16_t x, int16_t y, const Color *bitmap, uint8_t w, uint8_t h);

  /** \brief
   * Get a pointer to the display buffer in RAM.
   *
   * \return A pointer to the display buffer array in RAM.
   *
   * \details
   * The location of the display buffer in RAM, which is displayed using
   * `display()`, can be gotten using this function. The buffer can then be
   * read and directly manipulated.
   *
   * Every three bytes in the array specifies a horizontal row of two pixels, with the
   * most significant bit at the left end of the row. Pixels are 12-bit 444-formatted
   * RGB color values.
   *
   * The format for two neighboring pixels is as follows, where RX, GX, BX represent
   * the color data for the pixels:
   *
   * Pixel: |------------- Pixel 1 -------------|------------- Pixel 2 -------------|
   *  Data:  R3 R2 R1 R0 G3 G2 G1 G0 B3 B2 B1 B0 R3 R2 R1 R0 G3 G2 G1 G0 B3 B2 B1 B0
   *   Bit:   7  6  5  4  3  2  1  0  7  6  5  4  3  2  1  0  7  6  5  4  3  2  1  0
   *  Byte: |------- Byte 1 --------|------- Byte 2 --------|------- Byte 3 --------|
   */
  uint8_t* frameBuffer();

  /** \brief
   * Create a seed suitable for use with a random number generator.
   *
   * \return A random value that can be used to seed a random number generator.
   *
   * \details
   * The returned value will be the microseconds since boot.
   *
   * This method is most effective when called after a semi-random time, such
   * as after a user hits a button to start a game or other semi-random event.
   *
   * \see initRandomSeed()
   */
  unsigned long generateRandomSeed();

  /** \brief
   * Seed the random number generator with a random value.
   *
   * \details
   * The Arduino random number generator is seeded with the microseconds since boot.
   * The seed value is provided by calling the `generateRandomSeed()` function.
   *
   * This method is most effective when called after a semi-random time, such
   * as after a user hits a button to start a game or other semi-random event.
   *
   * \see generateRandomSeed()
   */
  static void initRandomSeed();

  /** \brief
   * Returns the current frame counter value.
   *
   * \details
   * This counter is incremented once per frame when using the `nextFrame()`
   * function. It will wrap to zero when it reaches its maximum value.
   *
   * It could be used to have an event occur for a given number of frames, or
   * a given number of frames later, in a way that wouldn't be quantized the
   * way that using `everyXFrames()` might.
   *
   * example:
   * \code{.cpp}
   * // move for 10 frames when right button is pressed, if not already moving
   * if (!moving) {
   *   if (dmg.justPressed(RIGHT_BUTTON)) {
   *     endMoving = dmg.frameCount + 10;
   *     moving = true;
   *   }
   * } else {
   *   movePlayer();
   *   if (dmg.frameCount == endMoving) {
   *     moving = false;
   *   }
   * }
   * \endcode
   *
   * This counter could also be used to determine the number of frames that
   * have elapsed between events but the possibility of the counter wrapping
   * would have to be accounted for.
   *
   * \see nextFrame() everyXFrames()
   */
  static unsigned long frameCount() { return frameCount; }

  /** \brief
   * Set the frame rate used by the frame control functions.
   *
   * \param rate The desired frame rate in frames per second.
   *
   * \details
   * Set the frame rate, in frames per second, used by `nextFrame()` to update
   * frames at a given rate. If this function or `setFrameDuration()`
   * isn't used, the default rate will be 60 (actually 62.5, see note below).
   *
   * Normally, the frame rate would be set to the desired value once, at the
   * start of the game, but it can be changed at any time to alter the frame
   * update rate.
   *
   * \note
   * \parblock
   * The given rate is internally converted to a frame duration in milliseconds,
   * rounded down to the nearest integer. Therefore, the actual rate will be
   * equal to or higher than the rate given.

   * For example, 60 FPS would be 16.67ms per frame. This will be rounded down
   * to 16ms, giving an actual frame rate of 62.5 FPS.
   * \endparblock
   *
   * \see nextFrame() setFrameDuration()
   */
  static void setFrameRate(uint8_t rate);

  /** \brief
   * Set the frame rate, used by the frame control functions, by giving
   * the duration of each frame.
   *
   * \param duration The desired duration of each frame in milliseconds.
   *
   * \details
   * Set the frame rate by specifying the duration of each frame in
   * milliseconds. This is used by `nextFrame()` to update frames at a
   * given rate. If this function or `setFrameRate()` isn't used,
   * the default will be 16ms per frame.
   *
   * Normally, the frame rate would be set to the desired value once, at the
   * start of the game, but it can be changed at any time to alter the frame
   * update rate.
   *
   * \see nextFrame() setFrameRate()
   */
  static void setFrameDuration(uint8_t duration);

  /** \brief
   * Indicate that it's time to render the next frame.
   *
   * \return `true` if it's time for the next frame.
   *
   * \details
   * When this function returns `true`, the amount of time has elapsed to
   * display the next frame, as specified by `setFrameRate()`.
   *
   * This function will normally be called at the start of the rendering loop
   * which would wait for `true` to be returned before rendering and
   * displaying the next frame.
   *
   * example:
   * \code{.cpp}
   * void loop() {
   *   if (!dmg.nextFrame()) {
   *     return; // go back to the start of the loop
   *   }
   *   // render and display the next frame
   * }
   * \endcode
   *
   * \see setFrameRate() setFrameDuration()
   */
  static bool nextFrame();

  /** \brief
   * Indicate if the specified number of frames has elapsed.
   *
   * \param frames The desired number of elapsed frames.
   *
   * \return `true` if the specified number of frames has elapsed.
   *
   * \details
   * This function should be called with the same value each time for a given
   * event. It will return `true` if the given number of frames has elapsed
   * since the previous frame in which it returned `true`.
   *
   * For example, if you wanted to fire a shot every 5 frames while the A button
   * is being held down:
   *
   * \code{.cpp}
   * if (dmg.everyXFrames(5)) {
   *   if dmg.pressed(A_BUTTON) {
   *     fireShot();
   *   }
   * }
   * \endcode
   *
   * \see setFrameRate() nextFrame()
   */
  static bool everyXFrames(uint8_t frames);

  /** \brief
   * Return the load on the CPU as a percentage.
   *
   * \return The load on the CPU as a percentage of the total frame time.
   *
   * \details
   * The returned value gives the time spent processing a frame as a percentage
   * the total time allotted for a frame, as determined by the frame rate.
   *
   * This function normally wouldn't be used in the final program. It is
   * intended for use during program development as an aid in helping with
   * frame timing.
   *
   * \note
   * The percentage returned can be higher than 100 if more time is spent
   * processing a frame than the time allotted per frame. This would indicate
   * that the frame rate should be made slower or the frame processing code
   * should be optimized to run faster.
   *
   * \see setFrameRate() nextFrame()
   */
  static int cpuLoad();

  /** \brief
   * Test if the specified buttons are pressed.
   *
   * \param buttons A bit mask indicating which buttons to test.
   * (Can be a single button)
   *
   * \return `true` if *all* buttons in the provided mask are currently pressed.
   *
   * \details
   * Read the state of the buttons and return `true` if all the buttons in the
   * specified mask are being pressed.
   *
   * Example: `if (pressed(LEFT_BUTTON + A_BUTTON))`
   *
   * \note
   * This function does not perform any button debouncing.
   */
  static bool pressed(uint8_t buttons);

  /** \brief
   * Test if the specified buttons are not pressed.
   *
   * \param buttons A bit mask indicating which buttons to test.
   * (Can be a single button)
   *
   * \return `true` if *all* buttons in the provided mask are currently
   * released.
   *
   * \details
   * Read the state of the buttons and return `true` if all the buttons in the
   * specified mask are currently released.
   *
   * Example: `if (notPressed(UP_BUTTON))`
   *
   * \note
   * This function does not perform any button debouncing.
   */
  static bool notPressed(uint8_t buttons);

  /** \brief
   * Poll the buttons and track their state over time.
   *
   * \details
   * Read and save the current state of the buttons and also keep track of the
   * button state when this function was previouly called. These states are
   * used by the `justPressed()` and `justReleased()` functions to determine
   * if a button has changed state between now and the previous call to
   * `pollButtons()`.
   *
   * This function should be called once at the start of each new frame.
   *
   * The `justPressed()` and `justReleased()` functions rely on this function.
   *
   * example:
   * \code{.cpp}
   * void loop() {
   *   if (!dmg.nextFrame()) {
   *     return;
   *   }
   *   dmg.pollButtons();
   *
   *   // use justPressed() as necessary to determine if a button was just pressed
   * \endcode
   *
   * \note
   * As long as the elapsed time between calls to this function is long
   * enough, buttons will be naturally debounced. Calling it once per frame at
   * a frame rate of 60 or lower (or possibly somewhat higher), should be
   * sufficient.
   *
   * \see justPressed() justReleased()
   */
  static void pollButtons();

  /** \brief
   * Check if a button has just been pressed.
   *
   * \param button The button to test for. Only one button should be specified.
   *
   * \return `true` if the specified button has just been pressed.
   *
   * \details
   * Return `true` if the given button was pressed between the latest
   * call to `pollButtons()` and previous call to `pollButtons()`.
   * If the button has been held down over multiple polls, this function will
   * return `false`.
   *
   * There is no need to check for the release of the button since it must have
   * been released for this function to return `true` when pressed again.
   *
   * This function should only be used to test a single button.
   *
   * \see pollButtons() justReleased()
   */
  static bool justPressed(uint8_t button);

  /** \brief
   * Check if a button has just been released.
   *
   * \param button The button to test for. Only one button should be specified.
   *
   * \return `true` if the specified button has just been released.
   *
   * \details
   * Return `true` if the given button, having previously been pressed,
   * was released between the latest call to `pollButtons()` and previous call
   * to `pollButtons()`. If the button has remained released over multiple
   * polls, this function will return `false`.
   *
   * There is no need to check for the button having been pressed since it must
   * have been previously pressed for this function to return `true` upon
   * release.
   *
   * This function should only be used to test a single button.
   *
   * \note
   * There aren't many cases where this function would be needed. Wanting to
   * know if a button has been released, without knowing when it was pressed,
   * is uncommon.
   *
   * \see pollButtons() justPressed()
   */
  static bool justReleased(uint8_t button);

  /** \brief
   * Wait until all buttons have been released.
   *
   * \details
   * This function is called by `begin()`.
   *
   * It won't return unless no buttons are being pressed. A short delay is
   * performed each time before testing the state of the buttons to do a
   * simple button debounce.
   *
   * \see begin()
   */
  static void waitNoButtons();

  /** \brief
   * Test if a point falls within a rectangle.
   *
   * \param point A structure describing the location of the point.
   * \param rect A structure describing the location and size of the rectangle.
   *
   * \return `true` if the specified point is within the specified rectangle.
   *
   * \details
   * This function is intended to detemine if an object, whose boundaries are
   * are defined by the given rectangle, is in contact with the given point.
   *
   * \see Point Rect
   */
  static bool collide(Point point, Rect rect);

  /** \brief
   * Test if a rectangle is intersecting with another rectangle.
   *
   * \param rect1,rect2 Structures describing the size and locations of the
   * rectangles.
   *
   * \return `true` if the first rectangle is intersecting the second.
   *
   * \details
   * This function is intended to detemine if an object, whose boundaries are
   * are defined by the given rectangle, is in contact with another rectangular
   * object.
   *
   * \see Rect
   */
  static bool collide(Rect rect1, Rect rect2);
};


//==============================
//========== DotMG ==========
//==============================

/** \brief
 * The main functions provided for writing sketches for the dotMG,
 * _including_ text output.
 *
 * \details
 * This class is derived from DotMGBase. It provides text output functions
 * in addition to all the functions inherited from DotMGBase.
 */
class DotMG : public Print, public DotMGBase
{
 public:
  /** \class Print
   * \brief
   * The Arduino `Print` class is available for writing text to the screen
   * buffer.
   *
   * \details
   * For an `DotMG` class object, functions provided by the Arduino `Print`
   * class can be used to write text to the screen buffer, in the same manner
   * as the Arduino `Serial.print()`, etc., functions.
   *
   * Print will use the `write()` function to actually draw each character
   * in the screen buffer.
   *
   * See:
   * https://www.arduino.cc/en/Serial/Print
   *
   * Example:
   * \code{.cpp}
   * int value = 42;
   *
   * dmg.println("Hello World"); // Prints "Hello World" and then moves the
   *                                 // text cursor to the start of the next line
   * dmg.print(value);  // Prints "42"
   * dmg.print('\n');   // Moves the text cursor to the start of the next line
   * dmg.print(78, HEX) // Prints "4E" (78 in hexadecimal)
   * \endcode
   *
   * \see DotMG::write()
   */
  using Print::write;

  /** \brief
   * Write a single ASCII character at the current text cursor location.
   *
   * \param c The ASCII value of the character to be written.
   *
   * \return The number of characters written (will always be 1).
   *
   * \details
   * This is the dotMG implemetation of the Arduino virtual `write()`
   * function. The single ASCII character specified is written to the
   * the screen buffer at the current text cursor. The text cursor is then
   * moved to the next character position in the screen buffer. This new cursor
   * position will depend on the current text size and possibly the current
   * wrap mode.
   *
   * Two special characters are handled:
   *
   * - The newline character `\n`. This will move the text cursor to the start
   *   of the next line based on the current text size.
   * - The carriage return character `\r`. This character will be ignored.
   *
   * \note
   * This function is rather low level and, although it's available as a public
   * function, it wouldn't normally be used. In most cases the Arduino Print
   * class should be used for writing text.
   *
   * \see Print setTextSize() setTextWrap()
   */
  virtual size_t write(uint8_t);

  /** \brief
   * Draw a single ASCII character at the specified location in the screen
   * buffer.
   *
   * \param x The X coordinate, in pixels, for where to draw the character.
   * \param y The Y coordinate, in pixels, for where to draw the character.
   * \param c The ASCII value of the character to be drawn.
   * \param color the forground color of the character.
   * \param bg the background color of the character.
   * \param size The size of the character to draw.
   *
   * \details
   * The specified ASCII character is drawn starting at the provided
   * coordinate. The point specified by the X and Y coordinates will be the
   * top left corner of the character.
   *
   * \note
   * This is a low level function used by the `write()` function to draw a
   * character. Although it's available as a public function, it wouldn't
   * normally be used. In most cases the Arduino Print class should be used for
   * writing text.
   *
   * \see Print write() setTextColor() setTextBackground() setTextSize()
   */
  static void drawChar(int16_t x, int16_t y, unsigned char c, Color color, uint16_t bg, uint8_t size);

  /** \brief
   * Set the location of the text cursor.
   *
   * \param x The X coordinate, in pixels, for the new location of the text cursor.
   * \param y The Y coordinate, in pixels, for the new location of the text cursor.
   *
   * \details
   * The location of the text cursor is set the the specified coordinates.
   * The coordinates are in pixels. Since the coordinates can specify any pixel
   * location, the text does not have to be placed on specific rows.
   * As with all drawing functions, location 0, 0 is the top left corner of
   * the display. The cursor location will be the top left corner of the next
   * character written.
   *
   * \see getCursorX() getCursorY()
   */
  static void setCursor(int16_t x, int16_t y);

  /** \brief
   * Get the X coordinate of the current text cursor position.
   *
   * \return The X coordinate of the current text cursor position.
   *
   * \details
   * The X coordinate returned is a pixel location with 0 indicating the
   * leftmost column.
   *
   * \see getCursorY() setCursor()
   */
  static int16_t getCursorX();

  /** \brief
   * Get the Y coordinate of the current text cursor position.
   *
   * \return The Y coordinate of the current text cursor position.
   *
   * \details
   * The Y coordinate returned is a pixel location with 0 indicating the
   * topmost row.
   *
   * \see getCursorX() setCursor()
   */
  static int16_t getCursorY();

  /** \brief
   * Set the text foreground color.
   *
   * \param color The color to be used for following text.
   *
   * \see setTextBackground() getTextColor()
   */
  static void setTextColor(uint8_t color);

  /** \brief
   * Get the currently set text foreground color.
   *
   * \return The color that will be used to display any following text.
   *
   * \see setTextColor()
   */
  static uint8_t getTextColor();

  /** \brief
   * Set the text background color.
   *
   * \param bg The background color to be used for following text.
   *
   * \see setTextColor() getTextBackground()
   */
  static void setTextBackground(uint8_t bg);

  /** \brief
   * Get the currently set text background color.
   *
   * \return The background color that will be used to display any following text.
   *
   * \see setTextBackground()
   */
  static uint8_t getTextBackground();

  /** \brief
   * Set the text character size.
   *
   * \param s The text size multiplier. Must be 1 or higher.
   *
   * \details
   * Setting a text size of 1 will result in standard size characters which
   * occupy 6x8 pixels (the result of 5x7 characters with spacing on the
   * right and bottom edges).
   *
   * The value specified is a multiplier. A value of 2 will double the
   * size so they will occupy 12x16 pixels. A value of 3 will result in
   * 18x24, etc.
   *
   * \see getTextSize()
   */
  static void setTextSize(uint8_t s);

  /** \brief
   * Get the currently set text size.
   *
   * \return The size that will be used for any following text.
   *
   * \see setTextSize()
   */
  static uint8_t getTextSize();

  /** \brief
   * Set or disable text wrap mode.
   *
   * \param w `true` enables text wrap mode. `false` disables it.
   *
   * \details
   * Text wrap mode is enabled by specifying `true`. In wrap mode, the text
   * cursor will be moved to the start of the next line (based on the current
   * text size) if the following character wouldn't fit entirely at the end of
   * the current line.

   * If wrap mode is disabled, characters will continue to be written to the
   * same line. A character at the right edge of the screen may only be
   * partially displayed and additional characters will be off screen.
   *
   * \see getTextWrap()
   */
  static void setTextWrap(bool w);

  /** \brief
   * Get the currently set text wrap mode.
   *
   * \return `true` if text wrapping is on, `false` if wrapping is off.
   *
   * \see setTextWrap()
   */
  static bool getTextWrap();

  /** \brief
   * Clear the display buffer and set the text cursor to location 0, 0
   */
  static void clear();
};

#endif
