/**
 * @file Blending.cpp
 * \brief
 * Pre-built blending definitions.
 */

#include "Blending.h"

Color BLEND_NONE(Color a, Color b)
{
  return a;
}

Color BLEND_ALPHA(Color a, Color b)
{
  uint8_t a0 = a.a();

  if (a0 == 0xF)
    return a;

  if (a0 == 0)
    return b;

  uint8_t a1 = 0xF - a0;
  return Color(
    (a.r() * a0 + b.r() * a1)/0xF,
    (a.g() * a0 + b.g() * a1)/0xF,
    (a.b() * a0 + b.b() * a1)/0xF
  );
}

Color BLEND_ALPHA_GRAY(Color a, Color b)
{
  return BLEND_ALPHA(a, b).grayscale();
}
