#include "lvgl_gui.h"
#include <lvgl.h>
#include <stdio.h>

static lv_obj_t *radon_label;
static lv_obj_t *sensor_label;
static lv_obj_t *status_label;

void init_lvgl()
{
    lv_init();

    // Näytön ajuri alustetaan muualla (esim. BSP tai IDF:n lvgl_driver)
    lv_obj_t *scr = lv_scr_act();

    radon_label = lv_label_create(scr);
    lv_obj_align(radon_label, LV_ALIGN_TOP_MID, 0, 20);
    lv_label_set_text(radon_label, "Radon: -- Bq/m³");

    sensor_label = lv_label_create(scr);
    lv_obj_align(sensor_label, LV_ALIGN_TOP_MID, 0, 50);
    lv_label_set_text(sensor_label, "Sensor: -----");

    status_label = lv_label_create(scr);
    lv_obj_align(status_label, LV_ALIGN_TOP_MID, 0, 80);
    lv_label_set_text(status_label, "Status: OK");
}

void update_gui_with_data(const SensorData &data, const RadonAlert &alert)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "Radon: %d Bq/m³", data.radon);
    lv_label_set_text(radon_label, buf);

    snprintf(buf, sizeof(buf), "Sensor: %s", data.identifier.c_str());
    lv_label_set_text(sensor_label, buf);

    if (alert.level == ALERT_NONE) {
        lv_label_set_text(status_label, "Status: Normal");
        lv_obj_set_style_text_color(status_label, lv_color_white(), LV_PART_MAIN);
    } else if (alert.level == ALERT_WARN) {
        lv_label_set_text(status_label, "Status: Elevated");
        lv_obj_set_style_text_color(status_label, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_MAIN);
    } else {
        lv_label_set_text(status_label, "Status: Danger!");
        lv_obj_set_style_text_color(status_label, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
    }

    lv_timer_handler(); // Päivitetään LVGL-kehys
}

