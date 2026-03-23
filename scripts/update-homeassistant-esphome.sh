#!/usr/bin/env bash
# Kopiert die ESPHome-Konfiguration nach ~/homeassistant/esphome/
# (Quelle: src/esphome/ im Wallbox-Repository)

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SRC="${ROOT}/src/esphome"
DEST="${HOME}/homeassistant/esphome"

if [[ ! -f "${SRC}/wallbox.yaml" ]]; then
  echo "Fehler: ${SRC}/wallbox.yaml nicht gefunden." >&2
  exit 1
fi
if [[ ! -d "${SRC}/components" ]]; then
  echo "Fehler: ${SRC}/components nicht gefunden." >&2
  exit 1
fi

mkdir -p "${DEST}"

cp "${SRC}/wallbox.yaml" "${DEST}/"
cp -R "${SRC}/components" "${DEST}/"

echo "OK: ${SRC} -> ${DEST}"
