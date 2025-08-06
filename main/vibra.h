#ifndef VIBRA_SENSOR_H
#define VIBRA_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

void vibra_sensor_init(void);
bool vibra_sensor_triggered(void);  // palauttaa true jos tärinää havaittu

#ifdef __cplusplus
}
#endif

#endif // VIBRA_SENSOR_H

