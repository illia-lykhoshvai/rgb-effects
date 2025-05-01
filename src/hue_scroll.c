#include "hue_scroll.h"

struct hue_scroll_t {
    uint32_t pixels_count;
    uint32_t color_cycle_interval_ms;
    bool is_rainbow;
};

uint32_t hue_scroll_get_handle_size(const hue_scroll_init_t *cfg)
{
    (void) cfg; // no used, to supress the warning
    return sizeof(hue_scroll_t);
}

void hue_scroll_init(void *hndl_ptr, const hue_scroll_init_t *cfg)
{
    hue_scroll_handle_t hndl = (hue_scroll_handle_t) hndl_ptr;
    hndl->pixels_count = cfg->pixels_amount;
    hndl->color_cycle_interval_ms = cfg->color_cycle_interval_ms;
    hndl->is_rainbow = cfg->is_rainbow;
}

void hue_scroll_render(void *hndl_ptr, RGB_t *leds, uint32_t current_time_ms) 
{
    hue_scroll_handle_t hndl = (hue_scroll_handle_t) hndl_ptr;

    const uint16_t hue_max = 360;                // Full hue range

    // Calculate the base hue component based on current time
    uint16_t periodical_time = (current_time_ms % hndl->color_cycle_interval_ms);
    uint16_t base_hue = (uint16_t)(periodical_time * hue_max / hndl->color_cycle_interval_ms);

    // Hue increment per pixel to create a smooth gradient
    uint16_t delta_each_pixel = hue_max / hndl->pixels_count;

    for (size_t i = 0; i < hndl->pixels_count; i++) {
        uint8_t pixel_variance = hndl->is_rainbow ? i : 1;
        // Calculate hue for the current pixel, wrapping around hue_max
        uint16_t hue_component = (base_hue + pixel_variance * delta_each_pixel) % hue_max;

        // Create HSV color and convert to RGB
        HSV_t hsv_color = { .h = hue_component, .s = 255, .v = 255 };
        RGB_t rgb_color = hsv_to_rgb(hsv_color);

        // Assign the calculated color to the current pixel
        leds[i] = rgb_color;
    }
}
