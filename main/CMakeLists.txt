# main/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# Rakenna oletuksena vain LVGL-näyttötesti. Kytke pois: idf.py -D SMOKETEST=OFF build
option(SMOKETEST "Build only LVGL display smoketest" ON)

if(SMOKETEST)
    message(STATUS "Building in SMOKETEST mode (only display test)")
    idf_component_register(
        SRCS
            lvgl_smoketest.cpp
        INCLUDE_DIRS
            .
            include
        REQUIRES
            lvgl__lvgl
            esp_lvgl_port
        PRIV_REQUIRES
            panel_init
            driver
            esp_timer
            esp_lcd
    )
else()
    message(STATUS "Building FULL project")
    idf_component_register(
        SRCS
            main.cpp
            radon_analyzer.cpp
            radon_alert.cpp
            lvgl_gui.cpp
            sd_card.cpp
            sensor_manager.c
            ble_scan.cpp
            ble_sensor.cpp    
        INCLUDE_DIRS
            .
            include
        REQUIRES
            lvgl__lvgl
            esp_lvgl_port
            fatfs
            sdmmc          # SDSPI-mount tulee sdmmc-komponentista
            nvs_flash      # main.cpp: nvs_flash.h
            json           # cJSON tulee json-komponentista
        PRIV_REQUIRES
            panel_init
            driver
            esp_timer
            esp_lcd
            spi_flash
            esp_event
            esp_netif
            mqtt
            lis3dh
    )
endif()

