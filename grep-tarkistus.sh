# 1) Listaa kaikki osumat selkeästi (tiedosto:rivi:osuma)
grep -RinE \
  --exclude-dir build \
  --exclude-dir .git \
  --exclude-dir managed_components \
  --exclude-dir esp-idf \
  --exclude-dir .espressif \
  "enum[[:space:]]+AlertLevel|struct[[:space:]]+RadonAlert" .

# 2) Näytä pelkät tiedostonimet (duplikaattien nopeaan havaitsemiseen)
grep -RilE \
  --exclude-dir build \
  --exclude-dir .git \
  --exclude-dir managed_components \
  --exclude-dir esp-idf \
  --exclude-dir .espressif \
  "enum[[:space:]]+AlertLevel|struct[[:space:]]+RadonAlert" .

