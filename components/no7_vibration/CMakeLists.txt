# CMakeLists.txt for component: no7_vibration

idf_component_register(
    SRCS
        no7_vibration.c
    INCLUDE_DIRS
        .
    PRIV_REQUIRES
        esp_timer
        driver
)

