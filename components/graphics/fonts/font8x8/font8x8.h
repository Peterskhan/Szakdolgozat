#pragma once
#ifndef GRAPHICS_FONTS_FONT8X8_FONT8X8_H_INCLUDED
#define GRAPHICS_FONTS_FONT8X8_FONT8X8_H_INCLUDED

// Standard includes
#include <cstdint>

// Project includes
#include "../../graphics.h"


// Extra symbol definitions
#define CODEFONT_8X8_CELSIUS           (95)
#define CODEFONT_8X8_FAHRENHEIT        (96)
#define CODEFONT_8X8_HUMIDITY_GAUGE    (97)
#define CODEFONT_8x8_MSG               (98)
#define CODEFONT_8X8_PASCAL            (99)
#define CODEFONT_8X8_PRESSURE_GAUGE    (100)
#define CODEFONT_8X8_TEMPERATURE_GAUGE (101)
#define CODEFONT_8X8_UNPRINTABLE       (102)
#define CODEFONT_8X8_BOOT_SCREEN       (103)

/** Pixel data for the Font8x8 symbol set. */
extern const uint8_t font8x8_data[];

/** Descriptors for the Font8x8 symbol set. */
extern const Symbol font8x8_descriptors[];

#endif // GRAPHICS_FONTS_FONT8X8_FONT8X8_H_INCLUDED
