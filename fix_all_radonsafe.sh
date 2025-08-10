#!/bin/bash

set -e

echo "Korjataan kaikki virheet..."

# 1. Lisää <cstdio> jos puuttuu lvgl_gui.cpp:ssä
file="main/lvgl_gui.cpp"
if ! grep -q "#include <cstdio>" "$file"; then
    echo "Lisätään <cstdio> tiedostoon $file"
    sed -i '/#include "lvgl_gui.h"/a #include <cstdio>' "$file"
fi

# 2. Korvaa virheelliset värit oikeilla
sed -i 's/lv_color_yellow()/lv_color_hex(0xFFFF00)/g' "$file"
sed -i 's/lv_color_red()/lv_color_hex(0xFF0000)/g' "$file"

# 3. Lisää <stdbool.h> sensor_manager.h:hen
file="main/sensor_manager.h"
if ! grep -q "#include <stdbool.h>" "$file"; then
    echo "Lisätään <stdbool.h> tiedostoon $file"
    sed -i '1i #include <stdbool.h>' "$file"
fi

# 4. Luo puuttuvat header-tiedostot tyhjinä jos niitä ei ole
touch_if_missing() {
    if [ ! -f "main/$1" ]; then
        echo "Luodaan puuttuva header: main/$1"
        echo "#pragma once" > "main/$1"
        echo "// Tämä tiedosto on placeholder." >> "main/$1"
    fi
}
touch_if_missing "sensor_data.h"
touch_if_missing "no7_vibration.h"

echo "Kaikki korjaukset tehty ✅"

