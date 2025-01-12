#include <stdlib.h>

#include "colors.h"

HSV_t rgb_to_hsv(RGB_t rgb)
{
    HSV_t hsv;
    uint8_t rgb_min = (rgb.r < rgb.g) ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    uint8_t rgb_max = (rgb.r > rgb.g) ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);
    uint8_t delta = rgb_max - rgb_min;

    hsv.v = rgb_max;  // Value is the max component

    if (rgb_max == 0) {
        hsv.s = 0;  // Saturation is 0 if the value is 0
        hsv.h = 0;  // Hue is undefined; set to 0
        return hsv;
    }

    hsv.s = (delta * 255) / rgb_max;  // Saturation scaled to 0-255

    if (delta == 0) {
        hsv.h = 0;  // Hue is undefined when all RGB components are the same
        return hsv;
    }

    if (rgb_max == rgb.r) {
        hsv.h = 60 * ((int16_t)(rgb.g - rgb.b) * 100 / delta) / 100;
    } else if (rgb_max == rgb.g) {
        hsv.h = 60 * (((int16_t)(rgb.b - rgb.r) * 100 / delta) / 100 + 2);
    } else {
        hsv.h = 60 * (((int16_t)(rgb.r - rgb.g) * 100 / delta) / 100 + 4);
    }

    // if (hsv.h < 0) hsv.h += 360;  // Wrap around to ensure positive hue

    return hsv;
}

RGB_t hsv_to_rgb(HSV_t hsv)
{
    RGB_t rgb;
    uint8_t region, remainder, p, q, t;

    if (hsv.s == 0) {
        rgb.r = rgb.g = rgb.b = hsv.v;  // If saturation is 0, all RGB are equal to value
        return rgb;
    }

    region = hsv.h / 60;           // Region (0 to 5)
    remainder = (hsv.h % 60) * 255 / 60;  // Remainder scaled to 0-255

    p = (hsv.v * (255 - hsv.s)) / 255;
    q = (hsv.v * (255 - (hsv.s * remainder) / 255)) / 255;
    t = (hsv.v * (255 - (hsv.s * (255 - remainder)) / 255)) / 255;

    switch (region) {
    case 0:
        rgb.r = hsv.v;
        rgb.g = t;
        rgb.b = p;
        break;
    case 1:
        rgb.r = q;
        rgb.g = hsv.v;
        rgb.b = p;
        break;
    case 2:
        rgb.r = p;
        rgb.g = hsv.v;
        rgb.b = t;
        break;
    case 3:
        rgb.r = p;
        rgb.g = q;
        rgb.b = hsv.v;
        break;
    case 4:
        rgb.r = t;
        rgb.g = p;
        rgb.b = hsv.v;
        break;
    default:
        rgb.r = hsv.v;
        rgb.g = p;
        rgb.b = q;
        break;
    }

    return rgb;
}
