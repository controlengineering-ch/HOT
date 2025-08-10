#include "data_structs.h"

#include "esp_log.h"
#include "esp_err.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"           // sisältää myös SDSPI-API:t
#include "driver/spi_master.h"

#include <sys/stat.h>
#include <sys/unistd.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <ctime>

static const char* TAG = "sd_card";

/* === PINNIT JA SPI-HOST (muuta vastaamaan rautaa) ======================= */
#ifndef SD_SPI_HOST
#define SD_SPI_HOST        SPI2_HOST     // ESP32-C6: SPI2 on tyypillinen
#endif
#ifndef SD_PIN_MOSI
#define SD_PIN_MOSI        10
#endif
#ifndef SD_PIN_MISO
#define SD_PIN_MISO        11
#endif
#ifndef SD_PIN_SCLK
#define SD_PIN_SCLK        12
#endif
#ifndef SD_PIN_CS
#define SD_PIN_CS          13
#endif
/* ======================================================================== */

static sdmmc_card_t* s_card = nullptr;
static bool           s_mounted = false;
static std::string    s_mount_point = "/sdcard";

static esp_err_t ensure_dir(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        if (S_ISDIR(st.st_mode)) return ESP_OK;
        ESP_LOGE(TAG, "Path exists but is not a directory: %s", path);
        return ESP_FAIL;
    }
    if (mkdir(path, 0775) == 0) return ESP_OK;
    ESP_LOGE(TAG, "mkdir failed for %s", path);
    return ESP_FAIL;
}

static void format_date_ymd(char* out, size_t outlen, time_t t) {
    struct tm tm_info;
    gmtime_r(&t, &tm_info);
    strftime(out, outlen, "%Y%m%d", &tm_info);
}

/* ======= SDSPI-POHJAINEN MOUNT ======= */
bool sdcard_init(const char* mount_point /*= "/sdcard"*/, bool /*use_4bit = false*/)
{
    if (s_mounted) {
        ESP_LOGI(TAG, "SD already mounted at %s", s_mount_point.c_str());
        return true;
    }
    if (mount_point && *mount_point) s_mount_point = mount_point;
    ESP_ERROR_CHECK_WITHOUT_ABORT(ensure_dir(s_mount_point.c_str()));

    // VFS+FAT mount-asetukset
    esp_vfs_fat_mount_config_t mount_config = {};
    mount_config.format_if_mount_failed = false;
    mount_config.max_files = 8;
    mount_config.allocation_unit_size = 16 * 1024;

    // SPI-väylä käyttöön
    spi_bus_config_t buscfg = {
        .mosi_io_num = SD_PIN_MOSI,
        .miso_io_num = SD_PIN_MISO,
        .sclk_io_num = SD_PIN_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
        .flags = SPICOMMON_BUSFLAG_MASTER
    };
    esp_err_t err = spi_bus_initialize((spi_host_device_t)SD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_initialize failed: %s", esp_err_to_name(err));
        return false;
    }

    // SDSPI-isäntä ja laitteen konfig
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = (spi_host_device_t)SD_SPI_HOST;  // liitä oikeaan SPI-väylään

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_PIN_CS;
    slot_config.host_id = (spi_host_device_t)SD_SPI_HOST;

    // Mount SDSPI
    err = esp_vfs_fat_sdspi_mount(s_mount_point.c_str(), &host, &slot_config, &mount_config, &s_card);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_vfs_fat_sdspi_mount failed: %s", esp_err_to_name(err));
        spi_bus_free((spi_host_device_t)SD_SPI_HOST);
        s_mounted = false;
        return false;
    }

    sdmmc_card_print_info(stdout, s_card);
    s_mounted = true;
    ESP_LOGI(TAG, "Mounted SD (SDSPI) at %s [MOSI=%d MISO=%d SCLK=%d CS=%d]",
             s_mount_point.c_str(), SD_PIN_MOSI, SD_PIN_MISO, SD_PIN_SCLK, SD_PIN_CS);
    return true;
}

void sdcard_deinit()
{
    if (!s_mounted) return;
    esp_err_t err = esp_vfs_fat_sdcard_unmount(s_mount_point.c_str(), s_card);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "unmount failed: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Unmounted SD at %s", s_mount_point.c_str());
    }
    s_card = nullptr;
    s_mounted = false;

    // Vapauta SPI-väylä
    spi_bus_free((spi_host_device_t)SD_SPI_HOST);
}

static std::string make_daily_csv_path(const StorageConfig& cfg, time_t t)
{
    char datebuf[16];
    format_date_ymd(datebuf, sizeof(datebuf), t);

    std::string dir = (cfg.base_dir && *cfg.base_dir) ? cfg.base_dir : s_mount_point;
    std::string prefix = (cfg.file_prefix && *cfg.file_prefix) ? cfg.file_prefix : "radon";

    std::string path = dir;
    if (!path.empty() && path.back() != '/') path.push_back('/');
    path += prefix;
    path.push_back('_');
    path += datebuf;
    path += ".csv";
    return path;
}

static void ensure_csv_header(FILE* f)
{
    long cur = ftell(f);
    fseek(f, 0, SEEK_END);
    long end = ftell(f);
    if (end == 0) {
        static const char* header = "timestamp_ms,radon_bq_m3,temperature_c,humidity_rh\n";
        fwrite(header, 1, strlen(header), f);
        fflush(f);
    }
    fseek(f, cur, SEEK_SET);
}

bool sdcard_write_sample(const StorageConfig& cfg, const RadonSample& s)
{
    if (!s_mounted) {
        ESP_LOGE(TAG, "SD not mounted");
        return false;
    }

    const char* dir = (cfg.base_dir && *cfg.base_dir) ? cfg.base_dir : s_mount_point.c_str();
    if (ensure_dir(dir) != ESP_OK) return false;

    time_t t_sec = (time_t)(s.timestamp_ms / 1000ULL);
    std::string path = make_daily_csv_path(cfg, t_sec);

    FILE* f = fopen(path.c_str(), "a+");
    if (!f) {
        ESP_LOGE(TAG, "fopen failed: %s", path.c_str());
        return false;
    }

    ensure_csv_header(f);

    int n = fprintf(f, "%llu,%.3f,%.3f,%.3f\n",
                    (unsigned long long)s.timestamp_ms,
                    (double)s.radon_bq_m3,
                    (double)s.temperature_c,
                    (double)s.humidity_rh);
    fflush(f);
    fclose(f);

    if (n <= 0) {
        ESP_LOGE(TAG, "write failed: %s", path.c_str());
        return false;
    }

    ESP_LOGD(TAG, "wrote sample to %s", path.c_str());
    return true;
}

