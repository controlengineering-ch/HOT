#!/usr/bin/env bash
set -euo pipefail
shopt -s nullglob

ret=0
for comp in components/*; do
  [ -d "$comp" ] || continue
  cmake="$comp/CMakeLists.txt"
  incdir="$comp/include"
  if [ -d "$incdir" ]; then
    if ! grep -Eq 'INCLUDE_DIRS([^#]*\n)*\s*include(\s|$)' "$cmake"; then
      echo "WARN: $comp has include/ but CMakeLists.txt is missing 'INCLUDE_DIRS include'"
      ret=1
    fi
  fi
done

if [ $ret -eq 0 ]; then
  echo "OK: Kaikkien komponenttien include-polut näyttävät kunnossa."
else
  echo "HUOM: Tee korjaukset yllä mainittuihin CMakeListseihin."
fi

