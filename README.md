# RadonSafe – ESP32-C6 BLE Überwachungssystem


👷 Projektin tarkoituksena on käyttää ESP32-C6 -mikrokontrolleria valvomaan radonpitoisuuksia. 

RadonSafe ist ein energiesparendes Sicherheitsüberwachungssystem, das auf dem ESP32-C6-LCD-1.47-Entwicklungsboard basiert. Es erkennt gefährliche Radonkonzentrationen in Echtzeit mithilfe von BLE-fähigen Airthings Wave Plus Sensoren.
(Update README.md with German and English descriptions)

## Eigenschaften

- 📡 **BLE-Erkennung** von Airthings Wave Plus Sensoren (alle 30s)
- 🧠 **Edge-Analyse** mit Warnlogik (220 Bq/m³ kritischer Grenzwert)
- 💾 **Datenlogging** auf SD-Karte (FIFO-Pufferung)
- 🌐 **Cloud-Upload** via Wi-Fi (Azure)
- 🔋 **Very Low Power Mode** mit GPIO-Aufwecksignal
- 📟 **1.47” LCD GUI** mit klarer Warnanzeige und Gerätenamen
- 🔒 **Lokaler Alarmstatus** bis zur manuellen Bestätigung gespeichert

## Systemübersicht

RadonSafe arbeitet vollständig autark im Feldbetrieb, erkennt automatisch verfügbare BLE-Sensoren, speichert Messwerte lokal, warnt bei Grenzwertüberschreitungen optisch und akustisch und lädt alle Messwerte regelmäßig in die Cloud.

---

# RadonSafe – ESP32-C6 BLE Monitoring System

RadonSafe is a low-power safety monitoring system based on the ESP32-C6-LCD-1.47 development board. It detects hazardous radon concentrations in real time using BLE-enabled Airthings Wave Plus sensors.

## Features

- 📡 **BLE scanning** for Airthings Wave Plus sensors (every 30s)
- 🧠 **Edge analytics** with warning logic (220 Bq/m³ critical threshold)
- 💾 **Data logging** to SD card (FIFO buffering)
- 🌐 **Cloud upload** via Wi-Fi (Azure)
- 🔋 **Very Low Power Mode** with GPIO wakeup signal
- 📟 **1.47” LCD GUI** with clear visual alerts and sensor name
- 🔒 **Local alarm status** retained until manually acknowledged

<<<<<<< HEAD
ENGLISH:

#RadonSafe v1.0

👷 The project's purpose is to use the ESP32-C6 microcontroller to monitor radon levels.

🚀 Functions:

- 🟡 BLE scanning: retrieves Airthings Wave Plus sensor radon measurements
- 📊 Analysis: compares values to thresholds (150/220 Bq/m³).
- 🖥️ Display: 1.47" TFT SPI display (LVGL)
- 💾 Storage: CSV files to an SD card
- ☁️ Cloud: data sent to Azure service
- 🔔 Alarm: yellow/red LED + display notification

## 🛠️ Construction

bash
cd esp32c6_radonsafe
source ~/esp-idf/export.sh
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor


SD card structure

/sdcard/
├ logs/
└ radon_log.csv
└── settings/
    └── config.json

config.json example:

{
  "polling_interval": 30000,
  "threshold_warn": 150,
  "threshold_danger": 220}
}

🔒 Notes:

    Use a FAT32-formatted SD card.

    BLE is only compatible with certain Airthings models.

    The Azure HTTPS endpoint is defined in the azure_upload.cpp file.

=======
## System Overview
>>>>>>> 52e2405 (Update README.md with German and English descriptions)

RadonSafe operates fully autonomously in the field, automatically detects nearby BLE sensors, stores measurements locally, issues visual/audio alerts if thresholds are exceeded, and periodically uploads all data to the cloud.

