#pragma once 

#include <stdint.h>
#include <string.h>

#include "colors.h"
#include "interface.h"

uint16_t map_range(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
RGB_t blend(RGB_t color1, RGB_t color2, uint8_t factor);

uint32_t ranged_rand(uint32_t min, uint32_t max);
int32_t smoothed_rand(int32_t min, int32_t max, int32_t max_delta, int32_t *last_value);
