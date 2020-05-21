/**
 * @file Blending.h
 * \brief
 * Pre-built blending definitions.
 */

#ifndef BLENDING_H
#define BLENDING_H

#include <Arduino.h>
#include "Color.h"

/** \brief
 * Represents a blending function that blends `a` onto `b`.
 *
 * \param a The incoming color at the location being blended.
 * \param b The current color at the location being blended.
 */
typedef Color (*BlendFunc) (Color a, Color b);

/** \brief
 * Performs standard alpha blending.
 *
 * \param a The incoming color at the location being blended.
 * \param b The current color at the location being blended.
 */
Color BLEND_ALPHA(Color a, Color b);

/** \brief
 * Does not perform blending, and simply returns incoming color.
 *
 * \param a The incoming color at the location being blended.
 * \param b The current color at the location being blended.
 */
Color BLEND_NONE(Color a, Color b) __attribute__((always_inline));

#endif
