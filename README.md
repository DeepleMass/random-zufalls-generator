Erläuterung der Begrifflichkeiten
Eine Binäre Folge ist ein beliebiger, binärer (endlicher) Datensatz oder Datenstrom.
Ein Bitblock ist eine Teilfolge gleichwertiger Bits in einer binären Folge

Beispiel:
Binäre Folge
  1100010100111010111001110100111001010001110011001110
Die selbe binäre Folge in Bitblöcke aufgeteilt
  11 000 1 0 1 00 111 0 1 0 111 00 111 0 1 00 111 00 1 0 1 000 111 00 11 00 111 0

###################################################

Dieser Git Repository enthält ein LINUX Werkzeug zur Überprüfung einer binären Folge auf Zufälligkeit

###################################################

kvvef : Liefert die Verteilung der kumulierten Häufigkeiten der Bitblöcke einer binären Folge

Nutzung: $ kvvef <Bitlänge> [Eingabe] [Ausgabe]

Parameter 
  <Bitlänge> ist die Länge der binären Folge in Bits (nicht in Bytes! Ggf muss eine Multiplikation mit 8 vorangestellt werden). Wird 0 angegeben, so wird die Eingabe bis EOF ausgewertet.
  [eingabe] optionale Angabe einer Datei, die die binäre Folge enthält. Der default ist stdin
  [Ausgabe] optionale Angabe einer Datei, die die Verteilung der kumulierten Häufigkeiten der Bitblöcke der binären Folge enthält. Der default ist stdout. Der erste Eintrag der Auswertung ist IMMER die vorgegebene Bitlänge, wenn nicht 0 angegeben wurde. Wenn 0 angegeben wurde, ist der erste Eintrag die Länge der Eingabe bis zum EOF, jedoch maximal 9.223.372.036.854.775.807 Bits. Diese Obergrenze ist durch die 64 Bits Architektur der heutigen Rechner bedingt und wird in den nächsten Versionen durch die Verwendung von Big Integer überwunden.

Beispiel mit der gelieferten Datei etef56285.bin

Auswertung: $ kvvef 56285 etef56285.bin kvvef56285.vert
Ausgabe:    $ cat kvef56285.vert
  1 : 56285
  2 : 28142
  3 : 14071
  4 :  7035
  5 :  3517
  6 :  1758
  7 :   879
  8 :   439
  9 :   219
 10 :   109
 11 :    54
 12 :    27
 13 :    13
 14 :     6
 15 :     3
 16 :     1

###################################################

Zufälligkeit:
Als zufällig werden die Bitfolgen bezeichnet, deren Häufigkeitsverteilung der zugehörigen Vorgabe entspricht. Die Auswertung des mitgelieferten Beispiel liefert 


$ cat kvef56285.vert
  1 : 56285
  2 : 28142
  3 : 14071
  4 :  7035
  5 :  3517
  6 :  1758
  7 :   879
  8 :   439
  9 :   219
 10 :   109
 11 :    54
 12 :    27
 13 :    13
 14 :     6
 15 :     3
 16 :     1

Ein Vergleich beider Ausgaben erfolgt mit 
$ cmp -l kvvef56285.vert kvef56285.vert
bzw.
$ cmp -l kvvef56285.vert kvef56285.vert && echo ok || echo nok
Die Ausgabe muss leer bzw ok sein

Die zu einer bestimmten Bitlänge zugehörigen Vorgabe wird auf Anfrage geliefert.
