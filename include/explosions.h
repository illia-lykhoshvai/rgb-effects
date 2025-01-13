#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "colors.h"
#include "utils.h"

typedef struct explosions_t explosions_t;
typedef explosions_t *explosions_handle_t;

typedef struct {
    RGB_t bg_color; // obligatory
    bool random; // if true -> each explosion will have random color
    RGB_t explosions_color; // if false -> all explosions will have this color
} explosions_colors_t;

typedef struct {
    uint32_t width;
    uint32_t frames_for_animation;
    uint32_t birth_delta;
    uint32_t explosions_max_num; // simultaneous
    explosions_colors_t c;
} explosions_init_t;

uint32_t explosions_get_handle_size(explosions_init_t cfg);
void explosions_init(void *hndl_ptr, explosions_init_t cfg);
void explosions_render(RGB_t *out, uint32_t current_time_ms, void *hndl_ptr);
