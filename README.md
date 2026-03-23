# ESP8266 Wallbox Controller (ESPHome)

Steuert die Ladeleistung einer analogen EVSE (z. B. AnalogEVSE) über ein digitales Potentiometer (DS3502, I2C), die **ESPHome Native API** (Home Assistant) und optional die **Web-GUI** im lokalen Netz (`http://<Geräte-IP>/`, Port 80).

- Steuerung aus Home Assistant (Entitäten + optional `set_charging_amps`)
- Browser: Status und Override über den eingebauten ESPHome-Webserver
- PV-abhängige Automatik auf dem Gerät (Modus **Automatik**, Test über Sensor „Mocked PV Power“)

## Hardware

- ESP8266 (Konfiguration: Wemos D1 mini lite)
- Adafruit DS3502 (I2C)
- AnalogEVSE-Platine mit Widerstandssteuerung

## Konfiguration

1. Verzeichnis `src/esphome/`
2. `secrets.yaml` aus `secrets.yaml.example` anlegen und alle Platzhalter setzen (siehe `.gitignore`: `secrets.yaml` nicht committen). API-Key: `openssl rand -base64 32` → Eintrag `api_encryption` (Name muss zu `!secret api_encryption` in `wallbox.yaml` passen; umbenennen geht jederzeit). Beim Einbinden in Home Assistant denselben Schlüsselwert angeben.
3. `esphome run wallbox.yaml` (oder über das ESPHome-Dashboard).

Hintergrund zur **linearen** Strom↔Wiper-Zuordnung und optionalen Referenz-PDFs: siehe `Referenz/README.md`.

## Home Assistant (Überschussladen)

Paket mit Automation und Templates: **`homeassistant/packages/wallbox.yaml`** — Anleitung: **`homeassistant/README.md`**. Voraussetzung: API-Verschlüsselung wie in `secrets.yaml`, Gerät `wallbox` in HA eingebunden. Service für die Regelung: `set_charging_amps_auto` (nur Modus Automatik).
