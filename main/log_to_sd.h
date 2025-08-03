#ifndef LOG_TO_SD_H
#define LOG_TO_SD_H

#include <string>

bool initSDCard(const char* mountPath = "/sdcard");
bool logSensorData(const std::string& sensorID, float radon, float co2, float temp, float humidity);

#endif // LOG_TO_SD_H

