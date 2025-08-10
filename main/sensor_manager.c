#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sensor_reader.h"
#if __has_include("cJSON.h")
  #include "cJSON.h"
#else
  #include <cjson/cJSON.h>
#endif
#include "lis3dh.h"  // ok vaikka ei käytetä tässä – varmistaa include-polun

static bool parse_json(const char *buf, sensor_data_t *out) {
    cJSON *json = cJSON_Parse(buf);
    if (!json) return false;

    cJSON *radon = cJSON_GetObjectItem(json, "radon");
    cJSON *ts    = cJSON_GetObjectItem(json, "timestamp");

    bool ok = cJSON_IsNumber(radon) && cJSON_IsString(ts);
    if (ok) {
        out->radon_value = radon->valuedouble;
        strncpy(out->timestamp, ts->valuestring, sizeof(out->timestamp) - 1);
        out->timestamp[sizeof(out->timestamp) - 1] = '\0';
    }

    cJSON_Delete(json);
    return ok;
}

static bool parse_csv_line(const char *line, sensor_data_t *out) {
    const char *comma = strchr(line, ',');
    if (!comma) return false;

    size_t ts_len = (size_t)(comma - line);
    if (ts_len == 0 || ts_len >= sizeof(out->timestamp)) return false;

    memcpy(out->timestamp, line, ts_len);
    out->timestamp[ts_len] = '\0';

    char *endptr = NULL;
    double val = strtod(comma + 1, &endptr);
    if (endptr == comma + 1) return false;

    out->radon_value = val;
    return true;
}

bool read_radon_value(const char *filepath, sensor_data_t *data) {
    if (!filepath || !data) return false;

    FILE *file = fopen(filepath, "r");
    if (!file) return false;

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

    bool ok = false;
    const char *p = buffer;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
    if (*p == '{') {
        ok = parse_json(buffer, data);
    }

    if (!ok) {
        char *saveptr = NULL;
        char *line = strtok_r(buffer, "\r\n", &saveptr);
        sensor_data_t last = {0};
        bool found = false;

        while (line) {
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

    FILE *wf = fopen(filepath, "a");
    if (!wf) return false;

    int rc = fprintf(wf, "%s,%.1f\n", data->timestamp, data->radon_value);
    fclose(wf);
    return (rc > 0);
}

