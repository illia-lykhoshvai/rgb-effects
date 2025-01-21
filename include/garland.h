#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "colors.h"
#include "utils.h"

typedef struct garland_t garland_t;
typedef garland_t *garland_handle_t;

typedef struct { // obligatory
    RGB_t bg_color;
    RGB_t main_color;
} garland_colors_t;

typedef struct {
    uint32_t width;
    garland_colors_t c;
    uint32_t ms_per_group;
    uint32_t pixels_in_group;
} garland_init_t;

uint32_t garland_get_handle_size(garland_init_t cfg);
void garland_init(void *hndl, garland_init_t cfg);
void garland_render(RGB_t *out, uint32_t current_time_ms, void *hndl_ptr);
