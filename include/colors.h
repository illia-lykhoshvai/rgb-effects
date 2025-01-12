#pragma once

#include <stdint.h>

#define RGB_BLACK ((RGB_t){.r=0,.g=0,.b=0})
#define RGB_WHITE ((RGB_t){.r=0xff,.g=0xff,.b=0xff})
#define RGB_CYAN ((RGB_t){.r=0,.g=0xff,.b=0xff})

typedef struct {
    uint16_t h; // Hue: 0-360 (scaled to fit 0-360 in 16 bits)
    uint8_t s;  // Saturation: 0-255
    uint8_t v;  // Value (Brightness): 0-255
} HSV_t;

typedef struct {
    uint8_t r;  // Red: 0-255
    uint8_t g;  // Green: 0-255
    uint8_t b;  // Blue: 0-255
} RGB_t;

HSV_t rgb_to_hsv(RGB_t rgb);
RGB_t hsv_to_rgb(HSV_t hsv);
