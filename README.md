# ESP32 Wallbox Controller

Dieses Projekt steuert die Ladeleistung einer analogen EVSE (z.B. AnalogEVSE) über ein digitales Potentiometer (DS3502) und bietet:
- REST API zur Steuerung
- Web-Interface zur Anzeige und Override
- PV-abhängige Automatik

## Hardware
- ESP32
- Adafruit DS3502 (I2C)
- AnalogEVSE Platine mit Widerstandssteuerung

## Konfiguration
- WLAN-Daten direkt in `main.ino` eintragen
