# Home Assistant: Wallbox-Überschuss

Die Logik liegt in **`packages/wallbox.yaml`** im Repository. Du musst **kein** `packages`-Verzeichnis haben – du legst es bei Bedarf neu an.

## Variante A – Ordner `packages/` (empfohlen)

1. Verzeichnis anlegen und Datei kopieren:

   ```bash
   mkdir -p /config/packages
   cp wallbox.yaml /config/packages/wallbox.yaml
   ```

   (Quelle: diese Datei aus dem Repo `homeassistant/packages/wallbox.yaml`.)

2. **`configuration.yaml` anpassen:** Unter dem Schlüssel `homeassistant:` muss **Packages** eingetragen sein. Gibt es den Block noch nicht, z. B.:

   ```yaml
   homeassistant:
     packages: !include_dir_named packages/
   ```

   **Wichtig:** Es darf nur **einen** Top-Level-Block `homeassistant:` geben. Wenn dort schon z. B. `name:` oder `latitude:` steht, **eine Zeile ergänzen** (gleiche Einrückung wie die anderen Einträge):

   ```yaml
   homeassistant:
     name: Home
     packages: !include_dir_named packages/
     # … rest wie bisher
   ```

3. In `/config/packages/wallbox.yaml` den Platzhalter **`sensor.REPLACE_ME_HIER_EUREN_UEBERSCHUSS_SENSOR`** durch euren Sensor ersetzen (Überschuss in **Watt**).

4. **Konfiguration prüfen** und Home Assistant **neu starten**.

## Variante B – eine Datei direkt unter `/config`

Wenn du **keinen** Ordner `packages/` willst:

1. Die Repo-Datei nach z. B. **`/config/wallbox_ha.yaml`** kopieren.

2. In **`configuration.yaml`** (unter `homeassistant:`) einbinden:

   ```yaml
   homeassistant:
     packages:
       wallbox: !include wallbox_ha.yaml
   ```

   Bestehende `packages:`-Einträge mit weiteren Zeilen erweitern, nicht doppeln.

3. Platzhalter-Sensor und Modus-Entity-ID wie in Variante A anpassen.

## Danach immer

- **Entity-ID „Wallbox Modus“** prüfen (*Entwicklerwerkzeuge → Zustände*). In der Automation steht `sensor.wallbox_wallbox_modus` – bei Abweichung in `wallbox.yaml` anpassen.
- ESPHome-Gerät **wallbox** verbunden; Service: **`esphome.wallbox_set_charging_amps_auto`**.

## Verhalten

- **Modus 0 (Automatik)** auf dem ESP: Home Assistant setzt den Strom per Service (ca. alle **15 s** und bei Änderung).
- **Andere Modi:** Der ESP ignoriert `set_charging_amps_auto`.

## Hilfsmittel

- **Eingaben** (Min/Max-Strom, 1×230 V / 3×230 V, Spannung) steuern Watt → Ampere.
- Näherung: \(I \approx P / (U \times \text{Phasen})\), begrenzt auf Min/Max.
