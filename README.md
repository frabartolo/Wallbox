# ESP8266 Wallbox Controller (ESPHome)

Steuert die Ladeleistung einer analogen EVSE (z. B. AnalogEVSE) über ein digitales Potentiometer (DS3502, I2C), die **ESPHome Native API** (Home Assistant) und optional die **Web-GUI** im lokalen Netz (`http://<Geräte-IP>/`, Port 80).

- Steuerung aus Home Assistant (Entitäten + optional `set_charging_amps`)
- Browser: Status und Override über den eingebauten ESPHome-Webserver
- PV-/Überschuss-Regelung in **Home Assistant** (Modus **Automatik** auf dem ESP)

## Hardware

- ESP8266 (Konfiguration: Wemos D1 mini lite)
- Adafruit DS3502 (I2C)
- AnalogEVSE-Platine mit Widerstandssteuerung

## Konfiguration

1. Verzeichnis `src/esphome/`
2. `secrets.yaml` aus `secrets.yaml.example` anlegen und alle Platzhalter setzen (siehe `.gitignore`: `secrets.yaml` nicht committen). API-Key: `openssl rand -base64 32` → Eintrag `api_encryption` (Name muss zu `!secret api_encryption` in `wallbox.yaml` passen; umbenennen geht jederzeit). Beim Einbinden in Home Assistant denselben Schlüsselwert angeben.
3. `esphome run wallbox.yaml` (oder über das ESPHome-Dashboard).  
   Anzeige: **Soll-Ladestrom** (Slider, A), **Ladestrom** (A), **Ladeleistung** = Messwert aus HA (**Shelly Scheinleistung**; Entität `ha_shelly_scheinleistung_entity` anpassen – kann bei Ladern sinnvoller zur „gefühlt“ richtigen Größe passen als Wirkleistung allein).

Hintergrund zur **linearen** Strom↔Wiper-Zuordnung und optionalen Referenz-PDFs: siehe `Referenz/README.md`.

## Home Assistant (Überschussladen)

Home Assistant: Pakete unter **`homeassistant/packages/`** (MQTT/Volkszähler, EOS, Wallbox), Wallbox-Automation in **`homeassistant/automations/wallbox.yaml`** (`!include_dir_merge_list`) — Anleitung: **`homeassistant/README.md`**. Zentrale Konfiguration: **`homeassistant/configuration.example.yaml`**. Service: `set_charging_amps_auto` (nur Modus Automatik).
