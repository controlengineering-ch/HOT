#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Yksittäinen mittanäyte */
typedef struct {
    uint64_t timestamp_ms;   /* aikaleima millisekunteina */
    float    radon_bq_m3;    /* radonpitoisuus Bq/m3 */
    float    temperature_c;  /* lämpötila C */
    float    humidity_rh;    /* suhteellinen kosteus % */
} RadonSample;

/* Tallennuskonfiguraatio SD:lle tms. */
typedef struct {
    const char *base_dir;    /* esim. "/sdcard" */
    const char *file_prefix; /* esim. "radon" -> "radon_YYYYMMDD.csv" */
    size_t      max_records; /* haluttu maksimi rivimäärä / tiedosto (0 = ei rajaa) */
} StorageConfig;

/* Yksinkertainen aggregaatti (jos tarvitset myöhemmin) */
typedef struct {
    float   min_bq_m3;
    float   max_bq_m3;
    float   avg_bq_m3;
    uint32_t count;
} RadonStats;

#ifdef __cplusplus
}
#endif

