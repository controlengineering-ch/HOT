cmake_minimum_required(VERSION 3.16)

# Tämä rivi kertoo ESP-IDF:lle, että omat komponentit löytyvät tästä kansiosta:
set(EXTRA_COMPONENT_DIRS components)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Tämän nimen näet build-hakemistossa, firmware.bin-tiedostoissa jne.
project(RadonSafe_Project)
