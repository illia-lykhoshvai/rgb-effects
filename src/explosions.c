#include "explosions.h"

#define NO_POS UINT32_MAX

typedef struct {
    uint32_t birth_time;
    uint16_t blend_factor;
    RGB_t color;
    uint32_t pos;
} explosion_t;

struct explosions_t {
    uint32_t active_items;
    uint32_t last_birth_time;
    // init:
    uint32_t width;
    uint32_t speed;
    uint32_t birth_delta;
    bool is_color_random;
    RGB_t bg_color;
    uint32_t explosions_max_num; // simultaneous
    // buffer for every meteor:
    explosion_t explosion[];
};

uint32_t explosions_get_handle_size(explosions_init_t cfg)
{
    return sizeof(explosions_t) + sizeof(explosion_t) * cfg.explosions_max_num;
}

void explosions_init(void *hndl_ptr, explosions_init_t cfg)
{
    explosions_handle_t hndl = (explosions_handle_t) hndl_ptr;
    hndl->active_items = 0;
    hndl->last_birth_time = 0;
    hndl->speed = cfg.frames_for_animation;
    hndl->birth_delta = cfg.birth_delta;
    hndl->width = cfg.width;
    hndl->bg_color = cfg.c.bg_color;
    hndl->is_color_random = cfg.c.random;
    hndl->explosions_max_num = cfg.explosions_max_num;
    for(uint32_t i = 0; i < hndl->explosions_max_num; i++) {
        explosion_t *e = &hndl->explosion[i];
        e->blend_factor = 0;
        e->pos = NO_POS;
        if (!hndl->is_color_random) {
            e->color = cfg.c.explosions_color;
        }
    }
}

static bool check_if_pos_is_free(explosions_handle_t hndl, uint32_t pos)
{
    for(uint32_t i = 0; i < hndl->explosions_max_num; i++) {
        if (hndl->explosion[i].pos == pos) {
            return false;
        }
    }
    return true;
}

static void explosion_create(explosions_handle_t hndl, explosion_t *e)
{
    e->blend_factor = 0;

    if (hndl->is_color_random) {
        e->color = effects_rand_rgb();
    }

    uint32_t new_pos = 0;
    do {
        new_pos = ranged_rand(0, hndl->width);
    } while (check_if_pos_is_free(hndl, new_pos) != true);
    e->pos = new_pos;
}

void explosions_render(RGB_t *out, uint32_t current_time_ms, void *hndl_ptr)
{
    explosions_handle_t hndl = (explosions_handle_t) hndl_ptr;

    for (uint32_t i = 0; i < hndl->width; i++) {
        out[i] = hndl->bg_color;
    }

    for (uint32_t i = 0; i < hndl->explosions_max_num; i++) {
        explosion_t *e = &hndl->explosion[i];

        bool enough_time_from_last_birth = (current_time_ms - hndl->last_birth_time) > hndl->birth_delta;
        bool need_birth = (ranged_rand(0, 100) < 10) && (hndl->active_items < hndl->explosions_max_num) && enough_time_from_last_birth;

        if (need_birth && (e->pos == NO_POS)) {
            e->birth_time = current_time_ms;
            explosion_create(hndl, e);
            hndl->last_birth_time = e->birth_time;
            hndl->active_items++;
        }

        if (e->pos == NO_POS) {
            continue;
        }

        // calculate blend
        const uint16_t full_animation_blend = UINT8_MAX * 2;
        uint32_t animation_period = 1000 / effects_get_fps();
        uint32_t explosion_timestamp = current_time_ms - e->birth_time;
        uint32_t frames_left = (explosion_timestamp / animation_period) % hndl->speed;
        e->blend_factor = map_range(frames_left, 0, hndl->speed - 1, 0, full_animation_blend);
        
        // draw explosion
        if (e->pos < hndl->width) {
            uint8_t real_blend = e->blend_factor > UINT8_MAX ? UINT8_MAX - (e->blend_factor - UINT8_MAX) : e->blend_factor;
            out[e->pos] = blend(hndl->bg_color, e->color, real_blend);
        }

        if (e->blend_factor == full_animation_blend) {
            // deinit:
            e->pos = NO_POS;
            hndl->active_items--;
        }
    }
}
