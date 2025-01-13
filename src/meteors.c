#include "meteors.h"

#define NO_POS INT32_MAX

typedef struct {
    uint32_t head_pos;
    uint32_t tail;
    uint16_t speed; // num of frames to do 1 move
    uint8_t blend_factor;
    RGB_t color;
    bool dir_towards_end;
} meteor_t;

struct meteors_t {
    uint32_t active_items;
    uint32_t step_cnt;
    uint32_t prev_birth_step;
    uint32_t tail_min;
    uint32_t tail_max;
    // init:
    meteors_direction_t dir;
    uint32_t width;
    bool is_color_random;
    RGB_t bg_color;
    uint32_t max_items_num;
    // buffer for every meteor:
    meteor_t meteor[];
};

uint32_t meteors_get_handle_size(meteors_init_t cfg)
{
    return sizeof(meteors_t) + (cfg.meteors_cnt * sizeof(meteor_t));
}

void meteors_init(void *hndl_ptr, meteors_init_t cfg)
{
    meteors_handle_t hndl = (meteors_handle_t) hndl_ptr;
    hndl->width = cfg.width;
    hndl->dir = cfg.dir;
    hndl->tail_min = cfg.tail_min;
    hndl->tail_max = cfg.tail_max;
    
    hndl->is_color_random = cfg.c.random;
    hndl->bg_color = cfg.c.bg_color;

    hndl->max_items_num = cfg.meteors_cnt;
    for(uint32_t i = 0; i < cfg.meteors_cnt; i++) {
        hndl->meteor[i].head_pos = NO_POS;
        if (!hndl->is_color_random) {
            hndl->meteor[i].color = cfg.c.meteors_color;
        }
    }
    hndl->prev_birth_step = UINT32_MAX/2;
}

static void meteor_create(meteors_handle_t hndl, meteor_t *m)
{
    if (hndl->is_color_random) {
        m->color = effects_rand_rgb();
    }
    if (hndl->dir == METEORS_DIRECTION_RANDOM) {
        m->dir_towards_end = ranged_rand(0, 100) > 50;
    } else {
        m->dir_towards_end = hndl->dir == METEORS_DIRECTION_RIGHT;
    }
    m->head_pos = m->dir_towards_end ? 0 : (hndl->width - 1);
    m->blend_factor = 0;
    m->speed = ranged_rand(1, 10);
    m->tail = ranged_rand(hndl->tail_min, hndl->tail_max);
}

void meteors_render(RGB_t *out, uint32_t current_time_ms, void *hndl_ptr)
{
    meteors_handle_t hndl = (meteors_handle_t) hndl_ptr;

    for (uint32_t i = 0; i < hndl->width; i++) {
        out[i] = hndl->bg_color;
    }

    for (uint32_t i = 0; i < hndl->max_items_num; i++) {
        meteor_t *m = &hndl->meteor[i];

        bool need_birth = (ranged_rand(0, 100) < 10) && ((hndl->step_cnt - hndl->prev_birth_step) > m->tail);
        if (need_birth && (m->head_pos == NO_POS)) {
            meteor_create(hndl, m);
            hndl->prev_birth_step = hndl->step_cnt;
            hndl->active_items++;
        }

        if (m->head_pos == NO_POS) {
            continue;
        }
        
        // draw head
        if (m->head_pos < hndl->width) {
            out[m->head_pos] = m->color;
        }
        
        uint32_t next_pos = m->dir_towards_end ? (m->head_pos + 1) : (m->head_pos - 1);
        bool next_pos_valid = next_pos < hndl->width;

        if (next_pos_valid) {
            out[next_pos] = blend(m->color, hndl->bg_color, 255 - m->blend_factor);
        }

        // draw tail
        for (uint32_t j = 1; j <= m->tail; j++) {
            uint32_t led_index = m->dir_towards_end ? m->head_pos - j : m->head_pos + j;
            if (led_index >= hndl->width) {
                continue;
            }
            uint8_t blend_step = 255/m->tail;
            uint8_t min = blend_step*(m->tail - (j - 1));
            uint8_t max = blend_step*(m->tail - j);
            uint8_t factor = map_range(m->blend_factor, 0, 255, min, max);
            out[led_index] = blend(hndl->bg_color, m->color, factor);
        }

        // calculate step
        const uint8_t frame_period = 1000 / effects_get_fps();
        uint32_t frames_untill_move = (current_time_ms / frame_period) % m->speed;
        bool need_move = frames_untill_move == 0;

        if (!need_move) {
            m->blend_factor = map_range(frames_untill_move, 0, m->speed, 0, 255);
        } else {
            hndl->step_cnt++;
            m->blend_factor = 0;
            
            int32_t signed_next_pos = (int32_t) next_pos;
            if ((signed_next_pos < (int32_t)(hndl->width + m->tail)) && (signed_next_pos > (int32_t)(-m->tail))) {
                m->head_pos = next_pos;
            } else {
                m->head_pos = NO_POS;
            }
        }
    }
}
