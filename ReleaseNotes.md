v 0.6.3
- Feature: Neuer Beschattungsmodus 6 „Geo. Positions- und Lamellennachführung (Min/Max)" – entspricht Modus 5 mit konfigurierbarer Min./Max.-Begrenzung für Position und Lamellenstellung. Wenn Min. Position > Max. Position konfiguriert ist, wird die Positionsbegrenzung ignoriert. Die Lamellenstellungsbegrenzung verwendet automatisch min/max der beiden Werte (Reihenfolge egal).
- Breaking: Parameterlayout des Beschattungsmodus-Blocks vergrößert (increment 50 → 52). Betrifft nur Kanäle mit zwei oder mehr konfigurierten Beschattungsmodi. Bestehende ETS-Projekte mit nur einem Beschattungsmodus pro Kanal sind nicht betroffen.
- Rename: Beschattungsmodus 4 umbenannt: „Schattenkantennachführung" → „Geometrische Positionsnachführung". Beschattungsmodus 5 umbenannt: „Schattenkanten- und Lamellenführung" → „Geometrische Positions- und Lamellennachführung". Die Parameterwerte (4, 5) sind unverändert.
- Fix: Kritischer Kippwinkel (theta_krit) wurde fälschlicherweise als 90°−atan2(...) berechnet; korrekt ist direkt atan2(...) (Winkelkonvention zur Senkrechten).
- Fix: Bei Dachflächen-Orientierung wurde sin_alpha ohne Betrag berechnet, was bei negativen Neigungswinkeln zu falschen Positionen führte (jetzt fabsf).
- Fix: OffsetSlatPosition wurde in Modus 5 nicht auf die berechnete Lamellenstellung angewendet.
- Fix: Hysterese (MinChangeForSlatAdaption) fehlte in Modus 2 (Standard-Lamellennachführung) vor dem Setzen der Lamellenstellung.
- Change: Wertebereich „Max. Eindringtiefe" angepasst: 1–200 → 10–255 cm.
v 0.6.2
- Feature: Neue Beschattungsmodi 3 „Lamellennachführung Experte", 4 „Schattenkantennachführung", 5 „Schattenkanten- und Lamellennachführung"
- Feature: Schattenkantennachführung für Rollo (neuer Parameter „Positionsnachführung" im Rollo-Kanal)
- Feature: Neuer Kanal-Parameter „Fassadenneigung/Fensterneigung" für geometrische Berechnung
- Breaking/Rename: Beschattungsmodus-Lamellensteuerung Wert 2 umbenannt: „Benutzerdefiniert" → „Lamellennachführung Min/Max". Der Parameterwert (2) ist unverändert. Bestehende ETS-Projekte laufen ohne Migration weiter; lediglich der angezeigte Text im ETS-Dropdown ändert sich nach einem Update der knxprod.
v 0.6.1
- Feature: Positionsprüfung im Status „Beschattung Bereit (Benutzer)" – Beschattungsbereitschaft wird nur signalisiert, wenn die aktuelle Zielposition ≤ dem konfigurierten Grenzwert „Nur wenn Position kleiner als" ist
- Feature: KO-Freigabe-Parameter je Kanal – alle optionalen KOs können nun einzeln in der ETS ein-/ausgeblendet werden (Sperren Kanal, Status aktiver Modus, Aktorrückmeldung, Beschattung ein/aus, Beschattung aktiv, Beschattungsbereitschaft, Handbetrieb Schalten, Handbetrieb Position, Fenster offen/gekippt Status und Sperren)
- Feature: KO-Freigabe-Parameter je Beschattungsmodus – Sperren, Status Aktiv und Status Bereitschaft je Beschattungsmodus separat freigebbar
- Feature: KO-Freigabe-Parameter je Fensterkontakt – Status und Sperren für Fenster offen / Fenster gekippt separat freigebbar
- Rename: KO-Freigabe-Parameter für Beschattungsmodus konsistent benannt (PPP+53 = Sperren freigeben, PPP+54 = Bereitschaft freigeben, PPP+55 = Status Aktiv freigeben)
v 0.6.0
- Refactor: "Helligkeit" (bool) + "Weitere Helligkeitssensoren" (count) zusammengefasst zu "Helligkeitssensoren" (Enum: Nein/1-5 Sensoren)
- Rename: "Helligkeit Sensor 1..5" -> "Ausrichtung Sensor 1..5"
- Breaking: Offset 19 Semantik geaendert (SHC_VerifyVersion 0.5 -> 0.6)
v 0.5.0
- Feature: Neues KO "Status Beschattung Bereit" je Kanal
- Feature: Dachflaeche bevorzugt unzugeordnete Helligkeitssensoren (z.B. Dachsensor)
- Feature: Wiederherstellung der vorherigen Position nach Fenster offen/gekippt
- Fix: Azimut-/Helligkeit-UI klarer (Helligkeit Sensor 1..5, "Keine Himmelsrichtung (Azimut-Auswertung aus)")
- Fix: Schreibfehler und Himmelsrichtungsbezeichnungen korrigiert
- Doc: Help-Context und Applikationsbeschreibung aktualisiert
v 0.4.2
- Fix: Stopp von Rolladen bei manueller Bedienung durch Jalousiensteuerung
v 0.4.1
- Feature: Bessere Bennenung der Gruppenobjekte
v 0.4.0
- Feature: Invertieren der Fensterkontakt KO
- Feature: Auswahl verhalten der Fensterkontakte
- Feature: Wartezeit für Fensterkontaktauswertung
- Fix: Das setzen einer Sperre führte zum Hänger des gesamten Gerätes, das ist raus, aber wahrscheinlich geht es besser, ich steige nur nicht durch, was zu machen wäre.
- Fix: die KOs für offen und kipp waren vertauscht
- Fix: Wenn man mehrmals zwischen kipp und offen wechselt, fährt der Rollladen beim Schließen nicht in die Position vor dem öffnen, sondern in eine der kipp- oder offen-Positionen.
- Fix: Gruppe offen / gekippt in der ETS Baumansicht vertauscht
v 0.3.0
- Fix: Handsteuerungslogik 
v 0.2.0
- Bugfix: Lammellen-KO wird bei Gerätetpye 'Rollo' angezeigt
- Bugfix: Position anfahren bei Fenster offen/gekippt
- Feature: Sonderfunktionen Tasterbedienung bei geschlossenen Jalousien
- Feature: Neue Sonderfunktion "Beschattung Ein" und "Beschattung Aus"
- Feature: Aussperrverhinderung
