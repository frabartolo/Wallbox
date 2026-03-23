# Home Assistant: Wallbox-Überschuss

## Variante A – `packages/` (neu anlegen)

### 1) Ordner und Paket

```bash
mkdir -p /config/packages
cp wallbox.yaml /config/packages/wallbox.yaml
```

(Quelle: `homeassistant/packages/wallbox.yaml` aus diesem Repository.)

### 2) `configuration.yaml` – Block `homeassistant:`

Wenn **noch kein** `homeassistant:` existiert, diesen Block **einmal** ergänzen (z. B. direkt unter `default_config:`). Inhalt siehe **`configuration_wallbox.fragment.yaml`** im Repo – minimal:

```yaml
homeassistant:
  packages: !include_dir_named packages/
```

**Nur ein** Top-Level-Block `homeassistant:` – wenn du später weitere Einträge brauchst (`country`, `time_zone`, …), in **denselben** Block schreiben.

### 3) Konfiguration prüfen & Neustart

### 4) Entity-IDs prüfen

- **Überschuss:** Das Paket nutzt `sensor.solaranlage_erzeugung_aktuell` und `sensor.verbrauch_aktuell` (aus deinen MQTT-Namen). Unter **Entwicklerwerkzeuge → Zustände** gegenprüfen; bei abweichenden IDs die `state:`-Zeile bei **Wallbox PV Rohleistung** anpassen.
- **ESPHome-Modus:** Automation nutzt `sensor.wallbox_wallbox_modus`. Wenn die ID anders lautet, in der Automation unter `entity_id` und in der **Bedingung** anpassen.

### 5) Service

`esphome.wallbox_set_charging_amps_auto` (Gerät muss in HA eingebunden sein).

## Verhalten

- **Modus 0 (Automatik)** auf dem ESP: HA setzt den Strom (ca. alle **15 s** und bei Änderung).
- **Andere Modi:** Service wird vom ESP ignoriert.

## Hinweise zu deiner `configuration.yaml`

- **YAML unter `mqtt`:** Der Block für „Wallbox Ladeleistung“ / „Wallbox Modus“ ist falsch eingerückt (`- sensor:` mit zusätzlichem `- name:`). Korrekt ist dieselbe Form wie bei den anderen Sensoren, z. B.:

  ```yaml
      - sensor:
          name: "Wallbox Ladeleistung"
          state_topic: "wallbox/state"
          ...
  ```

  Wenn du die Wallbox über **ESPHome** anbindest, entfallen diese MQTT-Sensoren meist – sonst **doppelte** Entitäten / Namenskollision mit „Wallbox Modus“.

## Hilfsmittel

- **Eingaben** (Min/Max-Strom, 1×230 V / 3×230 V, Spannung) steuern Watt → Ampere.
- Näherung: \(I \approx P / (U \times \text{Phasen})\), begrenzt auf Min/Max.
