#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "colors.h"
#include "utils.h"
#include "interface.h" // LOOK INSIDE FOR EXTERN FUNCTIONS DEFINITION

#include "hue_scroll.h"
#include "fire.h"
#include "meteors.h"
#include "explosions.h"
#include "garland.h"

typedef enum {
    EFFECT_TYPE_HUE_SCROLL = 0,
    EFFECT_TYPE_RAINBOW,
    EFFECT_TYPE_FIRE,
    EFFECT_TYPE_METEORS,
    EFFECT_TYPE_EXPLOSIONS,
    EFFECT_TYPE_GARLAND,
    EFFECT_TYPE_COUNT,
} effect_type_t;

typedef union {
    hue_scroll_init_t hue_scroll;
    fire_init_t fire;
    meteors_init_t meteors;
    explosions_init_t explosions;
    garland_init_t garland;
} effects_init_t;

uint32_t effect_get_handle_size(effect_type_t t, const effects_init_t *cfg);
void effect_init(effect_type_t t, const effects_init_t *cfg, void *hndl);
void effect_render(void *hndl, effect_type_t t, RGB_t *leds, uint32_t current_time_ms);
