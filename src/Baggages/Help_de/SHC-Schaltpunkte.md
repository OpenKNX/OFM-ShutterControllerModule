### Schaltpunkte

Jeder Kanal hat 8 Schaltpunkte. Ein Schaltpunkt legt fest, an welchen Wochentagen und unter welcher Bedingung eine Stufe auslöst. Mehrere Schaltpunkte derselben Stufe sind ODER-verknüpft: Der erste erfüllte Schaltpunkt löst die Stufe aus.

- **Stufe**: Die Stufe, die der Schaltpunkt auslöst, oder "nicht aktiv".
- **Mo-So**: Die Wochentage, an denen der Schaltpunkt gilt. Für Abendstufen zählt der Tag, an dem der Nachtzyklus begonnen hat (eine Zeit nach Mitternacht am Freitag gehört noch zum Freitag). An Feiertagen und im Urlaub gelten je nach Einstellung "Feiertage" bzw. "Urlaub" die Einstellungen für Sonntag.
- **Auslöser**:
  - Uhrzeit
  - bei Sonnenuntergang / Sonnenaufgang
  - Sonnenuntergang / Sonnenaufgang minus bzw. plus Zeitversatz (hh:mm)
  - Ende bzw. Beginn der bürgerlichen Dämmerung (Sonne 6° unter dem Horizont), auch minus bzw. plus Zeitversatz
  - Ende bzw. Beginn der nautischen Dämmerung (Sonne 12° unter dem Horizont), auch minus bzw. plus Zeitversatz. Im Sommer erreicht die Sonne in nördlichen Breiten 12° unter dem Horizont teilweise nicht, dann löst dieser Auslöser nicht aus.
  - Sonnenuntergang / Sonnenaufgang über bzw. unter Horizont (Höhenwinkel in Grad)
  - dunkler als / heller als (Lux), nur wenn "Helligkeit im Nachtmodus" verwendet wird
- **Wert**: Uhrzeit, Zeitversatz, Höhenwinkel oder Lux, je nach Auslöser.
- **Helligkeit / Lux**: Verknüpft den Auslöser innerhalb des Schaltpunkts mit der Helligkeit: "und dunkler als" (beides muss erfüllt sein) oder "oder dunkler als" (eines genügt). Morgens entsprechend "heller als".
- **Bedingung / von / bis**:
  - frühestens um (von): verhindert ein Auslösen vor dieser Zeit.
  - spätestens um (von): löst zu dieser Zeit auch dann aus, wenn Auslöser und Helligkeit noch nicht erfüllt sind.
  - zwischen (von, bis): frühestens um "von", spätestens um "bis".
  - zufällig zwischen (von, bis): frühestens um "von"; sind Auslöser und Helligkeit bis dahin nicht erfüllt, löst der Schaltpunkt zu einem zufälligen Zeitpunkt zwischen "von" und "bis" aus. Der Zeitpunkt wird einmal pro Nachtzyklus (um 12:00 und nach einem Neustart) neu bestimmt und im Diagnose-Log ausgegeben.

Ausgewertet wird in dieser Reihenfolge: (Auslöser und/oder Helligkeit), danach die Bedingung.

Beispiele:

| Stufe | Tage | Auslöser | Helligkeit | Bedingung | Ergebnis |
|---|---|---|---|---|---|
| Nacht | Mo-So | bei Sonnenuntergang | oder dunkler als 20 Lux | spätestens um 22:00 | schließt bei Sonnenuntergang oder Dunkelheit, spätestens um 22:00 |
| Nacht | Mo-So | dunkler als 20 Lux | | frühestens um 17:00 | kein Schließen bei einem Gewitter am Nachmittag |
| Tag | Mo-Fr | bei Sonnenaufgang | | frühestens um 06:00 | Wochentags nicht vor 06:00 |
| Tag | Mo-Fr | bei Sonnenaufgang | und heller als 300 Lux | zufällig zwischen 07:00 und 08:00 | bei Helligkeit ab 07:00, sonst zu einer zufälligen Zeit bis 08:00 |
| Nacht | Mo-So | Ende bürgerliche Dämmerung plus Zeitversatz 00:10 | | | 10 Minuten nach Ende der bürgerlichen Dämmerung |
| Tag | Sa, So | Uhrzeit 08:30 | | | am Wochenende um 08:30 |
| Vorstufe Abend | Mo-So | Sonnenuntergang minus Zeitversatz 00:30 | | | 30 Minuten vor Sonnenuntergang |

Uhrzeiten von Abendstufen vor 12:00 gelten als "nach Mitternacht". Uhrzeiten von Morgenstufen ab 12:00 werden wie 11:59 behandelt.

**Vollständiges Beispiel** (ein Fenster mit allen vier Stufen und einer Wochenend-Variante):

| # | Stufe | Tage | Auslöser | Wert | Helligkeit | Lux | Bedingung | von | bis |
|---|---|---|---|---|---|---|---|---|---|
| 1 | Vorstufe Morgen | Mo-Fr | Beginn bürgerliche Dämmerung | | und heller als | 20 | frühestens um | 06:40 | |
| 2 | Tag | Mo-Fr | bei Sonnenaufgang | | und heller als | 300 | zufällig zwischen | 07:00 | 08:00 |
| 3 | Vorstufe Morgen | Sa, So | Beginn bürgerliche Dämmerung | | und heller als | 20 | frühestens um | 07:30 | |
| 4 | Tag | Sa, So | bei Sonnenaufgang | | und heller als | 300 | zufällig zwischen | 08:20 | 09:30 |
| 5 | Vorstufe Abend | Mo-So | Sonnenuntergang plus Zeitversatz 00:20 | | oder dunkler als | 300 | | | |
| 6 | Nacht | Mo-So | Ende bürgerliche Dämmerung | | oder dunkler als | 20 | | | |

Dazu passend die Stufen: Vorstufe Abend "Nur schließen" 70 % / Lamelle 50 %, Nacht "Nur schließen" 100 % / 100 %, Vorstufe Morgen "Nur öffnen" 70 % / 50 %, Tag "Nur öffnen" 0 % / 0 %.

Ergebnis: Werktags schließt der Behang abends 20 Minuten nach Sonnenuntergang (oder schon vorher bei einem dunklen Himmel) auf die Vorstufe, zur bürgerlichen Dämmerung ganz zu. Morgens öffnet er ab der Morgendämmerung auf die Vorstufe und ab Sonnenaufgang vollständig, spätestens aber zur zufällig gewählten Zeit zwischen 07:00 und 08:00 (am Wochenende zwischen 08:20 und 09:30), auch wenn es noch nicht hell genug ist. Die Schaltpunkte 1-4 greifen nicht, solange es z. B. an einem dunklen Wintermorgen nicht heller als 20 bzw. 300 Lux wird, bis die jeweilige Bedingung "frühestens um"/"zufällig zwischen" das Öffnen trotzdem auslöst.

