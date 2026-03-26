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
| **`packages/wallbox.yaml`** | 3×230 V fest; `verbrauch_aktuell` → 0; Anzeigen **kW**; Automation: **`automations/wallbox.yaml`** |
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

**Regelung (Null am Netz):** `sensor.verbrauch_aktuell` = Saldo wie im Zählerdiagramm (**Bezug +** / **Lieferung −**). Soll-Ladeleistung (W): **`max(0, min(P_max, −w))`** — Einspeisung erhöht die Ladeleistung, Bezug senkt sie auf 0. **Fest 3×230 V**, **P_max** aus **Maximalstrom × U × 3**. **Zielstrom:** gerundeter Außenleiterstrom bis **`wallbox_max_amps`**; unter **~0,5 A** Rohwert → **0 A**. **`wallbox_min_amps`** u. a. für **`sensor.wallbox_evse_mindestleistung`**. **`input_boolean.wallbox_pv_regelung`:** aus = keine Übergabe an ESP.

## Dashboard „Wallbox“

`configuration.example.yaml` enthält einen Block **`lovelace:`** mit Dashboard **`wallbox`** → Datei **`/config/dashboards/wallbox.yaml`**. Wenn **`lovelace:`** bei dir schon existiert, nur den Unterblock **`dashboards:`** zusammenführen. **Reihenfolge in der Seitenleiste:** *Einstellungen → Dashboards* → „Wallbox“ nach oben ziehen.

### „Entität nicht gefunden“ im Dashboard

1. **Konfiguration prüfen** und **Neu starten**, damit **`packages/wallbox.yaml`** und **`packages/mqtt_volkszaehler.yaml`** geladen sind (sonst fehlen z. B. `sensor.verbrauch_aktuell`, `input_boolean.wallbox_pv_regelung`).
2. **Entwicklerwerkzeuge → Zustände** → nach **„wallbox“** filtern: die **Entity-ID** in der YAML muss **exakt** der in HA stehen (z. B. `sensor.wallbox_netz_leistung` aus dem Namen „Wallbox Netz-Leistung“).
3. **ESPHome:** Gerätenamen erzeugen oft **`sensor.wallbox_wallbox_…`** oder nur **`sensor.wallbox_…`**. Fehlen **Tasten** oder **Ladeleistung**, in der Geräteansicht des ESPHome-Geräts die **echten IDs** ablesen und in **`dashboards/wallbox.yaml`** ersetzen.
4. **MQTT:** `sensor.verbrauch_aktuell` kommt aus **`mqtt_volkszaehler.yaml`** (MQTT-Integration muss laufen, Topic wie bei dir konfiguriert). Ohne diesen Sensor bleiben Regelung und manche Templates unbrauchbar.

Siehe auch **Protokoll** (*Einstellungen → System → Protokoll*) bei Template-Fehlern nach einem YAML-Reload.

## Skript

`scripts/update-homeassistant-esphome.sh` kopiert Pakete, Automationen, optional **`homeassistant/dashboards/*.yaml`** nach `~/homeassistant/dashboards/` (und ESPHome nach `~/homeassistant/esphome/`).

## Service

`esphome.wallbox_set_charging_amps_auto` (ESPHome-Gerät `wallbox` verbunden).
