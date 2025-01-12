#include "hue_scroll.h"

struct hue_scroll_t {
    uint32_t pixels_count;
};

uint32_t hue_scroll_get_handle_size(hue_scroll_init_t cfg)
{
    (void) cfg; // no used, to not produce Warning
    return sizeof(hue_scroll_t);
}

void hue_scroll_init(void *hndl_ptr, hue_scroll_init_t cfg)
{
    hue_scroll_handle_t hndl = (hue_scroll_handle_t) hndl_ptr;
    hndl->pixels_count = cfg.pixels_amount;
}

void hue_scroll_render(RGB_t *leds, uint32_t current_time_ms, void *hndl_ptr)
{
    hue_scroll_handle_t hndl = (hue_scroll_handle_t) hndl_ptr;

    const uint16_t hue_max = 360;
    const uint16_t colorCycleInterval = 3000; // ms

    uint16_t periodical_time = (current_time_ms % colorCycleInterval);
    uint16_t hue_component = (uint16_t)(periodical_time * hue_max / colorCycleInterval);

    for (size_t i = 0; i < hndl->pixels_count; i++) {
        uint16_t delta_each_pixel = hue_max / 6 / hndl->pixels_count;
        hue_component += delta_each_pixel;
        while (hue_component > hue_max) { hue_component -= hue_max; }
        HSV_t hsv_color = { .h = hue_component, .s = 0xff, .v = 0xff};
        RGB_t rgb_color = hsv_to_rgb(hsv_color);
        leds[i] = rgb_color;
    }
}
