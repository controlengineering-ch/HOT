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



