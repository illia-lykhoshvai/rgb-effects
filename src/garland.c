#include "garland.h"

struct garland_t {
    // init:
    uint32_t width;
    garland_colors_t c;
    uint32_t ms_per_group;
    uint32_t pixels_in_group;
    // others:
};

uint32_t garland_get_handle_size(const garland_init_t *cfg)
{
    (void) cfg; // not used, just to supress the warning
    return sizeof(garland_t);
}
void garland_init(void *hndl_ptr, const garland_init_t *cfg)
{
    garland_handle_t hndl = (garland_handle_t) hndl_ptr;
    hndl->width = cfg->width;
    hndl->c = cfg->c;
    hndl->ms_per_group = cfg->ms_per_group;
    hndl->pixels_in_group = cfg->pixels_in_group;
}

void garland_render(void *hndl_ptr, RGB_t *out, uint32_t current_time_ms)
{
    garland_handle_t hndl = (garland_handle_t) hndl_ptr;

    for (uint32_t i = 0; i < hndl->width; i++) {
        uint32_t time_per_group = hndl->ms_per_group;
        uint32_t pixels_in_group = hndl->pixels_in_group;
        bool main_color_pixel = (current_time_ms % (time_per_group*2)) > time_per_group;
        main_color_pixel ^= (i % pixels_in_group) >= pixels_in_group/2;
        uint32_t time_before_change = time_per_group - (current_time_ms % time_per_group);
        uint8_t factor = map_range(time_before_change, 0, time_per_group, 0, 255);
        if (main_color_pixel) {
            out[i] = blend(hndl->c.main_color, hndl->c.bg_color, factor);
        } else {
            out[i] = blend(hndl->c.bg_color, hndl->c.main_color, factor);
        }
    }
}