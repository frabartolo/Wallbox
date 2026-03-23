# Home Assistant: Wallbox-Überschuss

Das Paket liegt **neben** der `configuration.yaml`, typisch:

```text
/config/configuration.yaml
/config/packages/wallbox.yaml
```

## Einbindung

In **`configuration.yaml`** muss **ein** Block `homeassistant:` existieren, z. B.:

```yaml
homeassistant:
  packages: !include_dir_named packages/
```

(Vorlage: **`configuration_wallbox.fragment.yaml`** im Repository.)

## Messgröße (Volkszähler / MQTT)

Für die **Momentanleistung** wird **`sensor.16-7-0-VerbrauchAktuellW`** verwendet (OBIS 1-0:16.7.0).  
**Negative Werte** bedeuten Einspeisung; daraus wird der **verfügbare Überschuss in Watt** berechnet: `max(0, -Leistung)`.

**`sensor.2-8-0-ZaehlerLieferungWh`** ist ein **Zählerstand** (Energie in Wh), kein Momentanwert – für diese direkte Wallbox-Regelung **nicht** geeignet (höchstens über eine Ableitung; Standard ist der Leistungssensor oben).

Die **MQTT-Sensoren** für den Stromzähler bleiben in der `configuration.yaml` – sie liefern die Daten für dieses Paket.

## Nach dem Kopieren

1. `packages/wallbox.yaml` aus dem Repo nach `/config/packages/wallbox.yaml` legen (oder aktualisieren).  
2. **Konfiguration prüfen** & HA **neu starten**.  
3. Entity-ID **Wallbox Modus** (ESPHome) prüfen – in der Automation steht `sensor.wallbox_wallbox_modus`; bei Abweichung anpassen.

## Service

`esphome.wallbox_set_charging_amps_auto` (ESPHome-Gerät `wallbox` verbunden).

## Verhalten

- **Modus 0 (Automatik)** auf dem ESP: HA setzt den Strom (ca. alle **15 s** und bei Änderung von Zielstrom, Modus oder **Momentanleistung**).
- **Andere Modi:** Service wird vom ESP ignoriert.

## Hilfsmittel

- **Eingaben** (Min/Max-Strom, 1×230 V / 3×230 V, Spannung): Umrechnung W → A, begrenzt auf Min/Max.
