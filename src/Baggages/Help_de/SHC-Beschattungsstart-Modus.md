### Beschattungsstart

Legt fest, unter welcher Bedingung die Beschattung gestartet wird, wenn alle Mess- und Sonnenwert-Freigaben erfüllt sind. Diese Einstellung ist nur bei aktivem Geo-Tracking sichtbar (Jalousie: Modi 3–6; Rollo: Modi 1–2).

**Sofort (mit Schutzposition)** (Standard):
Die Beschattung startet sofort, sobald Azimut, Elevation und Helligkeitswerte die konfigurierten Grenzen erfüllen. Es ist dabei möglich, dass die Sonne geometrisch noch nicht direkt auf die Fassade trifft (z.B. beim Westfenster kommt die Sonne zunächst noch aus Südosten). In diesem Fall fährt der Behang unmittelbar auf die konfigurierte **Beschattungsposition** als Schutz gegen indirektes Licht und Reflexionen. Sobald die Sonne die Fassade trifft (Profilwinkel > 0°), übernimmt die geometrische Nachführung die Positionsberechnung.

**Nur bei direktem Sonnenlicht**:
Die Beschattung startet erst, wenn die Sonne sowohl alle Mess- und Sonnenwert-Bedingungen erfüllt als auch geometrisch auf die Fassade trifft (Profilwinkel > 0°). Solange die Sonne noch nicht auf die Fassade trifft, bleibt der Behang geöffnet – die Beschattungsposition wird nicht angefahren.

**Beispiel Westfenster** (Azimut 120°–290°): Die Sonne trifft eine Westfassade erst ab ca. 180° Azimut.
- *Sofort*: Behang fährt ab 120° auf Beschattungsposition, geometrische Nachführung ab ~180°.
- *Nur bei direktem Sonnenlicht*: Behang bleibt bis ~180° vollständig geöffnet.

