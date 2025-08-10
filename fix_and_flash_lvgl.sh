#!/bin/bash

# Tiedoston sijainti
FILE="main/lvgl_gui.cpp"

# Varmuuskopioi alkuperäinen
cp "$FILE" "$FILE.bak"

echo "Päivitetään $FILE..."

# Lisää tarvittava include rivin #include "lvgl_gui.h" jälkeen
sed -i '/#include "lvgl_gui.h"/a #include <cstdio>' "$FILE"

# Korvaa lv_color_yellow() → lv_color_hex(0xFFFF00)
sed -i 's/lv_color_yellow()/lv_color_hex(0xFFFF00)/g' "$FILE"

# Korvaa lv_color_red() → lv_color_hex(0xFF0000)
sed -i 's/lv_color_red()/lv_color_hex(0xFF0000)/g' "$FILE"

echo "Korjaukset tehty. Käännetään ja fläshätään laite..."

# Rakennetaan ja fläshätään
idf.py build flash monitor
