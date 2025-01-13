#include "effects.h"

uint32_t effect_get_handle_size(effect_type_t t, effects_init_t cfg)
{
    switch (t) {
        case EFFECT_TYPE_HUE_SCROLL: 
        case EFFECT_TYPE_RAINBOW:
            return hue_scroll_get_handle_size(cfg.hue_scroll);

        case EFFECT_TYPE_FIRE: 
            return fire_get_handle_size(cfg.fire);

        case EFFECT_TYPE_METEORS: 
            return meteors_get_handle_size(cfg.meteors);

        case EFFECT_TYPE_EXPLOSIONS: 
            return explosions_get_handle_size(cfg.explosions);

        default:
        case EFFECT_TYPE_COUNT:
            return 0;
    }
}

void effect_init(effect_type_t t, void *hndl, effects_init_t cfg)
{
    switch (t) {
        case EFFECT_TYPE_HUE_SCROLL: 
        case EFFECT_TYPE_RAINBOW:
            hue_scroll_init(hndl, cfg.hue_scroll);
            break;

        case EFFECT_TYPE_FIRE: 
            fire_init(hndl, cfg.fire);
            break;

        case EFFECT_TYPE_METEORS: 
            meteors_init(hndl, cfg.meteors);
            break;

        case EFFECT_TYPE_EXPLOSIONS: 
            explosions_init(hndl, cfg.explosions);
            break;

        default:
        case EFFECT_TYPE_COUNT:
            break;
    }
}

void effect_render(effect_type_t t, RGB_t *leds, uint32_t current_time_ms, void *hndl)
{
    typedef void(*render_func_t)(RGB_t *leds, uint32_t current_time_ms, void *hndl);
    render_func_t f[EFFECT_TYPE_COUNT] = {
        [EFFECT_TYPE_HUE_SCROLL] = hue_scroll_render,
        [EFFECT_TYPE_RAINBOW] = hue_scroll_render,
        [EFFECT_TYPE_FIRE] = fire_render,
        [EFFECT_TYPE_METEORS] = meteors_render,
        [EFFECT_TYPE_EXPLOSIONS] = explosions_render,
    };

    if (t < EFFECT_TYPE_COUNT) {
        f[t](leds, current_time_ms, hndl);
    }
}
