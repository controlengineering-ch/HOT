cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(radon_safe)

set(SOURCES
    main.cpp
    lvgl_gui.cpp
    log_to_sd.cpp
    ble_scanner.cpp
)

