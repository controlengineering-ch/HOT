#include "lvgl.h"
#include <stdio.h>

static lv_obj_t *label;

void lvgl_display_init(void) {
    lv_obj_t *scr = lv_scr_act();
    label = lv_label_create(scr);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label, "RadonSafe");
}

void lvgl_display_update(float radon_value) {
    char buf[64];
    snprintf(buf, sizeof(buf), "Radon: %.1f Bq/m³", radon_value);
    lv_label_set_text(label, buf);

    lv_color_t bg_color;
    if (radon_value < 150) {
        bg_color = lv_color_hex(0x2ecc71);  // green
    } else if (radon_value < 220) {
        bg_color = lv_color_hex(0xf1c40f);  // yellow
    } else {
        bg_color = lv_color_hex(0xe74c3c);  // red
    }

    lv_obj_set_style_bg_color(lv_scr_act(), bg_color, LV_PART_MAIN);
}

