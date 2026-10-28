Experiment-Verzeichnis
======================

Zugehoerig zu Abschnitt 5 (Ein motivierendes Experiment) des Papers
in ../latex/.

Dateien
-------
  sammle.sh     Erfassungsskript (Rekonstruktion des Originals):
                stuendlich ein Block von bis zu 1024 Bytes aus
                /dev/random, Dateiname = Zeitstempel im Format
                YYYY_MM_DD_HH_MM_SS.

  auswertung.py Auswertung: liest alle Dateien eines Verzeichnisses
                namenssortiert (= chronologisch), verkettet sie zu
                einer Bitfolge (MSB zuerst je Byte) und berechnet die
                kumulierte Verteilung der Laeufe und Teillaeufe
                (Einschritttest). Die Konsistenzbedingung l_1 = M
                wird automatisch geprueft.

Datenbestand
------------
  Der Originalbestand liegt im Repository unter dev/random:
  9856 Dateien (9277 davon exakt 1024 Bytes, der Rest kurze Reads
  des Entropie-Pools), zusammen 9.793.146 Bytes = 78.345.168 Bits,
  Erhebungszeitraum 13.11.2016 bis 15.02.2018.

Ergebnis (python3 auswertung.py ../../dev/random)
------------------------------------------------
  M = 78.345.168 Bits, laengster Lauf P = 25,
  l_1 = M exakt, Halbierungsreihe durchgehend,
  Chi-Quadrat-Anpassungstest der Laufverteilung gegen die
  geometrische Verteilung: chi2 = 19,8 bei 22 Freiheitsgraden
  (kritischer Wert zum Niveau 5%: 33,9),
  Uebergangswahrscheinlichkeit p = 0,49994 +- 0,00006.
  Die graphische Darstellung (../latex/haeufigkeiten.dat) wird im
  Paper per pgfplots direkt aus den Daten gezeichnet.
