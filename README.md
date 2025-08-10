# RadonSafe Project

RadonSafe is an ESP32-C6 based sensor gateway designed for local radon level monitoring without the need for cloud-based APIs.  
It reads **Airthings Wave Plus** sensor data from an SD card (`radon_latest.txt`), displays values on a 1.47" LCD using LVGL, and triggers local alerts based on thresholds.  
Optional upgrades include touch displays, additional sensors, and Ethernet/PoE support (ESP32-S3 variant).

---

## 📌 Features
- **ESP32-C6** gateway with LVGL-based GUI
- Reads radon data from SD card (no cloud dependency)
- Alert levels:
  - **Warning**: 150 Bq/m³
  - **Critical**: 220 Bq/m³
- Multiple sensor support (max 3 in RadonSafe variant)
- Wake-on proximity, motion, vibration, or touch
- Local data storage and secure upload (HTTPS, MQTT, OPC UA options)
- Optional Ethernet/PoE branch: `radonsafe_esp32s3_ethernet`

---

## 🗂 Branch Strategy
| Branch name                     | Purpose |
|----------------------------------|---------|
| `main`                           | Stable production-ready code |
| `radonsafe`                      | Radon monitoring (ESP32-C6) |
| `radonsafe_esp32s3_ethernet`     | Ethernet/PoE version (ESP32-S3) |
| `energywise`                     | Energy optimization logic |
| `sensorclever`                   | Advanced sensor fusion & AI |
| `powergate`                      | Power control and battery gateway |

---

## ⚙️ Development Setup

### Prerequisites
- ESP-IDF (latest stable version)
- CMake + Ninja build system
- Git
- SquareLine Studio (for LVGL GUI design)



### 🗂 Branch Strategy
| Branch name                     | Purpose |
|----------------------------------|---------|
| `main`                           | Stable production-ready code |
| `radonsafe`                      | Radon monitoring (ESP32-C6) |
| `radonsafe_esp32s3_ethernet`     | Ethernet/PoE version (ESP32-S3) |
| `energywise`                     | Energy optimization logic |
| `sensorclever`                   | Advanced sensor fusion & AI |
| `powergate`                      | Power control and battery gateway |

## ⚙️ Development Setup

### Prerequisites
- ESP-IDF (latest stable version)
- CMake + Ninja build system
- Git
- SquareLine Studio (for LVGL GUI design)

### Clone repository

```bash
git clone https://github.com/controlengineering-ch/HOT.git
cd HOT
git submodule update --init --recursive

### Building the Project

idf.py set-target esp32c6   # or esp32s3 for Ethernet branch
idf.py menuconfig
idf.py build
idf.py flash
idf.py monitor

📄 Git Branch Quick Commands

