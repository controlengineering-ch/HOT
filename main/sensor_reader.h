#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double radon_value;
    char   timestamp[40];  // ISO8601 mahtuu hyvin
} sensor_data_t;

/**
 * Lukee tiedoston ja palauttaa radonin sekä aikaleiman.
 * Tukee JSON-muotoa:
 *   {"radon": 98.0, "timestamp":"2025-08-09T22:55:00Z"}
 * ja fallbackina CSV-riviä:
 *   2025-08-09T22:55:00Z,98.0
 */
bool read_radon_value(const char *filepath, sensor_data_t *data);

/** Lisää rivin CSV-historiaan jos timestamp ei vielä esiinny. */
bool append_to_history(const char *filepath, const sensor_data_t *data);

#ifdef __cplusplus
}
#endif

