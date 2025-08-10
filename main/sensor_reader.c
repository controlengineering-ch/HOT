#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sensor_reader.h"
#include "cJSON.h"
#include "lis3dh.h"  // ok vaikka ei käytetä vielä – pitää include-polun kunnossa

static bool parse_json(const char *buf, sensor_data_t *out) {
    cJSON *json = cJSON_Parse(buf);
    if (!json) return false;

    cJSON *radon = cJSON_GetObjectItem(json, "radon");
    cJSON *ts    = cJSON_GetObjectItem(json, "timestamp");

    bool ok = cJSON_IsNumber(radon) && cJSON_IsString(ts);
    if (ok) {
        out->radon_value = radon->valuedouble;
        // varmistetaan nollaterminointi
        strncpy(out->timestamp, ts->valuestring, sizeof(out->timestamp) - 1);
        out->timestamp[sizeof(out->timestamp) - 1] = '\0';
    }

    cJSON_Delete(json);
    return ok;
}

static bool parse_csv_line(const char *line, sensor_data_t *out) {
    // Odotetaan muotoa: ISO8601,radon
    // Leikataan ensimmäinen pilkkuun asti timestampiksi
    const char *comma = strchr(line, ',');
    if (!comma) return false;

    size_t ts_len = (size_t)(comma - line);
    if (ts_len == 0 || ts_len >= sizeof(out->timestamp)) return false;

    memcpy(out->timestamp, line, ts_len);
    out->timestamp[ts_len] = '\0';

    // Seuraava osa on radon double
    char *endptr = NULL;
    double val = strtod(comma + 1, &endptr);
    if (endptr == comma + 1) return false; // ei numeroa

    out->radon_value = val;
    return true;
}

bool read_radon_value(const char *filepath, sensor_data_t *data) {
    if (!filepath || !data) return false;

    FILE *file = fopen(filepath, "r");
    if (!file) return false;

    // Lue koko tiedosto puskuriksi (pieni konfigi / viimeisin mittaus ok)
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return false;
    }
    long len = ftell(file);
    if (len < 0) {
        fclose(file);
        return false;
    }
    rewind(file);

    char *buffer = (char *)malloc((size_t)len + 1);
    if (!buffer) {
        fclose(file);
        return false;
    }

    size_t nread = fread(buffer, 1, (size_t)len, file);
    fclose(file);
    buffer[nread] = '\0';

    // 1) JSON-yritys jos alkaa '{'
    bool ok = false;
    const char *p = buffer;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
    if (*p == '{') {
        ok = parse_json(buffer, data);
    }

    // 2) CSV fallback – otetaan viimeinen ei-tyhjä rivi
    if (!ok) {
        char *saveptr = NULL;
        char *line = strtok_r(buffer, "\r\n", &saveptr);
        sensor_data_t last = {0};
        bool found = false;

        while (line) {
            // ohita kommentit ja tyhjät
            if (*line != '#' && *line != '\0') {
                sensor_data_t tmp;
                if (parse_csv_line(line, &tmp)) {
                    last = tmp;
                    found = true;
                }
            }
            line = strtok_r(NULL, "\r\n", &saveptr);
        }

        if (found) {
            *data = last;
            ok = true;
        }
    }

    free(buffer);
    return ok;
}

bool append_to_history(const char *filepath, const sensor_data_t *data) {
    if (!filepath || !data) return false;

    // Tarkista duplikaatti aikaleiman perusteella
    FILE *rf = fopen(filepath, "r");
    if (rf) {
        char line[256];
        while (fgets(line, sizeof(line), rf)) {
            if (strstr(line, data->timestamp)) {
                fclose(rf);
                return true; // jo kirjattu – ok
            }
        }
        fclose(rf);
    }

    // Append-rivi CSV:nä
    FILE *wf = fopen(filepath, "a");
    if (!wf) return false;

    // ISO8601,radon
    int rc = fprintf(wf, "%s,%.1f\n", data->timestamp, data->radon_value);
    fclose(wf);
    return (rc > 0);
}

