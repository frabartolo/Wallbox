# Home Assistant: Wallbox-Überschuss

## Einrichtung

1. Datei `packages/wallbox.yaml` nach `/config/packages/wallbox.yaml` kopieren.

2. In der `configuration.yaml` (oder einer eingebundenen Datei) **Packages** aktivieren, falls noch nicht geschehen:

   ```yaml
   homeassistant:
     packages: !include_dir_named packages/
   ```

3. In `packages/wallbox.yaml` den Platzhalter **`sensor.REPLACE_ME_HIER_EUREN_UEBERSCHUSS_SENSOR`** durch euren echten Sensor ersetzen (Leistung in **Watt**, idealerweise **positiver** Hausüberschuss / verfügbare PV-Leistung fürs Laden).

4. Home Assistant neu starten oder **„Konfiguration prüfen“** und **Neustart**.

5. **Entity-ID „Wallbox Modus“** prüfen: *Entwicklerwerkzeuge → Zustände* nach `modus` filtern (Gerät Wallbox). In der Automation ist standardmäßig `sensor.wallbox_wallbox_modus` eingetragen. Weicht die ID ab (z. B. `sensor.wallbox_modus`), in `wallbox.yaml` bei `entity_id` und in der **Bedingung** der Automation anpassen.

6. ESPHome-Gerät **wallbox** muss mit Home Assistant verbunden sein. Der Service lautet:

   `esphome.wallbox_set_charging_amps_auto`

## Verhalten

- **Modus 0 (Automatik)** auf dem ESP: Home Assistant schreibt alle **15 Sekunden** (und bei Änderung) den berechneten Zielstrom per Service.
- **Andere Modi** (Max, Überschuss-Default, Manuell): Der Service wird vom ESP ignoriert (`set_charging_amps_auto` wirkt nur bei Modus 0).

## Hilfsmittel

- **Eingaben** (Min/Max-Strom, 1×230 V / 3×230 V, Spannung) steuern die Umrechnung Watt → Ampere.
- Formel: \(I \approx P / (U \times \text{Phasen})\), danach Begrenzung auf Min/Max.
