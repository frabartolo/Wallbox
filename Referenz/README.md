# Referenzdokumente

Lege hier optional ab:

- `AnalogEVSE.pdf` – Schaltung / CP-Leitung (IEC 61851)
- `ds3502_Datenblatt.pdf` – Maxim DS3502

## Abgleich: lineare Wiper-Ansteuerung

**DS3502 (Datenblatt):** Der Baustein ist ein 128-Stufen-Digitalpoti (Wiper 0…127). Zwischen Anschlüssen H–W–L verhält sich der eingestellte Widerstand in der Regel **proportional zur Wiper-Stellung** (typisch für solche Bausteine). Eine **lineare Abbildung** „Sollstrom → Wiper“ ist daher fachlich stimmig, sofern die **AnalogEVSE-Schaltung** den Strom ebenfalls in guter Näherung proportional zum eingestellten Widerstand vorgibt.

**AnalogEVSE / Norm:** Der Pilotstrom wird über die CP-Beschaltung festgelegt. Ob die Beziehung **Strom ↔ Widerstand** exakt linear ist, hängt von der konkreten Beschaltung (Spannungsteiler, feste Widerstände, Toleranzen) ab. Wenn die Praxis zeigt, dass bestimmte Ströme leicht danebenliegen, kann man gezielt **einzelne Kalibrierpunkte** ergänzen – für den Start ist **linear über den gesamten Bereich** aber üblich und wartbar.

**Im Projekt:** Es wird **eine** lineare Zuordnung genutzt: nominell 0…64 A → Wiper 0…127 (wie in `wallbox.yaml` im Template-Output). Die C++-Komponente setzt beim Start denselben Zusammenhang wie die YAML-Logik.
