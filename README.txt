RadonSafe MVP Drop 1
====================

Included:
- scripts/sdcard_test_radon.sh — Automated mount + write/read test for radon_latest.txt
- components/lis3dh/ — Minimal LIS3DH driver (WHO_AM_I, init, XYZ raw)
- main/main_gui.c — Minimal LVGL UI stub with status banner and file-driven update
- components/mqtt_min/ — Minimal MQTT client (QoS1 publish loop with LWT/reconnect)

How to use
----------
1) Copy files into your ESP-IDF project, preserving folder structure (or adjust CMakeLists accordingly).
2) Ensure `scripts/smart_mount_sdcard_radonsafe.sh` is available/executable for the sdcard test script.
3) Update `components/mqtt_min/mqtt_min_config.h` with your broker URI and credentials.
4) In your app init:
   - Initialize LVGL and call `main_gui_create()` once.
   - Periodically call `main_gui_update_from_file("/sdcard/RadonSafe/data/radon_latest.txt")`.
   - Call `mqtt_min_start()` after network is up, and then call `mqtt_min_tick("<value>")` about every 30s.

