#!/usr/bin/env bash
# Kopiert aus dem Wallbox-Repository:
#   - ESPHome: src/esphome/ → ~/homeassistant/esphome/
#   - HA-Paket: homeassistant/packages/wallbox.yaml → ~/homeassistant/packages/

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SRC="${ROOT}/src/esphome"
HA_BASE="${HOME}/homeassistant"
DEST_ESPHOME="${HA_BASE}/esphome"
DEST_PACKAGES="${HA_BASE}/packages"
HA_PKG_SRC="${ROOT}/homeassistant/packages/wallbox.yaml"

if [[ ! -f "${SRC}/wallbox.yaml" ]]; then
  echo "Fehler: ${SRC}/wallbox.yaml nicht gefunden." >&2
  exit 1
fi
if [[ ! -d "${SRC}/components" ]]; then
  echo "Fehler: ${SRC}/components nicht gefunden." >&2
  exit 1
fi

mkdir -p "${DEST_ESPHOME}"
cp "${SRC}/wallbox.yaml" "${DEST_ESPHOME}/"
cp -R "${SRC}/components" "${DEST_ESPHOME}/"
echo "OK ESPHome: ${SRC} -> ${DEST_ESPHOME}"

if [[ ! -f "${HA_PKG_SRC}" ]]; then
  echo "Hinweis: ${HA_PKG_SRC} nicht gefunden, überspringe HA-Paket." >&2
else
  mkdir -p "${DEST_PACKAGES}"
  cp "${HA_PKG_SRC}" "${DEST_PACKAGES}/wallbox.yaml"
  echo "OK HA-Paket: ${HA_PKG_SRC} -> ${DEST_PACKAGES}/wallbox.yaml"
fi
