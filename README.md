# RadonSafe v1.0

👷 Projektin tarkoituksena on käyttää ESP32-C6 -mikrokontrolleria valvomaan radonpitoisuuksia. 

## 🚀 Toiminnot

- 🟡 BLE-skannaus: hakee Airthings Wave Plus -sensorin radonmittaukset
- 📊 Analyysi: vertaa arvoja raja-arvoihin (150/220 Bq/m³)
- 🖥️ Näyttö: 1.47" TFT SPI-näyttö (LVGL)
- 💾 Tallennus: CSV-tiedostot SD-kortille
- ☁️ Pilvi: tiedot lähetetään Azure-palvelimeen
- 🔔 Hälytys: keltainen/punainen LED + näyttöilmoitus

## 🛠️ Rakentaminen

```bash
cd esp32c6_radonsafe
source ~/esp-idf/export.sh
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor


SD-kortin rakenne

/sdcard/
├── logs/
│   └── radon_log.csv
└── settings/
    └── config.json

config.json-esimerkki:

{
  "polling_interval": 30000,
  "threshold_warn": 150,
  "threshold_danger": 220
}

🔒 Huomioita

    Käytä FAT32-formatoitua SD-korttia

    BLE yhteensopiva vain tiettyjen Airthings-mallien kanssa

    Azure HTTPS-endpoint määritellään azure_upload.cpp-tiedostossa

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



