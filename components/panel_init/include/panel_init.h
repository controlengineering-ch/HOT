#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Alustaa SPI-näytön (ST7789, 172x320) ja kytkee sen LVGL:ään.
void panel_init(void);

// Yhteensopivuus-API: sama kuin panel_init().
void panel_init_default(void);

// Aseta taustavalon kirkkaus 0–100 %.
void panel_backlight_set(int percent);

#ifdef __cplusplus
}
#endif

