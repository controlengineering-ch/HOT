README.md

# 📡 RadonSafe – ESP32-C6 Radon Monitor Gateway

RadonSafe is an **ESP-IDF**-based IoT device that reads and displays **Airthings Wave Plus** radon levels and provides local alerts without relying on any cloud service.  
It operates as a standalone gateway device, storing data on an SD card, and can forward it via MQTT, WebSocket, or OPC UA.

---

## 🚀 Features

- **ESP32-C6** with Wi-Fi 6 + Bluetooth LE
- 1.47" LCD display (172x320) with LVGL graphics
- Airthings Wave Plus data reading (manual sync)
- Local alert logic (150 Bq/m³ warning, 220 Bq/m³ critical)
- SD card data logging
- Wake-up from touch, motion, or vibration
- Supports up to 3 sensors in current configuration
- MQTT, WebSocket, and OPC UA connectivity
- Secure communication (HTTPS + Proof-of-Possession)

---

## 📂 Project Structure

RadonSafe_Project/
├── components/
│ ├── lis3dh/ # LIS3DH accelerometer driver
│ ├── mqtt_min/ # Lightweight MQTT client
├── main/
│ ├── main_gui.c # LVGL GUI implementation
│ ├── sensor_manager.c # Sensor management
├── scripts/ # Utility scripts
├── README.md
└── .gitignore


---
## 🔧 Development Environment

- **ESP-IDF v5/v6**
- **SquareLine Studio** for LVGL GUI design
- CMake project structure
- Git version control with submodules (e.g., cJSON)

## 🛠️ Build & Flash

1. Install ESP-IDF and set up the environment:
   ```bash
   . $HOME/esp/esp-idf/export.sh
Build the project:

idf.py build
Flash to device:
idf.py -p /dev/ttyUSB0 flash monitor


License
This project is released under the MIT License.
See the LICENSE file for details.
idf.py -p /dev/ttyUSB0 flash monitor






🇫🇮 Suomi
Yleiskuvaus
RadonSafe on ESP-IDF-pohjainen IoT-laite, joka lukee Airthings Wave Plus -radonmittarin arvoja ja näyttää hälytykset paikallisesti ilman pilvipalvelua.
Laite toimii itsenäisenä gatewayna, tallentaa SD-kortille ja voi välittää dataa MQTT:n, WebSocketin tai OPC UA:n kautta.

Ominaisuudet
ESP32-C6 (Wi-Fi 6 + Bluetooth LE)

1,47" LCD (172×320) LVGL-käyttöliittymällä

Airthings Wave Plus (manuaalinen synkronointi)

Paikallinen hälytyslogiikka: 150 Bq/m³ (varoitus), 220 Bq/m³ (kriittinen)

SD-korttilogitus

Herätys kosketuksesta/liikkeestä/tärinästä (enint. 3 sensoria)

MQTT, WebSocket, OPC UA

Suojattu viestintä (HTTPS + Proof-of-Possession)

Projektirakenne
css
Copy
Edit
RadonSafe_Project/
├── components/
│   ├── lis3dh/
│   └── mqtt_min/
├── main/
│   ├── main_gui.c
│   └── sensor_manager.c
├── scripts/
├── README.md
└── .gitignore
Kehitysympäristö
ESP-IDF v5/v6, CMake

LVGL + (valinn.) SquareLine Studio

Git + submoduulit (esim. cJSON)

Käännös & Flash
bash
Copy
Edit
. $HOME/esp/esp-idf/export.sh
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
Lisenssi
MIT — katso LICENSE.

# 📡 RadonSafe – ESP32-C6 Radon Monitor Gateway

![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5%2Fv6-blue?logo=espressif)
![LVGL](https://img.shields.io/badge/LVGL-9.x-purple?logo=lvgl)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![Status](https://img.shields.io/badge/Status-Active-success)


