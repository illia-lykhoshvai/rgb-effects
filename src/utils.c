#include "utils.h"

// Map value from one range to another using integer math
uint16_t map_range(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) 
{
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Factor is scaled 0-255, where 0 is all color1 and 255 is all color2
RGB_t blend(RGB_t color1, RGB_t color2, uint8_t factor)
{
    RGB_t result;
    result.r = (uint8_t)(((uint16_t)color1.r * (255 - factor) + (uint16_t)color2.r * factor) / 255);
    result.g = (uint8_t)(((uint16_t)color1.g * (255 - factor) + (uint16_t)color2.g * factor) / 255);
    result.b = (uint8_t)(((uint16_t)color1.b * (255 - factor) + (uint16_t)color2.b * factor) / 255);
    return result;
}

// Simple pseudo-random generator (linear congruential)
static uint32_t rand_seed = 123456; // Fixed seed for reproducibility
uint32_t simple_rand(void) 
{
    rand_seed = (rand_seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return rand_seed;
}

uint32_t ranged_rand(uint32_t min, uint32_t max) 
{
    uint32_t range = max - min + 1;
    return min + (simple_rand() % range);
}

// Function to generate a smoothed random value
int32_t smoothed_rand(int32_t min, int32_t max, int32_t max_delta, int32_t *last_value) 
{
    // Ensure valid input ranges
    if (min >= max || max_delta <= 0) {
        return min; // Fallback to the minimum value
    }

    // Generate a pseudo-random delta within the range [-max_delta, max_delta]
    int32_t delta = (simple_rand() % (max_delta * 2 + 1)) - max_delta;

    // Calculate the new value
    int32_t new_value = *last_value + delta;

    // Clamp the value to stay within the min and max range
    if (new_value < min) new_value = min;
    if (new_value > max) new_value = max;

    // Update the last value
    *last_value = new_value;

    return new_value;
}

RGB_t get_random_color(void)
{
    return hsv_to_rgb((HSV_t) {
        .h = ranged_rand(0, 360),
        .s = ranged_rand(127, 255),
        .v = ranged_rand(127, 255),
    });
}
