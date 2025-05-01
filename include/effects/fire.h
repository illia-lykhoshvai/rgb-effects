#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "colors.h"
#include "utils.h"

typedef struct fire_t fire_t;
typedef fire_t *fire_handle_t;

typedef enum {
    FIRE_TYPE_WARM,
    FIRE_TYPE_COLD,
    FIRE_TYPE_DISCO,
} fire_type_t;

typedef struct {
    uint32_t pixels_amount;
    fire_type_t type;
} fire_init_t;

uint32_t fire_get_handle_size(const fire_init_t *cfg);
void fire_init(void *hndl_ptr, const fire_init_t *cfg);
void fire_render(void *hndl_ptr, RGB_t *out, uint32_t current_time_ms);
