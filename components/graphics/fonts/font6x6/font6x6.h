#pragma once
#ifndef GRAPHICS_FONTS_FONT6X6_FONT6X6_H_INCLUDED
#define GRAPHICS_FONTS_FONT6X6_FONT6X6_H_INCLUDED

// Standard includes
#include <cstdint>

// Project includes
#include "../../graphics.h"

#define FONT6X6_THUNDERSTORM (102)
#define FONT6X6_SUNNY        (101)
#define FONT6X6_SNOW         (100)
#define FONT6X6_RAIN_SAD     (99)
#define FONT6X6_RAIN         (98)
#define FONT6X6_FOGGY        (97)
#define FONT6X6_CLOUDY_SUN   (96)
#define FONT6X6_CLOUDS       (95)


/** Pixel data for the Font6x6 symbol set. */
extern const uint8_t font6x6_data[];

/** Descriptors for the Font6x6 symbol set. */
extern const Symbol font6x6_descriptors[];

#endif // GRAPHICS_FONTS_FONT6X6_FONT6X6_H_INCLUDED

