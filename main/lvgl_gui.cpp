#include "lvgl_gui.h"

static lv_obj_t* label_sensor_id;
static lv_obj_t* label_radiation;
static lv_obj_t* label_status;
static lv_obj_t* label_legend;

void lvgl_gui_init() {
    lv_obj_t* scr = lv_scr_act();

    // Header
    lv_obj_t* header = lv_label_create(scr);
    lv_label_set_text(header, "RadonSafe v1.0");
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 5);

    // Sensor ID
    label_sensor_id = lv_label_create(scr);
    lv_label_set_text(label_sensor_id, "Sensor: -");
    lv_obj_align(label_sensor_id, LV_ALIGN_TOP_LEFT, 10, 30);

    // Radiation value
    label_radiation = lv_label_create(scr);
    lv_label_set_text(label_radiation, "Radon: --- Bq/m³");
    lv_obj_align(label_radiation, LV_ALIGN_TOP_LEFT, 10, 55);

    // Status
    label_status = lv_label_create(scr);
    lv_label_set_text(label_status, "Status: Unknown");
    lv_obj_align(label_status, LV_ALIGN_TOP_LEFT, 10, 80);

    // Legend
    label_legend = lv_label_create(scr);
    lv_label_set_text(label_legend, "🟡 YELLOW = Warning\n🔴 RED = Critical!");
    lv_obj_align(label_legend, LV_ALIGN_BOTTOM_LEFT, 10, -10);
}

void update_sensor_id(const char* id) {
    static char buffer[64];
    snprintf(buffer, sizeof(buffer), "Sensor: %s", id);
    lv_label_set_text(label_sensor_id, buffer);
}

void update_radiation_value(int bq_value) {
    static char buffer[64];
    snprintf(buffer, sizeof(buffer), "Radon: %d Bq/m³", bq_value);
    lv_label_set_text(label_radiation, buffer);
}

void update_status(StatusLevel level) {
    const char* status_text;
    lv_color_t text_color;

    switch (level) {
        case StatusLevel::NORMAL:
            status_text = "Status: Normal";
            text_color = lv_color_white();
            break;
        case StatusLevel::WARNING:
            status_text = "Status: Warning";
            text_color = lv_color_yellow();
            break;
        case StatusLevel::CRITICAL:
            status_text = "Status: CRITICAL!";
            text_color = lv_color_red();
            break;
    }

    lv_label_set_text(label_status, status_text);
    lv_obj_set_style_text_color(label_status, text_color, 0);
}

