#include "panel_init.h"
#include "lvgl.h"

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_timer.h"
#include "esp_check.h"
#include "esp_log.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

static const char *TAG = "panel_init";

/* --- Muokattavat oletukset (ylikirjoita projektissa tarpeen mukaan) --- */
#ifndef LCD_H_RES
#define LCD_H_RES        172
#endif
#ifndef LCD_V_RES
#define LCD_V_RES        320
#endif

#ifndef LCD_SPI_HOST
#define LCD_SPI_HOST     SPI2_HOST
#endif
#ifndef LCD_PIN_MOSI
#define LCD_PIN_MOSI     6
#endif
#ifndef LCD_PIN_SCLK
#define LCD_PIN_SCLK     5
#endif
#ifndef LCD_PIN_CS
#define LCD_PIN_CS       4
#endif
#ifndef LCD_PIN_DC
#define LCD_PIN_DC       7
#endif
#ifndef LCD_PIN_RST
#define LCD_PIN_RST      8
#endif
#ifndef LCD_PIN_BL
#define LCD_PIN_BL       9
#endif

#ifndef LCD_COL_OFFSET
#define LCD_COL_OFFSET   0
#endif
#ifndef LCD_ROW_OFFSET
#define LCD_ROW_OFFSET   0
#endif

#ifndef LVGL_TICK_MS
#define LVGL_TICK_MS     2
#endif

/* Montako viivaa piirretään kerrallaan (osittainen renderöinti) */
#ifndef LCD_DRAW_BUF_LINES
#define LCD_DRAW_BUF_LINES  40
#endif

/* Taustavalon ohjaus (1=LEDC PWM, 0=GPIO ON/OFF) */
#ifndef LCD_BL_USE_PWM
#define LCD_BL_USE_PWM    1
#endif
#if LCD_BL_USE_PWM
#ifndef LCD_BL_LEDC_TIMER
#define LCD_BL_LEDC_TIMER   LEDC_TIMER_0
#endif
#ifndef LCD_BL_LEDC_MODE
#define LCD_BL_LEDC_MODE    LEDC_LOW_SPEED_MODE
#endif
#ifndef LCD_BL_LEDC_CHANNEL
#define LCD_BL_LEDC_CHANNEL LEDC_CHANNEL_0
#endif
#ifndef LCD_BL_LEDC_FREQ_HZ
#define LCD_BL_LEDC_FREQ_HZ 1000
#endif
#ifndef LCD_BL_LEDC_DUTY_RES
#define LCD_BL_LEDC_DUTY_RES LEDC_TIMER_10_BIT  // 0..1023
#endif
#endif

/* --- Pysyvät kahvat --- */
static bool s_lvgl_inited = false;
static esp_timer_handle_t s_lvgl_tick_timer = NULL;

static esp_lcd_panel_handle_t s_panel = NULL;

/* LVGL v9: display-kahva + raakapuskurit */
static lv_display_t *s_disp = NULL;
static lv_color_t *s_buf1 = NULL;
static lv_color_t *s_buf2 = NULL;

/* LVGL v9: flush-callback */
static void lvgl_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    esp_err_t err = esp_lcd_panel_draw_bitmap(
        s_panel, area->x1, area->y1, area->x2 + 1, area->y2 + 1, px_map);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "panel_draw_bitmap failed: %d", err);
    }
    lv_display_flush_ready(disp);
}

/* LVGL tick-ajastin */
static void lvgl_tick_cb(void *arg)
{
    (void)arg;
    lv_tick_inc(LVGL_TICK_MS);
}

/* SPI-paneelin (ST7789) alustaminen */
static esp_err_t init_spi_panel(void)
{
    spi_bus_config_t buscfg = {
        .mosi_io_num = LCD_PIN_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = LCD_PIN_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * LCD_DRAW_BUF_LINES * sizeof(lv_color_t) + 8,
        .flags = SPICOMMON_BUSFLAG_MASTER
    };
    ESP_RETURN_ON_ERROR(spi_bus_initialize(LCD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO), TAG, "spi init");

    esp_lcd_panel_io_handle_t io = NULL;
    esp_lcd_panel_io_spi_config_t io_cfg = {
        .cs_gpio_num = LCD_PIN_CS,
        .dc_gpio_num = LCD_PIN_DC,
        .spi_mode = 0,
        .pclk_hz = 40 * 1000 * 1000,      // tarvittaessa pienennä (esim. 26 MHz) jos häiriöitä
        .trans_queue_depth = 10,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        // ei .flags-jäseniä tässä IDF-yhdistelmässä
    };
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_HOST, &io_cfg, &io), TAG, "panel io");

    esp_lcd_panel_dev_config_t panel_cfg = {
        .reset_gpio_num = LCD_PIN_RST,
        .color_space = ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = 16,
        .flags = { .reset_active_high = 0 },
        .vendor_config = NULL
    };
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_st7789(io, &panel_cfg, &s_panel), TAG, "new st7789");

    ESP_RETURN_ON_ERROR(esp_lcd_panel_reset(s_panel), TAG, "reset");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_init(s_panel), TAG, "init");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_invert_color(s_panel, true), TAG, "invert");

    if (LCD_COL_OFFSET || LCD_ROW_OFFSET) {
        ESP_RETURN_ON_ERROR(esp_lcd_panel_set_gap(s_panel, LCD_COL_OFFSET, LCD_ROW_OFFSET), TAG, "gap");
    }
    return ESP_OK;
}

/* Taustavalon ohjaus */
static void bl_init_if_needed(void)
{
#if LCD_BL_USE_PWM
    static bool pwm_init = false;
    if (!pwm_init && LCD_PIN_BL >= 0) {
        ledc_timer_config_t tcfg = {
            .speed_mode = LCD_BL_LEDC_MODE,
            .duty_resolution = LCD_BL_LEDC_DUTY_RES,
            .timer_num = LCD_BL_LEDC_TIMER,
            .freq_hz = LCD_BL_LEDC_FREQ_HZ,
            .clk_cfg = LEDC_AUTO_CLK
        };
        ESP_ERROR_CHECK(ledc_timer_config(&tcfg));
        ledc_channel_config_t ccfg = {
            .gpio_num = LCD_PIN_BL,
            .speed_mode = LCD_BL_LEDC_MODE,
            .channel = LCD_BL_LEDC_CHANNEL,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LCD_BL_LEDC_TIMER,
            .duty = 0,
            .hpoint = 0
        };
        ESP_ERROR_CHECK(ledc_channel_config(&ccfg));
        pwm_init = true;
    }
#else
    static bool bl_gpio_init = false;
    if (!bl_gpio_init && LCD_PIN_BL >= 0) {
        gpio_config_t io_conf = {
            .pin_bit_mask = 1ULL << LCD_PIN_BL,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = 0,
            .pull_down_en = 0,
            .intr_type = GPIO_INTR_DISABLE
        };
        gpio_config(&io_conf);
        bl_gpio_init = true;
    }
#endif
}

void panel_backlight_set(int percent)
{
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    bl_init_if_needed();
#if LCD_BL_USE_PWM
    if (LCD_PIN_BL >= 0) {
        int maxd = (1 << LCD_BL_LEDC_DUTY_RES) - 1;
        int duty = (percent * maxd) / 100;
        ledc_set_duty(LCD_BL_LEDC_MODE, LCD_BL_LEDC_CHANNEL, duty);
        ledc_update_duty(LCD_BL_LEDC_MODE, LCD_BL_LEDC_CHANNEL);
    }
#else
    if (LCD_PIN_BL >= 0) gpio_set_level(LCD_PIN_BL, percent > 0 ? 1 : 0);
#endif
}

void panel_init(void)
{
    if (!s_lvgl_inited) {
        lv_init();
        s_lvgl_inited = true;

        const esp_timer_create_args_t tcfg = {
            .callback = &lvgl_tick_cb,
            .arg = NULL,
            .dispatch_method = ESP_TIMER_TASK,
            .name = "lv_tick"
        };
        ESP_ERROR_CHECK(esp_timer_create(&tcfg, &s_lvgl_tick_timer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(s_lvgl_tick_timer, LVGL_TICK_MS * 1000));
        ESP_LOGI(TAG, "LVGL initialized, tick=%d ms", LVGL_TICK_MS);
    }

    ESP_ERROR_CHECK(init_spi_panel());

    /* LVGL v9: luo display, flush-cb ja puskurit */
    s_disp = lv_display_create(LCD_H_RES, LCD_V_RES);
    lv_display_set_flush_cb(s_disp, lvgl_disp_flush);

    size_t buf_bytes = LCD_H_RES * LCD_DRAW_BUF_LINES * sizeof(lv_color_t);
    s_buf1 = (lv_color_t *)heap_caps_malloc(buf_bytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    s_buf2 = (lv_color_t *)heap_caps_malloc(buf_bytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    ESP_ERROR_CHECK_WITHOUT_ABORT((s_buf1 && s_buf2) ? ESP_OK : ESP_ERR_NO_MEM);

    /* Raakapuskurit LVGL:lle */
    lv_display_set_buffers(s_disp, s_buf1, s_buf2, buf_bytes, LV_DISPLAY_RENDER_MODE_PARTIAL);

    panel_backlight_set(100);
    ESP_LOGI(TAG, "Panel initialized: ST7789 %dx%d (SPI, LVGL v9)", LCD_H_RES, LCD_V_RES);
}

void panel_init_default(void)
{
    panel_init();
}

