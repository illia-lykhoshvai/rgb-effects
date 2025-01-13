#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "colors.h"
#include "utils.h"

typedef struct hue_scroll_t hue_scroll_t;
typedef hue_scroll_t *hue_scroll_handle_t;

typedef struct {
    uint32_t pixels_amount;
    uint32_t color_cycle_interval_ms;
    bool is_rainbow;
} hue_scroll_init_t;

uint32_t hue_scroll_get_handle_size(hue_scroll_init_t cfg);
void hue_scroll_init(void *hndl, hue_scroll_init_t cfg);
void hue_scroll_render(RGB_t *leds, uint32_t current_time_ms, void *buf);
