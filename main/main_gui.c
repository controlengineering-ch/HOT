#include "lvgl.h"
#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char *TAG = "main_gui";

static lv_obj_t *label_title;
static lv_obj_t *label_value;
static lv_obj_t *label_status;

static void set_status_color(const char *state) {
    lv_color_t col = lv_palette_main(LV_PALETTE_GREY);
    if (strcmp(state, "OK") == 0) col = lv_palette_main(LV_PALETTE_GREEN);
    else if (strcmp(state, "WARN") == 0) col = lv_palette_main(LV_PALETTE_AMBER);
    else if (strcmp(state, "CRIT") == 0) col = lv_palette_main(LV_PALETTE_RED);
    lv_obj_set_style_text_color(label_status, col, 0);
}

void main_gui_create(void) {
    lv_obj_t *scr = lv_scr_act();
    label_title = lv_label_create(scr);
    lv_label_set_text(label_title, "RadonSafe");
    lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 8);

    label_value = lv_label_create(scr);
    lv_label_set_text(label_value, "— Bq/m³");
    lv_obj_align(label_value, LV_ALIGN_CENTER, 0, -10);
    lv_obj_set_style_text_font(label_value, &lv_font_montserrat_24, 0);

    label_status = lv_label_create(scr);
    lv_label_set_text(label_status, "STATE: INIT");
    lv_obj_align(label_status, LV_ALIGN_CENTER, 0, 24);
    set_status_color("OK");
}

static const char *classify_status(int value, int last_value) {
    if (value >= 220) return "CRIT";
    if (value >= 150) return "WARN";
    if (last_value > 0 && (value - last_value) * 100 / last_value >= 30) return "WARN";
    return "OK";
}

bool main_gui_update_from_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { ESP_LOGW(TAG, "open failed: %s", path); return false; }

    char line[128] = {0};
    if (!fgets(line, sizeof line, f)) { fclose(f); return false; }
    fclose(f);

    // Parse first integer appearing in line
    int value = -1;
    for (char *p = line; *p; ++p) {
        if (*p >= '0' && *p <= '9') { value = atoi(p); break; }
    }
    static int last_value = -1;
    if (value >= 0) {
        char buf[64];
        snprintf(buf, sizeof buf, "%d Bq/m³", value);
        lv_label_set_text(label_value, buf);
        const char *state = classify_status(value, last_value);
        last_value = value;
        char sbuf[32]; snprintf(sbuf, sizeof sbuf, "STATE: %s", state);
        lv_label_set_text(label_status, sbuf);
        set_status_color(state);
        return true;
    }
    return false;
}

