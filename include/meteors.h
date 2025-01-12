#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "colors.h"
#include "utils.h"

typedef struct meteors_t meteors_t;
typedef meteors_t *meteors_handle_t;

typedef enum {
    METEORS_DIRECTION_RIGHT,
    METEORS_DIRECTION_LEFT,
    METEORS_DIRECTION_RANDOM,
} meteors_direction_t;

typedef struct {
    RGB_t bg_color; // obligatory
    bool random; // if true -> each meteor will have random color
    RGB_t meteors_color; // if false -> all meteors will have this color
} meteors_colors_t;

typedef struct {
    uint32_t meteors_cnt;
    uint32_t width;
    uint32_t tail_min;
    uint32_t tail_max;
    meteors_direction_t dir;
    meteors_colors_t c;
} meteors_init_t;

uint32_t meteors_get_handle_size(meteors_init_t cfg);
void meteors_init(void *hndl_ptr, meteors_init_t cfg);
void meteors_render(RGB_t *out, uint32_t current_time_ms, void *hndl_ptr);
