/**
 * @file DotMGCore.h
 * \brief
 * The DotMGCore class for dotMG hardware initilization and control.
 */

#ifndef DOTMG_CORE_H
#define DOTMG_CORE_H

#include <Arduino.h>
#include "Color.h"

// ----- Helpful values/macros -----

// Button values

#define A_BUTTON_BIT        0
#define A_BUTTON            bit(A_BUTTON_BIT)

#define B_BUTTON_BIT        1
#define B_BUTTON            bit(B_BUTTON_BIT)

#define UP_BUTTON_BIT       2
#define UP_BUTTON           bit(UP_BUTTON_BIT)

#define DOWN_BUTTON_BIT     3
#define DOWN_BUTTON         bit(DOWN_BUTTON_BIT)

#define LEFT_BUTTON_BIT     4
#define LEFT_BUTTON         bit(LEFT_BUTTON_BIT)

#define RIGHT_BUTTON_BIT    5
#define RIGHT_BUTTON        bit(RIGHT_BUTTON_BIT)

#define START_BUTTON_BIT    6
#define START_BUTTON        bit(START_BUTTON_BIT)

#define SELECT_BUTTON_BIT   7
#define SELECT_BUTTON       bit(SELECT_BUTTON_BIT)

// Display values

#define WIDTH       160
#define HEIGHT      128

// ----- SPI/DMA configuration -----

#define SPI_SETTINGS        SPISettings(24000000, MSBFIRST, SPI_MODE0)
#define SPI_SERCOM          SERCOM1
#define DMA_TRIGGER_SRC     SERCOM1_DMAC_ID_TX
#define DMA_CHAN            0u


// ----- Pins -----

#define IO_PORT             (&(PORT->Group[PORTA]))

#define PIN_DISP_SS         16
#define MASK_DISP_SS        digitalPinToBitMask(PIN_DISP_SS)

#define PIN_DISP_DC         17
#define MASK_DISP_DC        digitalPinToBitMask(PIN_DISP_DC)

#define MASK_SPI_MOSI       digitalPinToBitMask(PIN_SPI_MOSI)
#define MASK_SPI_SCK        digitalPinToBitMask(PIN_SPI_SCK)

#define PIN_BUTTON_A        9
#define MASK_BUTTON_A       digitalPinToBitMask(PIN_BUTTON_A)

#define PIN_BUTTON_B        10
#define MASK_BUTTON_B       digitalPinToBitMask(PIN_BUTTON_B)

#define PIN_BUTTON_UP       5
#define MASK_BUTTON_UP      digitalPinToBitMask(PIN_BUTTON_UP)

#define PIN_BUTTON_DOWN     6
#define MASK_BUTTON_DOWN    digitalPinToBitMask(PIN_BUTTON_DOWN)

#define PIN_BUTTON_LEFT     7
#define MASK_BUTTON_LEFT    digitalPinToBitMask(PIN_BUTTON_LEFT)

#define PIN_BUTTON_RIGHT    8
#define MASK_BUTTON_RIGHT   digitalPinToBitMask(PIN_BUTTON_RIGHT)

#define PIN_BUTTON_START    14
#define MASK_BUTTON_START   digitalPinToBitMask(PIN_BUTTON_START)

#define PIN_BUTTON_SELECT   15
#define MASK_BUTTON_SELECT  digitalPinToBitMask(PIN_BUTTON_SELECT)

#define PIN_SPEAKER         PIN_DAC0


// ----- For display configuration (ST7735R) -----

#define ST77XX_NOP        0x00
#define ST77XX_SWRESET    0x01
#define ST77XX_RDDID      0x04
#define ST77XX_RDDST      0x09

#define ST77XX_SLPIN      0x10
#define ST77XX_SLPOUT     0x11
#define ST77XX_PTLON      0x12
#define ST77XX_NORON      0x13

#define ST77XX_INVOFF     0x20
#define ST77XX_INVON      0x21
#define ST77XX_DISPOFF    0x28
#define ST77XX_DISPON     0x29
#define ST77XX_CASET      0x2A
#define ST77XX_RASET      0x2B
#define ST77XX_RAMWR      0x2C
#define ST77XX_RAMRD      0x2E

#define ST77XX_PTLAR      0x30
#define ST77XX_COLMOD     0x3A
#define ST77XX_MADCTL     0x36

#define ST77XX_MADCTL_MY  0x80
#define ST77XX_MADCTL_MX  0x40
#define ST77XX_MADCTL_MV  0x20
#define ST77XX_MADCTL_ML  0x10
#define ST77XX_MADCTL_RGB 0x00

#define ST77XX_RDID1      0xDA
#define ST77XX_RDID2      0xDB
#define ST77XX_RDID3      0xDC
#define ST77XX_RDID4      0xDD

#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

#define ST7735_FRMCTR1    0xB1
#define ST7735_FRMCTR2    0xB2
#define ST7735_FRMCTR3    0xB3
#define ST7735_INVCTR     0xB4
#define ST7735_DISSET5    0xB6

#define ST7735_PWCTR1     0xC0
#define ST7735_PWCTR2     0xC1
#define ST7735_PWCTR3     0xC2
#define ST7735_PWCTR4     0xC3
#define ST7735_PWCTR5     0xC4
#define ST7735_VMCTR1     0xC5

#define ST7735_PWCTR6     0xFC

#define ST7735_GMCTRP1    0xE0
#define ST7735_GMCTRN1    0xE1

// --------------------


/** \brief
 * Lower level functions generally dealing directly with the hardware.
 *
 * \details
 * This class is inherited by DotMGBase and thus also DotMG, so it wouldn't
 * normally be used directly by a sketch.
 */
class DotMGCore
{
  public:
    DotMGCore();

    /** \brief
     * Get the width of the display in pixels.
     *
     * \return The width of the display in pixels.
     */
    constexpr uint8_t static width() { return WIDTH; }

    /** \brief
     * Get the height of the display in pixels.
     *
     * \return The height of the display in pixels.
     */
    constexpr uint8_t static height() { return HEIGHT; }

    /** \brief
     * Paints an entire image directly to the display from program memory.
     *
     * \param image A byte array in program memory representing the entire
     * contents of the display.
     *
     * \details
     * The contents of the specified array in program memory are written to the
     * display. Every three bytes represents a horizontal row of two 12-bit
     * pixels, similar to `DotMGBase::frameBuffer()`.
     *
     * \see DotMGBase::frameBuffer()
     */
    static void paintScreen(const uint8_t *image);

    /** \brief
     * Paints an entire image directly to the display from an array in RAM.
     *
     * \param image A byte array in RAM representing the entire contents of
     * the display.
     * \param clear If `true` the array in RAM will be cleared to zeros upon
     * return from this function. If `false` the RAM buffer will remain
     * unchanged. (optional; defaults to `false`)
     *
     * \details
     * The contents of the specified array in RAM are written to the display.
     * Every three bytes represents a horizontal row of two 12-bit pixels,
     * similar to `DotMGBase::frameBuffer()`.
     *
     * \see DotMGBase::frameBuffer()
     *
     * If parameter `clear` is set to `true` the RAM array will be cleared to
     * zeros after its contents are written to the display.
     */
    static void paintScreen(uint8_t image[], bool clear = false);

    /** \brief
     * Blank the display screen by setting all pixels off.
     *
     * \details
     * All pixels on the screen will be written with a value of 0 to turn
     * them off.
     */
    static void blank();

    /** \brief
     * Invert display colors.
     *
     * \param inverse `true` will invert the display. `false` will set the
     * display to normal.
     *
     * \details
     * Once in inverted mode, the display will remain this way until it is
     * set back to non-inverted mode by calling this function with `false`.
     */
    static void invert(bool inverse);

    /** \brief
     * Flip the display vertically or set it back to normal.
     *
     * \param flipped `true` will set vertical flip mode. `false` will set
     * normal vertical orientation.
     *
     * \details
     * Calling this function with a value of `true` will cause the Y coordinate
     * to start at the bottom edge of the display instead of the top,
     * effectively flipping the display vertically.
     *
     * Once in vertical flip mode, it will remain this way until normal
     * vertical mode is set by calling this function with a value of `false`.
     *
     * \see flipHorizontal()
     */
    static void flipVertical(bool flipped);

    /** \brief
     * Flip the display horizontally or set it back to normal.
     *
     * \param flipped `true` will set horizontal flip mode. `false` will set
     * normal horizontal orientation.
     *
     * \details
     * Calling this function with a value of `true` will cause the X coordinate
     * to start at the right edge of the display instead of the left,
     * effectively flipping the display horizontally.
     *
     * Once in horizontal flip mode, it will remain this way until normal
     * horizontal mode is set by calling this function with a value of `false`.
     *
     * \see flipVertical()
     */
    static void flipHorizontal(bool flipped);

    /** \brief
     * Turn all display pixels on or display the buffer contents.
     *
     * \param on `true` turns all pixels on. `false` displays the contents
     * of the hardware display buffer.
     *
     * \details
     * Calling this function with a value of `true` will override the contents
     * of the hardware display buffer and turn all pixels on. The contents of
     * the hardware buffer will remain unchanged.
     *
     * Calling this function with a value of `false` will set the normal state
     * of displaying the contents of the hardware display buffer.
     *
     * \note
     * All pixels will be lit even if the display is in inverted mode.
     *
     * \see invert()
     */
    static void allPixelsOn(bool on);

    /** \brief
     * Turn the display off.
     *
     * \details
     * The display will clear and be put into a low power mode. This can be
     * used to extend battery life when a game is paused or when a sketch
     * doesn't require anything to be displayed for a relatively long period
     * of time.
     *
     * \see displayOn()
     */
    static void displayOff();

    /** \brief
     * Turn the display on.
     *
     * \details
     * Used to power up and reinitialize the display after calling
     * `displayOff()`.
     *
     * \see displayOff()
     */
    static void displayOn();

    /** \brief
     * Get the current state of all buttons as a bitmask.
     *
     * \return A bitmask of the state of all the buttons.
     *
     * \details
     * The returned mask contains a bit for each button. For any pressed button,
     * its bit will be 1. For released buttons their associated bits will be 0.
     *
     * The following defined mask values should be used for the buttons:
     *
     * `A_BUTTON`, `B_BUTTON`, `UP_BUTTON`, `DOWN_BUTTON`, `LEFT_BUTTON`,
     * `RIGHT_BUTTON`, `START_BUTTON`, `SELECT_BUTTON`
     */
    uint8_t static buttonsState();

  protected:
    static void boot();
};

#endif
