#include "fire.h"

enum {
    PARAM_HUE,
    PARAM_SAT,
    PARAM_VAL,
    PARAM_COUNT
};
#define HSV_PARAMS PARAM_COUNT

typedef struct {
    int32_t noise[HSV_PARAMS];
    RGB_t render_color[2];
} fire_pixel_buf_t;

struct fire_t {
    uint16_t min_max_pair[HSV_PARAMS][2];
    uint32_t pixels_count;
    fire_pixel_buf_t pixel_buf[];
};

static RGB_t generate_fire_color(fire_handle_t hndl, fire_pixel_buf_t *pixel_buf)
{
    uint16_t min = hndl->min_max_pair[PARAM_HUE][0];
    uint16_t max = hndl->min_max_pair[PARAM_HUE][1];
    uint16_t hue = smoothed_rand(min, max, (max - min)/3, &pixel_buf->noise[PARAM_HUE]);

    min = hndl->min_max_pair[PARAM_SAT][0];
    max = hndl->min_max_pair[PARAM_SAT][1];
    uint8_t sat = smoothed_rand(min, max, (max - min)/3, &pixel_buf->noise[PARAM_SAT]);

    min = hndl->min_max_pair[PARAM_VAL][0];
    max = hndl->min_max_pair[PARAM_VAL][1];
    uint8_t val = smoothed_rand(min, max, (max - min)/3, &pixel_buf->noise[PARAM_VAL]);
    
    return hsv_to_rgb((HSV_t) {
        .h = hue,
        .s = sat,
        .v = val,
    });
}

uint32_t fire_get_handle_size(fire_init_t cfg)
{
    return sizeof(fire_t) + (sizeof(fire_pixel_buf_t) * cfg.pixels_amount);
}

void fire_init(void *hndl_ptr, fire_init_t cfg)
{
    fire_handle_t hndl = (fire_handle_t) hndl_ptr;
    hndl->pixels_count = cfg.pixels_amount;

    switch (cfg.type) {
        default:
        case FIRE_TYPE_WARM:
            hndl->min_max_pair[PARAM_HUE][0] = 15;
            hndl->min_max_pair[PARAM_HUE][1] = 60;
            hndl->min_max_pair[PARAM_SAT][0] = 240;
            hndl->min_max_pair[PARAM_SAT][1] = 255;
            hndl->min_max_pair[PARAM_VAL][0] = 255;
            hndl->min_max_pair[PARAM_VAL][1] = 255;
            break;

        case FIRE_TYPE_COLD:
            hndl->min_max_pair[PARAM_HUE][0] = 196;
            hndl->min_max_pair[PARAM_HUE][1] = 265;
            hndl->min_max_pair[PARAM_SAT][0] = 240;
            hndl->min_max_pair[PARAM_SAT][1] = 255;
            hndl->min_max_pair[PARAM_VAL][0] = 255;
            hndl->min_max_pair[PARAM_VAL][1] = 255;
            break;

        case FIRE_TYPE_DISCO:
            hndl->min_max_pair[PARAM_HUE][0] = 0;
            hndl->min_max_pair[PARAM_HUE][1] = 360;
            hndl->min_max_pair[PARAM_SAT][0] = 255;
            hndl->min_max_pair[PARAM_SAT][1] = 255;
            hndl->min_max_pair[PARAM_VAL][0] = 255;
            hndl->min_max_pair[PARAM_VAL][1] = 255;
            break;
    }
}

void fire_render(RGB_t *out, uint32_t current_time_ms, void *hndl_ptr)
{
    const uint8_t frame_period = 1000 / effects_get_fps();
    const uint16_t render_period = (200 / frame_period);
    uint32_t frames_untill_render = (current_time_ms / frame_period) % render_period;

    fire_handle_t hndl = (fire_handle_t) hndl_ptr;
    fire_pixel_buf_t *pixel_buf = (fire_pixel_buf_t *)hndl->pixel_buf;

    if (frames_untill_render != 0) {
        for (uint32_t i = 0; i < hndl->pixels_count; i++) {
            uint8_t factor = map_range(frames_untill_render, 0, render_period, 0, 255);
            out[i] = blend(pixel_buf[i].render_color[0], pixel_buf[i].render_color[1], factor);
        }
    } else {
        for (uint32_t i = 0; i < hndl->pixels_count; i++) {
            out[i] = pixel_buf[i].render_color[1];
            pixel_buf[i].render_color[0] = pixel_buf[i].render_color[1];
            pixel_buf[i].render_color[1] = generate_fire_color(hndl, &pixel_buf[i]);
        }
    }
}