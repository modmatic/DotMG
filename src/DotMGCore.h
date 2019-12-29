/**
 * @file DotMGCore.h
 * \brief
 * The DotMGCore class for dotMG hardware initilization and control.
 */

#ifndef DOTMG_CORE_H
#define DOTMG_CORE_H

#include <Arduino.h>
#include <avr/power.h>
#include <avr/sleep.h>

// main hardware compile flags

// ----- dotMG pins -----

#define PORT_SCK_MISO_MOSI_SS_DC_RST   PORTB
#define DDR_SCK_MISO_MOSI_SS_DC_RST    DDRB

#define PORT_CS_SPK_SEL_ST             PORTC
#define PORTIN_CS_SPK_SEL_ST           PINC
#define DDR_CS_SPK_SEL_ST              DDRC

#define PORT_RT_LFT_DN_UP_B_A_TX_RX    PORTD
#define PORTIN_RT_LFT_DN_UP_A_B_TX_RX  PIND
#define DDR_RT_LFT_DN_UP_B_A_TX_RX     DDRD

#define PIN_TFT_CS          A3
#define BIT_TFT_CS          (PIN_TFT_CS - A0)

#define PIN_TFT_DC          9
#define BIT_TFT_DC          (PIN_TFT_DC - 8)

#define PIN_TFT_RST         8
#define BIT_TFT_RST         (PIN_TFT_RST - 8)

#define BIT_SPI_SS          (PIN_SPI_SS - 8)
#define BIT_SPI_MOSI        (PIN_SPI_MOSI - 8)
#define BIT_SPI_SCK         (PIN_SPI_SCK - 8)

#define PIN_BUTTON_A        3
#define BIT_BUTTON_A        (PIN_BUTTON_A - 0)

#define PIN_BUTTON_B        2
#define BIT_BUTTON_B        (PIN_BUTTON_B - 0)

#define PIN_BUTTON_UP       4
#define BIT_BUTTON_UP        (PIN_BUTTON_UP - 0)

#define PIN_BUTTON_DOWN     5
#define BIT_BUTTON_DOWN     (PIN_BUTTON_DOWN - 0)

#define PIN_BUTTON_LEFT     6
#define BIT_BUTTON_LEFT     (PIN_BUTTON_LEFT - 0)

#define PIN_BUTTON_RIGHT    7
#define BIT_BUTTON_RIGHT    (PIN_BUTTON_RIGHT - 0)

#define PIN_BUTTON_START    A0
#define BIT_BUTTON_START    (PIN_BUTTON_START - A0)

#define PIN_BUTTON_SELECT   A1
#define BIT_BUTTON_SELECT   (PIN_BUTTON_SELECT - A0)

#define B_BUTTON            bit(0)
#define A_BUTTON            bit(1)
#define UP_BUTTON           bit(2)
#define DOWN_BUTTON         bit(3)
#define LEFT_BUTTON         bit(4)
#define RIGHT_BUTTON        bit(5)
#define START_BUTTON        bit(6)
#define SELECT_BUTTON       bit(7)

#define PIN_SPEAKER         A2
#define BIT_SPEAKER         (PIN_SPEAKER - A0)

#define RED_LED    0
#define GREEN_LED  1
#define BLUE_LED   2

#define RGB_OFF    0
#define RGB_ON     1

#define TFT_WIDTH   160
#define TFT_HEIGHT  128

#define WIDTH       80
#define HEIGHT      64

// Converts individual color channels to a 12-bit 444-formatted color value.
// Channel values must be between 0 and 15, inclusive.
#define COLOR(R, G, B) ((((R) & 0xF) << 8) | (((G) & 0xF) << 4) | ((B) & 0xF))

// 2-bit pixel color values
#define COLOR_BG 0
#define COLOR_A  1
#define COLOR_B  2
#define COLOR_C  3

// Some ready-made 12-bit ('444') color values
#define ST77XX_WHITE      0x0FFF
#define ST77XX_GRAY       0x0AAA
#define ST77XX_DARKGRAY   0x0555
#define	ST77XX_BLACK      0x0000
#define ST77XX_RED        0x0F00
#define ST77XX_GREEN      0x00F0
#define ST77XX_BLUE       0x000F
#define ST77XX_CYAN       0x00FF
#define ST77XX_MAGENTA    0x0F0F
#define ST77XX_YELLOW     0x0FF0
#define	ST77XX_ORANGE     0x0F40

// Some register settings
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
 * This class is inherited by DotMGBase and thus also DotMG, so wouldn't
 * normally be used directly by a sketch.
 *
 * \note
 * A friend class named _DotMGEx_ is declared by this class. The intention
 * is to allow a sketch to create an _DotMGEx_ class which would have access
 * to the private and protected members of the DotMGCore class. It is hoped
 * that this may eliminate the need to create an entire local copy of the
 * library, in order to extend the functionality, in most circumstances.
 */
class DotMGCore
{
  friend class DotMGEx;

  public:
    DotMGCore();

    /** \brief
     * Idle the CPU to save power.
     *
     * \details
     * This puts the CPU in _idle_ sleep mode. You should call this as often
     * as you can for the best power savings. The timer 0 overflow interrupt
     * will wake up the chip every 1ms, so even at 60 FPS a well written
     * app should be able to sleep maybe half the time in between rendering
     * it's own frames.
     */
    void static idle();

    /** \brief
     * Put the display into data mode.
     *
     * \details
     * When placed in data mode, data that is sent to the display will be
     * considered as data to be displayed.
     *
     * \note
     * This is a low level function that is not intended for general use in a
     * sketch. It has been made public and documented for use by derived
     * classes.
     *
     * \see LCDCommandMode() transferSPI()
     */
    void static LCDDataMode();

    /** \brief
     * Put the display into command mode.
     *
     * \details
     * When placed in command mode, data that is sent to the display will be
     * treated as commands.
     *
     * See the ST7735R controller and TFT display documents for available
     * commands and command sequences.
     *
     * \note
     * This is a low level function that is not intended for general use in a
     * sketch. It has been made public and documented for use by derived
     * classes.
     *
     * \see LCDDataMode() sendLCDCommand() transferSPI()
     */
    void static LCDCommandMode();

    /** \brief
     * Initializes SPI transfers for the TFT.
     *
     * \details
     * Lowers the CS pin of the TFT, allowing SPI data to be sent to it.
     * Use before one or more calls to transferSPI(). Other SPI devices
     * cannot receive data while the CS pin of the TFT is low.
     *
     * \see endTransferSPI() transferSPI()
     */
    void static startTransferSPI();

    /** \brief
     * Terminates SPI transfers for the TFT.
     *
     * \details
     * Raises the CS pin of the TFT, allowing other SPI devices to accept data.
     * Use after one or mor calls to transferSPI().
     *
     * \see startTransferSPI() transferSPI()
     */
    void static endTransferSPI();

    /** \brief
     * Transfer a byte to the display.
     *
     * \param data The byte to be sent to the display.
     *
     * \details
     * Transfer one byte to the display over the SPI port and wait for the
     * transfer to complete. The byte will either be interpreted as a command
     * or as data to be placed on the screen, depending on the command/data
     * mode.
     *
     * \see LCDDataMode() LCDCommandMode() sendLCDCommand()
     */
    void static transferSPI(uint8_t data);

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
    void static displayOff();

    /** \brief
     * Turn the display on.
     *
     * \details
     * Used to power up and reinitialize the display after calling
     * `displayOff()`.
     *
     * \note
     * The previous call to `displayOff()` will have caused the display's
     * buffer contents to be lost. The display will have to be re-painted,
     * which is usually done by calling `display()`.
     *
     * \see displayOff()
     */
    void static displayOn();

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
     * A_BUTTON, B_BUTTON, UP_BUTTON, DOWN_BUTTON, LEFT_BUTTON, RIGHT_BUTTON, START_BUTTON, SELECT_BUTTON
     */
    uint8_t static buttonsState();

    /** \brief
     * Get the actual 12-bit display color for a given 2-bit color.
     *
     * \param color The color to get. Must be COLOR_A, COLOR_B, COLOR_C, or COLOR_BG.
     *
     * \details
     * Returns a 12-bit 444-formatted RGB color value.
     *
     * \note
     * COLOR_BG is used as the background color, as it is used by any screen-clearing
     * operations.
     *
     * \see setColor() blank()
     */
    uint16_t static getColorValue(uint8_t color);

    /** \brief
     * Set the actual 12-bit display color for a given 2-bit color.
     *
     * \param color The color to set. Must be COLOR_A, COLOR_B, COLOR_C, or COLOR_BG.
     * \param value The color value to set. Must be a 12-bit 444-formatted RGB color value.
     *
     * \details
     * May be called before begin(). Value will take effect on next call to display().
     *
     * \note
     * COLOR_BG is used as the background color, as it is used by any screen-clearing
     * operations.
     *
     * You can use the COLOR(r, g, b) macro to convert individual color channels
     * to a 12-bit 444-formatted color value.
     *
     * \see getColor() blank()
     */
    void static setColorValue(uint8_t color, uint16_t value);

    /** \brief
     * Paints an entire image directly to the display from program memory.
     *
     * \param image A byte array in program memory representing the entire
     * contents of the display.
     *
     * \details
     * The contents of the specified array in program memory are written to the
     * display.
     *
     * Each byte in the array specifies a horizontal row of 4 pixels, with the
     * most significant bit at the left end of the row.
     *
     * The format for each byte is as follows, where WW, XX, YY, and ZZ each
     * represent a pixel value that is one of COLOR_A, COLOR_B, COLOR_C, or
     * COLOR_BG:
     *
     *         WW XX YY ZZ
     *    bit: 76 54 32 10
     */
    void static paintScreen(const uint8_t *image);

    /** \brief
     * Paints an entire image directly to the display from an array in RAM.
     *
     * \param image A byte array in RAM representing the entire contents of
     * the display.
     * \param clear If `true` the array in RAM will be cleared upon return
     * from this function. If `false` the RAM buffer will remain unchanged.
     * (optional; defaults to `false`)
     *
     * \details
     * The contents of the specified array in RAM are written to the display.
     *
     * Each byte in the array specifies a horizontal row of 4 pixels, with the
     * most significant bit at the left end of the row.
     *
     * The format for each byte is as follows, where WW, XX, YY, and ZZ each
     * represent a pixel value that is one of COLOR_A, COLOR_B, COLOR_C, or
     * COLOR_BG:
     *
     *         WW XX YY ZZ
     *    bit: 76 54 32 10
     */
    void static paintScreen(uint8_t image[], bool clear = false);

    /** \brief
     * Blank the display screen by setting all pixels to COLOR_BG's configured
     * color.
     *
     * \see getColorValue() setColorValue()
     */
    void static blank();

    /** \brief
     * Invert the entire display or set it back to normal.
     *
     * \param inverse `true` will invert the display. `false` will set the
     * display to non-inverted.
     *
     * \details
     * Calling this function with a value of `true` will set the display to
     * inverted mode.
     *
     * Once in inverted mode, the display will remain this way until it is
     * set back to non-inverted mode by calling this function with `false`.
     */
    void static invert(bool inverse);

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
     * All pixels will be lit, even if the display is in inverted mode.
     *
     * \see invert()
     */
    void static allPixelsOn(bool on);

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
    void static flipVertical(bool flipped);

    /** \brief
     * Flip the display horizontally or set it back to normal.
     *
     * \param flipped `true` will set horizontal flip mode. `false` will set
     * normal horizontal orientation.
     *
     * \details
     * Calling this function with a value of `true` will cause the X coordinate
     * to start at the left edge of the display instead of the right,
     * effectively flipping the display horizontally.
     *
     * Once in horizontal flip mode, it will remain this way until normal
     * horizontal mode is set by calling this function with a value of `false`.
     *
     * \see flipVertical()
     */
    void static flipHorizontal(bool flipped);

    /** \brief
     * Send a single command byte to the display.
     *
     * \param command The command byte to send to the display.
     *
     * \details
     * The display will be set to command mode then the specified command
     * byte will be sent. The display will then be set to data mode.
     * Multi-byte commands can be sent by calling this function multiple times.
     *
     * \note
     * Sending improper commands to the display can place it into invalid or
     * unexpected states, possibly even causing physical damage.
     */
    void static sendLCDCommand(uint8_t command);

  protected:
    // internals
    void static bootSPI();
    void static bootTFT();
    void static bootPins();
    void static bootPowerSaving();

  private:
    uint16_t static colors[4];
    uint8_t static frameCount;
    uint8_t static frameRender;
    uint8_t static MADCTL;
    bool static inverted;

    inline void static setWriteRegion(uint8_t x = 0, uint8_t y = 0, uint8_t width = TFT_WIDTH, uint8_t height = TFT_HEIGHT);
};

#endif
