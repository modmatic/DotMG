/**
 * @file Color.h
 * \brief
 * The Color class and pre-built color definitions.
 */

#ifndef COLOR_H
#define COLOR_H

// Useful 16-bit 4444-formatted RGBA color values

#define COLOR_CLEAR      0x0000
#define COLOR_BLACK      0x000F
#define COLOR_WHITE      0xFFFF
#define COLOR_GRAY       0xAAAF
#define COLOR_RED        0xF00F
#define COLOR_GREEN      0x0F0F
#define COLOR_BLUE       0x00FF
#define COLOR_CYAN       0x0FFF
#define COLOR_MAGENTA    0xF0FF
#define COLOR_YELLOW     0xFF0F
#define COLOR_ORANGE     0xF40F


/** \brief
 * Class for working with 16-bit 4444-formatted RGBA colors.
 *
 * \details
 * Colors are made up of four channels: red, green, blue, and alpha. The alpha
 * channel designates the level of opacity. Each channel is four bits wide and
 * can therefore have a value from 0 to 15 (0x0F). The channel bits are ordered
 * as follows:
 *
 * Data: R3 R2 R1 R0 G3 G2 G1 G0 B3 B2 B1 B0 A3 A2 A1 A0
 *  Bit: 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
 */
class Color
{
public:
  /** \brief
   * Constructs a default 16-bit 4444-formatted RGBA color value.
   */
  Color() {}

  /** \brief
   * Constructs a 16-bit 4444-formatted RGBA color value from individual 4-bit
   * RGBA channels.
   *
   * \param r The red channel value.
   * \param g The green channel value.
   * \param b The blue channel value.
   * \param a The alpha channel value (optional; defaults to 100% opacity).
   */
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0x0F)
    : value(((r & 0xF) << 12) | ((g & 0xF) << 8) | ((b & 0xF) << 4) | (a & 0xF))
  {}

  /** \brief
   * Constructs a 16-bit 4444-formatted RGBA color value from a single 16-bit
   * integer.
   *
   * \param value The color value.
   */
  Color(uint16_t value)
    : value(value)
  {}

  operator uint16_t()
  {
    return value;
  }

  /** \brief
   * Builds a 16-bit 4444-formatted RGBA color value from individual floating
   * point RGBA channels.
   *
   * \param r The red channel value. Must be in the range of 0.0 to 1.0.
   * \param g The green channel value. Must be in the range of 0.0 to 1.0.
   * \param b The blue channel value. Must be in the range of 0.0 to 1.0.
   * \param a The alpha channel value (optional; defaults to 100% opacity).
   * Must be in the range of 0.0 to 1.0.
   */
  static Color fromFloatRGBA(float r, float g, float b, float a = 1.0f)
  {
    return Color(
      r*0x0F,
      g*0x0F,
      b*0x0F,
      a*0x0F
    );
  }

  /** \brief
   * Builds a 16-bit 4444-formatted RGBA color value from individual 8-bit RGBA
   * channels.
   *
   * \param r The red channel value.
   * \param g The green channel value.
   * \param b The blue channel value.
   * \param a The alpha channel value (optional; defaults to 100% opacity).
   *
   * \details
   * The channel values will be scaled (not truncated) to fit into 4-bits.
   */
  static Color from8BitRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)
  {
    return Color(
      (r*0x0F)/0xFF,
      (g*0x0F)/0xFF,
      (b*0x0F)/0xFF,
      (a*0x0F)/0xFF
    );
  }

  /** \brief
   * Builds a 16-bit 4444-formatted RGBA color value from a standard 32-bit
   * hex color value.
   *
   * \param hexColor The color to convert.
   */
  static Color fromHex(uint32_t hexColor)
  {
    return from8BitRGBA(
      (hexColor & 0xFF000000) >> 24,
      (hexColor & 0x00FF0000) >> 16,
      (hexColor & 0x0000FF00) >> 8,
      (hexColor & 0x000000FF) >> 0
    );
  }

  /** \brief
   * Returns the complement of this color, preserving the original alpha
   * channel.
   */
  Color complement() const
  {
    return (0xFFF0 - (value & 0xFFF0)) | a();
  }

  /** \brief
   * Returns the 4-bit red component of this color.
   */
  uint8_t r() const
  {
    return (value & 0xF000) >> 12;
  }

  /** \brief
   * Sets the 4-bit red component of this color.
   *
   * \param val The value to set. Must be a 4-bit value.
   */
  void r(uint8_t val)
  {
    uint16_t mask = 0x0F << 12;
    value &= ~mask;
    value |= (val << 12) & mask;
  }

  /** \brief
   * Returns the 4-bit green component of this color.
   */
  uint8_t g() const
  {
    return (value & 0x0F00) >> 8;
  }

  /** \brief
   * Sets the 4-bit green component of this color.
   *
   * \param val The value to set. Must be a 4-bit value.
   */
  void g(uint8_t val)
  {
    uint16_t mask = 0x0F << 8;
    value &= ~mask;
    value |= (val << 8) & mask;
  }

  /** \brief
   * Returns the 4-bit blue component of this color.
   */
  uint8_t b() const
  {
    return (value & 0x00F0) >> 4;
  }

  /** \brief
   * Sets the 4-bit blue component of this color.
   *
   * \param val The value to set. Must be a 4-bit value.
   */
  void b(uint8_t val)
  {
    uint16_t mask = 0x0F << 4;
    value &= ~mask;
    value |= (val << 4) & mask;
  }

  /** \brief
   * Returns the 4-bit alpha component of this color.
   */
  uint8_t a() const
  {
    return value & 0x000F;
  }

  /** \brief
   * Sets the 4-bit alpha component of this color.
   *
   * \param val The value to set. Must be a 4-bit value.
   */
  void a(uint8_t val)
  {
    uint16_t mask = 0x0F;
    value &= ~mask;
    value |= val & mask;
  }

  /** \brief
   * Raw 16-bit value of this color.
   */
  uint16_t value;
};

#endif
