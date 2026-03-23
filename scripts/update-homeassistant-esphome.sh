#!/usr/bin/env bash
# Kopiert aus dem Wallbox-Repository:
#   - ESPHome: src/esphome/ → ~/homeassistant/esphome/
#   - HA-Pakete: homeassistant/packages/*.yaml → ~/homeassistant/packages/
#   - homeassistant/automations/*.yaml (u. a. wallbox.yaml) → ~/homeassistant/automations/

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SRC="${ROOT}/src/esphome"
HA_BASE="${HOME}/homeassistant"
DEST_ESPHOME="${HA_BASE}/esphome"
DEST_PACKAGES="${HA_BASE}/packages"
DEST_AUTOMATIONS="${HA_BASE}/automations"
HA_PKG_DIR="${ROOT}/homeassistant/packages"
HA_AUTOMATIONS_DIR="${ROOT}/homeassistant/automations"

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

if [[ ! -d "${HA_PKG_DIR}" ]]; then
  echo "Hinweis: ${HA_PKG_DIR} nicht gefunden, überspringe HA-Pakete." >&2
else
  shopt -s nullglob
  pkgs=( "${HA_PKG_DIR}"/*.yaml )
  shopt -u nullglob
  if [[ ${#pkgs[@]} -eq 0 ]]; then
    echo "Hinweis: Keine *.yaml in ${HA_PKG_DIR}" >&2
  else
    mkdir -p "${DEST_PACKAGES}"
    cp "${HA_PKG_DIR}"/*.yaml "${DEST_PACKAGES}/"
    echo "OK HA-Pakete (${#pkgs[@]} Dateien): ${HA_PKG_DIR} -> ${DEST_PACKAGES}/"
  fi
fi

if [[ ! -d "${HA_AUTOMATIONS_DIR}" ]]; then
  echo "Hinweis: ${HA_AUTOMATIONS_DIR} nicht gefunden, überspringe HA-Automationen." >&2
else
  shopt -s nullglob
  autos=( "${HA_AUTOMATIONS_DIR}"/*.yaml )
  shopt -u nullglob
  if [[ ${#autos[@]} -eq 0 ]]; then
    echo "Hinweis: Keine *.yaml in ${HA_AUTOMATIONS_DIR}" >&2
  else
    mkdir -p "${DEST_AUTOMATIONS}"
    cp "${HA_AUTOMATIONS_DIR}"/*.yaml "${DEST_AUTOMATIONS}/"
    echo "OK HA-Automationen (${#autos[@]} Dateien): ${HA_AUTOMATIONS_DIR} -> ${DEST_AUTOMATIONS}/"
  fi
fi
