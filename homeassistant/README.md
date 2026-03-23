# Home Assistant: Paketstruktur

Die zentrale **`configuration.yaml`** bleibt schlank; fachliche Blöcke liegen unter **`/config/packages/`**. **Automationen** können analog in **`/config/automations/`** auf mehrere Dateien verteilt werden (`!include_dir_merge_list`).

## Dateien im Repository

| Datei | Inhalt |
|--------|--------|
| **`configuration.example.yaml`** | Vorbild für die **root** `configuration.yaml` (ohne MQTT/EOS/Wallbox-Logik) |
| **`configuration_wallbox.fragment.yaml`** | Nur der Block `homeassistant:` mit `packages` (falls du ihn separat einfügen willst) |
| **`automations/wallbox.yaml`** | Überschuss → ESPHome; optional Benachrichtigung „Ladevorgang beendet“ (Scheinleistung unter 2000 VA, 1 min) — `notify.*` anpassen |
| **`packages/mqtt_volkszaehler.yaml`** | MQTT-Sensoren (Volkszähler / vzlogger) |
| **`packages/eos.yaml`** | `rest_command` + REST-Sensor für EOS |
| **`packages/wallbox.yaml`** | Hilfs-Entitäten, `input_boolean.wallbox_pv_regelung`, Templates inkl. **`sensor.wallbox_*_ladestrom_gui`** (Anzeige 0 A wie Zielstrom); Automation: **`automations/wallbox.yaml`** |
| **`dashboards/wallbox.yaml`** | Lovelace-Dashboard „Wallbox“ (optional; `lovelace:` in `configuration.yaml` einbinden) |

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

Regelung über **`sensor.verbrauch_aktuell`** (MQTT „Verbrauch Aktuell“, OBIS 16.7.0): negatives W = Einspeisung → nutzbarer Überschuss `max(0,-W)`. **Zielstrom:** rechnerisch unter **6 A** → **0 A** (kein Mindestladen). **`input_boolean.wallbox_pv_regelung`:** aus = keine automatische Übergabe an ESP (manuell oder ESP-Modi). Die MQTT-**`unique_id`** ist **nicht** die Entity-ID. Optional: **`sensor.wallbox_lieferung_leistung`** (Ableitung Wh).

## Dashboard „Wallbox“

`configuration.example.yaml` enthält einen Block **`lovelace:`** mit Dashboard **`wallbox`** → Datei **`/config/dashboards/wallbox.yaml`**. Wenn **`lovelace:`** bei dir schon existiert, nur den Unterblock **`dashboards:`** zusammenführen. **Reihenfolge in der Seitenleiste:** *Einstellungen → Dashboards* → „Wallbox“ nach oben ziehen. Entity-IDs des ESPHome-Geräts (z. B. `number.*`) ggf. im Dashboard anpassen.

## Skript

`scripts/update-homeassistant-esphome.sh` kopiert Pakete, Automationen, optional **`homeassistant/dashboards/*.yaml`** nach `~/homeassistant/dashboards/` (und ESPHome nach `~/homeassistant/esphome/`).

## Service

`esphome.wallbox_set_charging_amps_auto` (ESPHome-Gerät `wallbox` verbunden).
