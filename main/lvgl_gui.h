#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "lvgl.h"
#ifdef __cplusplus
}
#endif

// Yhtenäinen status-enum kaikkialle projektiin
enum StatusLevel {
    STATUS_OK = 0,
    STATUS_WARNING = 1,
    STATUS_CRITICAL = 2
};

// Alusta yksinkertainen GUI (otsikko + statuslabel)
void lvgl_gui_init(void);

// C-rajapinta: päivitys kokonaislukuna (esim. C:stä tai shellistä)
#ifdef __cplusplus
extern "C" {
#endif
void gui_update_status(int level);
#ifdef __cplusplus
}
#endif

// C++-rajapinta: suora enum
#ifdef __cplusplus
void update_status(StatusLevel level);
#endif

