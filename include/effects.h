#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "colors.h"
#include "utils.h"

#include "hue_scroll.h"
#include "fire.h"
#include "meteors.h"
#include "explosions.h"

typedef enum {
    EFFECT_TYPE_HUE_SCROLL = 0,
    EFFECT_TYPE_RAINBOW,
    EFFECT_TYPE_FIRE,
    EFFECT_TYPE_METEORS,
    EFFECT_TYPE_EXPLOSIONS,
    EFFECT_TYPE_COUNT,
} effect_type_t;

typedef union {
    hue_scroll_init_t hue_scroll;
    fire_init_t fire;
    meteors_init_t meteors;
    explosions_init_t explosions;
} effects_init_t;

uint32_t effect_get_handle_size(effect_type_t t, effects_init_t cfg);
void effect_init(effect_type_t t, void *hndl, effects_init_t cfg);
void effect_render(effect_type_t t, RGB_t *leds, uint32_t current_time_ms, void *hndl);
