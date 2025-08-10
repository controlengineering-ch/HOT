#!/usr/bin/env bash
# Tarkistaa ettei projektissa ole duplikaatteja AlertLevel/RadonAlert -määrittelyistä.
# Sallittu ainoa lähde: main/radon_alert.h

set -euo pipefail

# Siirry projektin juureen (skripti sijaitsee scripts/-kansiossa)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

# Hakukuvio
PATTERN='enum[[:space:]]+AlertLevel|struct[[:space:]]+RadonAlert'

# Poissuljettavat hakemistot
EXCLUDES=(
  --exclude-dir build
  --exclude-dir .git
  --exclude-dir managed_components
  --exclude-dir esp-idf
  --exclude-dir .espressif
)

echo "==> Etsitään AlertLevel/RadonAlert -määrittelyt..."
MATCHING_FILES=$(grep -RilE "${PATTERN}" . "${EXCLUDES[@]}" || true)

# Ei osumia lainkaan
if [[ -z "${MATCHING_FILES}" ]]; then
  echo "OK: Ei löydy yhtään määrittelyä (tätä ei yleensä haluta) – varmista että main/radon_alert.h on olemassa."
  exit 0
fi

# Sallittu tiedosto
ALLOWED="./main/radon_alert.h"

# Suodata pois sallittu tiedosto
VIOLATIONS=$(echo "${MATCHING_FILES}" | grep -v "^${ALLOWED}\$" || true)

# Raportoi rivit + konteksti
echo
echo "==> Yksityiskohtaiset osumat (tiedosto:rivi:teksti):"
grep -RinE "${PATTERN}" . "${EXCLUDES[@]}" || true
echo

if [[ -n "${VIOLATIONS}" ]]; then
  echo "FAIL: Löytyi duplikaattimäärittelyjä muualla kuin ${ALLOWED}:"
  echo "${VIOLATIONS}"
  echo
  echo "Korjaa poistamalla omat enum/struct -määrittelyt näistä tiedostoista ja käytä vain:"
  echo '#include "radon_alert.h"'
  exit 1
fi

echo "OK: Määrittelyt löytyvät vain ${ALLOWED} – ei duplikaatteja."
exit 0

