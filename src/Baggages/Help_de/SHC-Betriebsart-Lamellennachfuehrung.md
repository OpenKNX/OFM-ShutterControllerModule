### Betriebsart Lamellennachführung

Legt fest, wie die Lamellenstellung automatisch berechnet wird:

- **Direktlicht abschneiden (Cut-Off)**: Geometrische Berechnung des kritischen Kippwinkels θ, bei dem gerade kein direktes Sonnenlicht durch die Lamellen eindringt (Schattenkante). Hierfür werden Lamellenbreite und Lamellenabstand benötigt.
- **Blendschutz (Retro-Reflexion)**: Die Lamellen werden so gestellt, dass Sonnenstrahlen parallel reflektiert werden (θ = 90° − Profilwinkel). Maximaler Blendschutz ohne geometrische Kalibrierung.
- **Adaptiv (Daylight)**: Kombiniert Cut-Off und Retro-Reflexion automatisch. Wählt jeweils die offenere Lamellenstellung (θ = min(θ_CutOff, θ_Retro)). Maximale Tageslichtnutzung bei gleichzeitigem Blendschutz — der Wechsel zwischen beiden Betriebsarten erfolgt am geometrischen Schnittpunkt, sprungfrei und ohne zusätzliche Parameter.
- **Über Tabelle**: Die Lamellenstellung wird anhand einer konfigurierbaren Tabelle mit 6 Stützpunkten (Höhenwinkel → Position in %) per linearer Interpolation berechnet. Geeignet für herstellerspezifische Vorgaben (z. B. Warema-Tabellen).

