### Beschattungsposition

Position die bei Beschattungsstart angefahren wird.

Bei den Modi **Geometrische Positionsnachführung**, **Geometrische Positions- und Lamellennachführung** und **Geo. Positions- und Lamellennachführung (Min/Max)** wird die Position normalerweise dynamisch anhand des Sonnenstands berechnet. Die Beschattungsposition wirkt in diesen Modi nur als **Fallback**: Wenn die Sonne so flach steht, dass die berechnete Schattenkante oberhalb der Fensterhöhe liegt (d.h. der Behang muss vollständig abgesenkt sein), wird die hier konfigurierte Position als Zielwert verwendet.

**Schutzposition:** Bei Beschattungsstart kann die Sonne zwar im konfigurierten Azimut- und Helligkeitsfenster liegen, aber geometrisch noch nicht auf die Fassade treffen (z.B. Westfenster, Sonne kommt noch aus Südosten). In diesem Fall fährt der Behang sofort auf die Beschattungsposition als Schutz gegen indirektes Licht und Reflexionen. Sobald die Sonne die Fassade trifft, übernimmt die geometrische Nachführung.

Hinweis: Die Beschattungsposition wird beim Start **nicht** durch Min./Max.-Begrenzungen eingeschränkt. Soll dieses Verhalten vermieden werden, kann unter **Beschattungsstart** die Option "Nur bei direktem Sonnenlicht" gewählt werden.

