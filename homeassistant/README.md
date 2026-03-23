# Home Assistant: Paketstruktur

Die zentrale **`configuration.yaml`** bleibt schlank; fachliche Blöcke liegen unter **`/config/packages/`**. **Automationen** können analog in **`/config/automations/`** auf mehrere Dateien verteilt werden (`!include_dir_merge_list`).

## Dateien im Repository

| Datei | Inhalt |
|--------|--------|
| **`configuration.example.yaml`** | Vorbild für die **root** `configuration.yaml` (ohne MQTT/EOS/Wallbox-Logik) |
| **`configuration_wallbox.fragment.yaml`** | Nur der Block `homeassistant:` mit `packages` (falls du ihn separat einfügen willst) |
| **`automations/wallbox.yaml`** | Automation „Überschuss an ESPHome“ (`set_charging_amps_auto`) — ergänzt `packages/wallbox.yaml` |
| **`packages/mqtt_volkszaehler.yaml`** | MQTT-Sensoren (Volkszähler / vzlogger) |
| **`packages/eos.yaml`** | `rest_command` + REST-Sensor für EOS |
| **`packages/wallbox.yaml`** | Hilfs-Entitäten + Template-Sensoren (PV-Rohleistung, Zielstrom); Steuer-Automation: **`automations/wallbox.yaml`** |

## Einbindung

Root **`/config/configuration.yaml`** muss u. a. enthalten:

```yaml
homeassistant:
  packages: !include_dir_named packages/

automation: !include_dir_merge_list automations/
```

Alle bisherigen Definitionen zu **MQTT (Volkszähler)**, **EOS** und **Wallbox** aus der großen `configuration.yaml` **entfernen**, sobald die gleichen Inhalte als Pakete unter `/config/packages/` liegen – sonst doppelte Entitäten / Konflikte.

Unter **`/config/automations/`** liegen beliebig viele `.yaml`-Dateien; jede enthält eine **Liste** (mit `- id:` …), Home Assistant führt sie zu **einer** Automation-Liste zusammen. Dateinamen sind egal (z. B. `wallbox.yaml`, `benachrichtigungen.yaml`).

**Hinweis:** Die alten MQTT-Einträge *„Wallbox Ladeleistung“ / „Wallbox Modus“* auf `wallbox/state` sind **entfernt** – dieselben Informationen kommen von **ESPHome** (und würden sonst doppelt erscheinen).

## Messgröße Wallbox-Paket

Siehe Kommentar in `packages/wallbox.yaml`: **`sensor.16-7-0-VerbrauchAktuellW`**, negatives W = Einspeisung → `max(0,-w)`.

## Skript

`scripts/update-homeassistant-esphome.sh` kopiert **`homeassistant/packages/*.yaml`** nach `~/homeassistant/packages/`, **`homeassistant/automations/*.yaml`** (z. B. **`wallbox.yaml`**) nach `~/homeassistant/automations/` (und ESPHome nach `~/homeassistant/esphome/`).

## Service

`esphome.wallbox_set_charging_amps_auto` (ESPHome-Gerät `wallbox` verbunden).
