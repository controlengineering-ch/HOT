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
components/
├── no8_lis3dh/
│   ├── lis3dh.c
│   ├── lis3dh.h
│   └── CMakeLists.txt
├── shell/
│   ├── console.c         ◀️ Serial console
│   ├── console.h
│   └── CMakeLists.txt
peripherals_tests/
└── lis3dh_test.c         ◀️ Fuctional_Tests ++
main/
├── main.c
└── CMakeLists.txt


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


## 🛠️ Components

components/
└── shell/
    ├── console.c           ◀️ Komennot + komentojen rekisteröinti
    ├── console.h
    └── CMakeLists.txt
main/
├── sensor_manager.c       ◀️ Hallitsee mitä sensoreita on käynnissä
├── sensor_manager.h
└── main.c


