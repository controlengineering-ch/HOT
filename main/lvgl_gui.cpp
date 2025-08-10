// lvgl_gui.cpp
#include "esp_log.h"
extern "C" {
#include "lvgl.h"
}
#include "lvgl_gui.h"
#include "esp_lvgl_port.h"

static const char* TAG = "lvgl_gui";

// UI elements
static lv_obj_t* status_label = nullptr;

void update_status(StatusLevel level);  // fwd

extern "C" void gui_update_status(int level) {
    if (level < STATUS_OK || level > STATUS_CRITICAL) {
        ESP_LOGW(TAG, "gui_update_status: out-of-range level=%d", level);
        return;
    }
    update_status(static_cast<StatusLevel>(level));
}

void lvgl_gui_init(void) {
    // LVGL-kutsut porttilukon sisällä
    lvgl_port_lock(0);

    lv_obj_t* scr = lv_scr_act();

    // Title
    lv_obj_t* title = lv_label_create(scr);
    lv_label_set_text(title, "RadonSafe");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 8);

    // Status label
    status_label = lv_label_create(scr);
    lv_label_set_text(status_label, "Status: OK");
    lv_obj_align(status_label, LV_ALIGN_TOP_LEFT, 10, 36);

    lvgl_port_unlock();
    update_status(STATUS_OK);
}

void update_status(StatusLevel level) {
    lvgl_port_lock(0);

    if (!status_label) {
        ESP_LOGW(TAG, "update_status before init; creating label");
        lv_obj_t* scr = lv_scr_act();
        status_label = lv_label_create(scr);
        lv_obj_align(status_label, LV_ALIGN_TOP_LEFT, 10, 36);
    }

    switch (level) {
        case STATUS_OK:
            lv_label_set_text(status_label, "Status: OK");
            break;
        case STATUS_WARNING:
            lv_label_set_text(status_label, "Status: WARNING");
            break;
        case STATUS_CRITICAL:
            lv_label_set_text(status_label, "Status: CRITICAL");
            break;
        default:
            lv_label_set_text(status_label, "Status: UNKNOWN");
            break;
    }

    lvgl_port_unlock();
    ESP_LOGI(TAG, "Status updated: %d", static_cast<int>(level));
}

