README.md

markdown
Copy
Edit
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

yaml
Copy
Edit

---

## 🔧 Development Environment

- **ESP-IDF v5/v6**
- **SquareLine Studio** for LVGL GUI design
- CMake project structure
- Git version control with submodules (e.g., cJSON)

---

## 🛠️ Build & Flash

1. Install ESP-IDF and set up the environment:
   ```bash
   . $HOME/esp/esp-idf/export.sh
Build the project:

bash
Copy
Edit
idf.py build
Flash to device:

bash
Copy
Edit
idf.py -p /dev/ttyUSB0 flash monitor
📜 License
This project is released under the MIT License.
See the LICENSE file for details.

yaml
Copy
Edit

---

If you want, I can add **a status flow diagram and a sequence diagram** so that the GitHub README also visually shows how RadonSafe works — that would make it stand out immediately.







You said:
ja saksa


ChatGPT said:
Here’s the German version of the RadonSafe README.md.

README.md (Deutsch)

markdown
Copy
Edit
# 📡 RadonSafe – ESP32-C6 Radon-Monitor-Gateway

RadonSafe ist ein **ESP-IDF**-basiertes IoT-Gerät, das die Radonwerte eines **Airthings Wave Plus**-Sensors ausliest und anzeigt.  
Es bietet lokale Warnmeldungen, ohne auf einen Cloud-Dienst angewiesen zu sein.  
Das Gerät arbeitet als eigenständiges Gateway, speichert Daten auf einer SD-Karte und kann diese über MQTT, WebSocket oder OPC UA weiterleiten.

---

## 🚀 Funktionen

- **ESP32-C6** mit Wi-Fi 6 + Bluetooth LE
- 1,47" LCD-Display (172x320) mit LVGL-Grafik
- Auslesen der Airthings Wave Plus-Daten (manuelle Synchronisierung)
- Lokale Alarm-Logik (150 Bq/m³ Warnung, 220 Bq/m³ kritisch)
- SD-Karten-Datenspeicherung
- Aufwecken durch Berührung, Bewegung oder Vibration
- Unterstützung von bis zu 3 Sensoren in aktueller Konfiguration
- MQTT-, WebSocket- und OPC UA-Konnektivität
- Sichere Kommunikation (HTTPS + Proof-of-Possession)

---

## 📂 Projektstruktur

RadonSafe_Project/
├── components/
│ ├── lis3dh/ # LIS3DH-Beschleunigungssensor-Treiber
│ ├── mqtt_min/ # Leichter MQTT-Client
├── main/
│ ├── main_gui.c # LVGL-GUI-Implementierung
│ ├── sensor_manager.c # Sensorverwaltung
├── scripts/ # Hilfsskripte
├── README.md
└── .gitignore

yaml
Copy
Edit

---

## 🔧 Entwicklungsumgebung

- **ESP-IDF v5/v6**
- **SquareLine Studio** für LVGL-GUI-Design
- CMake-Projektstruktur
- Git-Versionskontrolle mit Submodulen (z. B. cJSON)

---

## 🛠️ Kompilieren & Flashen

1. ESP-IDF installieren und die Umgebung laden:
   ```bash
   . $HOME/esp/esp-idf/export.sh
Projekt kompilieren:

bash
Copy
Edit
idf.py build
Auf das Gerät flashen:

bash
Copy
Edit
idf.py -p /dev/ttyUSB0 flash monitor
📜 Lizenz
Dieses Projekt wird unter der MIT-Lizenz veröffentlicht.
Weitere Informationen siehe LICENSE.

yaml
Copy
Edit

---

Jos haluat, voin yhdistää nämä niin, että **README.md sisältää kielivalikon (EN / DE / FI)** ja ohjaa käyttäjän oikeaan osioon GitHubissa ilman erillisiä tiedostoja.  
Se olisi siisti ja ammattimainen ratkaisu.







You said:
Jos haluat, voin yhdistää nämä niin, että **README.md sisältää kielivalikon (EN / DE / FI)** ja ohjaa käyttäjän oikeaan osioon GitHubissa ilman erillisiä tiedostoja.  
Se olisi siisti ja ammattimainen ratkaisu.


ChatGPT said:
Tässä yksi README.md, jossa on kielivalikko (EN / DE / FI) ja ankkurilinkit samoihin tiedostoihin — ei tarvita erillisiä READMEjä:

markdown
Copy
Edit
# 📡 RadonSafe – ESP32-C6 Radon Monitor Gateway

**Languages:** [EN](#-english) | [DE](#-deutsch) | [FI](#-suomi)

---

## 🇬🇧 English

### Overview
RadonSafe is an **ESP-IDF** based IoT device that reads and displays **Airthings Wave Plus** radon levels and provides local alerts without cloud dependency.  
It operates as a standalone gateway, logs to SD card, and can forward data via MQTT, WebSocket, or OPC UA.

### Features
- **ESP32-C6** with Wi-Fi 6 + Bluetooth LE  
- 1.47" LCD (172×320) with **LVGL** UI  
- Airthings Wave Plus data (manual sync)  
- Local alert logic: **150 Bq/m³ (warning)**, **220 Bq/m³ (critical)**  
- SD card logging  
- Wake on touch/motion/vibration (up to 3 sensors)  
- MQTT, WebSocket, OPC UA options  
- Secure comms (HTTPS + Proof-of-Possession)

### Project Structure
RadonSafe_Project/
├── components/
│ ├── lis3dh/ # LIS3DH accelerometer driver
│ └── mqtt_min/ # Lightweight MQTT client
├── main/
│ ├── main_gui.c # LVGL GUI implementation
│ └── sensor_manager.c # Sensor management
├── scripts/ # Utility scripts
├── README.md
└── .gitignore

bash
Copy
Edit

### Dev Environment
- **ESP-IDF v5/v6**, CMake
- LVGL + (optional) SquareLine Studio
- Git with submodules (e.g., cJSON)

### Build & Flash
```bash
. $HOME/esp/esp-idf/export.sh
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
License
MIT — see LICENSE.

🇩🇪 Deutsch
Überblick
RadonSafe ist ein ESP-IDF-basiertes IoT-Gerät, das die Airthings Wave Plus-Radonwerte ausliest und lokal anzeigt – ganz ohne Cloud.
Es arbeitet als eigenständiges Gateway, protokolliert auf SD-Karte und kann Daten über MQTT, WebSocket oder OPC UA weiterleiten.

Funktionen
ESP32-C6 mit Wi-Fi 6 + Bluetooth LE

1,47" LCD (172×320) mit LVGL

Airthings Wave Plus (manuelle Synchronisierung)

Lokale Alarm-Logik: 150 Bq/m³ (Warnung), 220 Bq/m³ (kritisch)

SD-Karten-Logging

Aufwecken durch Berührung/Bewegung/Vibration (bis zu 3 Sensoren)

MQTT, WebSocket, OPC UA

Sichere Kommunikation (HTTPS + Proof-of-Possession)

Projektstruktur
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
Entwicklungsumgebung
ESP-IDF v5/v6, CMake

LVGL + optional SquareLine Studio

Git-Submodule (z. B. cJSON)

Kompilieren & Flashen
bash
Copy
Edit
. $HOME/esp/esp-idf/export.sh
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
Lizenz
MIT — siehe LICENSE.

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

Back to top: EN | DE | FI

scss
Copy
Edit

Haluatko, että lisään ylös myös pienet **badget** (ESP-IDF, LVGL, License) ja pari kuvaa (UI screenshot + laitekuva) GitHub-tyyliin? Se tekee README:stä heti viimeistellyn.







You said:
Tottakai


ChatGPT said:
Hyvä 👍
Tehdään README:stä viimeistelty niin, että siinä on kielivalikko, badget ja kuvat GitHub-tyyliin.

README.md

markdown
Copy
Edit
# 📡 RadonSafe – ESP32-C6 Radon Monitor Gateway

![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5%2Fv6-blue?logo=espressif)
![LVGL](https://img.shields.io/badge/LVGL-9.x-purple?logo=lvgl)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![Status](https://img.shields.io/badge/Status-Active-success)

**Languages:** [🇬🇧 English](#-english) | [🇩🇪 Deutsch](#-deutsch) | [🇫🇮 Suomi](#-suomi)

---

<p align="center">
  <img src="docs/images/radonsafe_ui.png" alt="RadonSafe UI" width="300">
  <img src="docs/images/radonsafe_device.jpg" alt="RadonSafe Device" width="300">
</p>

---

## 🇬🇧 English

### Overview
RadonSafe is an **ESP-IDF** based IoT device that reads and displays **Airthings Wave Plus** radon levels and provides local alerts without cloud dependency.  
It operates as a standalone gateway, logs to SD card, and can forward data via MQTT, WebSocket, or OPC UA.

### Features
- **ESP32-C6** with Wi-Fi 6 + Bluetooth LE  
- 1.47" LCD (172×320) with **LVGL** UI  
- Airthings Wave Plus data (manual sync)  
- Local alert logic: **150 Bq/m³ (warning)**, **220 Bq/m³ (critical)**  
- SD card logging  
- Wake on touch/motion/vibration (up to 3 sensors)  
- MQTT, WebSocket, OPC UA options  
- Secure comms (HTTPS + Proof-of-Possession)

### Project Structure
RadonSafe_Project/
├── components/
│ ├── lis3dh/ # LIS3DH accelerometer driver
│ └── mqtt_min/ # Lightweight MQTT client
├── main/
│ ├── main_gui.c # LVGL GUI implementation
│ └── sensor_manager.c # Sensor management
├── scripts/ # Utility scripts
├── README.md
└── .gitignore

bash
Copy
Edit

### Dev Environment
- **ESP-IDF v5/v6**, CMake
- LVGL + (optional) SquareLine Studio
- Git with submodules (e.g., cJSON)

### Build & Flash
```bash
. $HOME/esp/esp-idf/export.sh
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
License
MIT — see LICENSE.

🇩🇪 Deutsch
Überblick
RadonSafe ist ein ESP-IDF-basiertes IoT-Gerät, das die Airthings Wave Plus-Radonwerte ausliest und lokal anzeigt – ganz ohne Cloud.
Es arbeitet als eigenständiges Gateway, protokolliert auf SD-Karte und kann Daten über MQTT, WebSocket oder OPC UA weiterleiten.

Funktionen
ESP32-C6 mit Wi-Fi 6 + Bluetooth LE

1,47" LCD (172×320) mit LVGL

Airthings Wave Plus (manuelle Synchronisierung)

Lokale Alarm-Logik: 150 Bq/m³ (Warnung), 220 Bq/m³ (kritisch)

SD-Karten-Logging

Aufwecken durch Berührung/Bewegung/Vibration (bis zu 3 Sensoren)

MQTT, WebSocket, OPC UA

Sichere Kommunikation (HTTPS + Proof-of-Possession)

Projektstruktur
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
Entwicklungsumgebung
ESP-IDF v5/v6, CMake

LVGL + optional SquareLine Studio

Git-Submodule (z. B. cJSON)

Kompilieren & Flashen
bash
Copy
Edit
. $HOME/esp/esp-idf/export.sh
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
Lizenz
MIT — siehe LICENSE.

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

Back to top: 🇬🇧 EN | 🇩🇪 DE | 🇫🇮 FI

yaml
Copy
Edit

---

📌 **Seuraavat vaiheet**  
- Tarvitaan **`docs/images/`** -kansio ja sinne 2 kuvaa:
  1. `radonsafe_ui.png` → LVGL-käyttöliittymäkuva
  2. `radonsafe_device.jpg` → Laitteen kuva  
- Kun kuvat ovat siellä, GitHub näyttää README:n täydellisenä.

Haluatko, että teen sulle myös **yksinkertaisen Graphviz-sequence-kaavion**, joka näyttää sensorin, ESP32-C6:n ja käyttäjän välisen datavirran README:n loppuun?  
Se nostaa dokumentin uskottavuutta heti.
