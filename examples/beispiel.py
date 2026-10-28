"""Beispiel: Die Rust-Bibliothek `bitfolgen` aus Python nutzen.

Voraussetzung: installierte Bindings (siehe Makefile, Ziel
`python-install` oder `python-venv`).

Aufruf: python3 examples/beispiel.py
"""

# Die Rust-Implementierung des Papers als Python-Modul importieren.
import bitfolgen

# --- Abschnitt 6: Grundverteilung durch Abgeltung -----------------

# Die kumulierte Verteilung der Klasse B_5 (16, 8, 4, 2, 1).
kumulierte_verteilung = [16, 8, 4, 2, 1]
# Die Grundverteilung durch Abgeltung bestimmen (Paper-Beispiel).
grundverteilung = bitfolgen.grundverteilung_aus_kumulierter(kumulierte_verteilung)
# Das Paper erwartet die Grundverteilung (4; 2; 1; 0; 1).
assert grundverteilung == [4, 2, 1, 0, 1], grundverteilung
# Das Ergebnis der Abgeltung ausgeben.
print("Grundverteilung von B_5:", grundverteilung)

# --- Abschnitt 7: Generatoren und Mitgliedschaft -------------------

# Die Lauffolge des Klassenvertreters von B_7 erzeugen.
lauffolge = bitfolgen.iterativer_generator(7)
# Der Vertreter muss Mitglied der Klasse sein.
assert bitfolgen.ist_mitglied(lauffolge, 7)
# Den rekursiven Uebergang nach B_8 anwenden (Lemma der Konstruktion).
zielfolge = bitfolgen.rekursiver_uebergang(lauffolge)
# Das Ergebnis muss Mitglied der Folgewklasse sein.
assert bitfolgen.ist_mitglied(zielfolge, 8)
# Die Laenge des Vertreters von B_7 ausgeben (Erwartung: 64 Bits).
print("Vertreter von B_7:", lauffolge)

# --- Abschnitt 8: Komposition aus Binaerdarstellungen --------------

# Die Beispielzahl des Papers: 341 = 101010101_2.
beispielzahl = 341
# Die zusammengesetzte Verteilung bestimmen.
verteilung = bitfolgen.zusammengesetzte_haeufigkeit(beispielzahl)
# Das Paper tabelliert (86, 42, 22, 10, 6, 2, 2, 0, 1).
assert verteilung == [86, 42, 22, 10, 6, 2, 2, 0, 1], verteilung
# Die gewichtete Gesamtlaenge muss wieder die Zahl selbst sein.
assert bitfolgen.zusammengesetzte_laenge(beispielzahl) == beispielzahl
# Die kumulierte Haeufigkeit C_1 muss 171 sein (Identitaet des Papers).
assert bitfolgen.kumulierte_zusammengesetzte_haeufigkeit(beispielzahl, 1) == 171
# Die zusammengesetzte Verteilung ausgeben.
print("Zusammengesetzte Verteilung von B = 341:", verteilung)

# --- Abschnitt 4/5: Einschritttest auf rohen Bytes -----------------

# Ein Beispielblock aus rohen Bytes (identisch zu Bitfolge-Auswertung).
beispielbytes = bytes([0b01001100, 0b11000001, 0b11111111])
# Das Lauf-Histogramm ueber die Bytes bestimmen (schneller Pfad).
histogramm = bitfolgen.laufhistogramm_aus_bytes(beispielbytes)
# Die kumulierte Verteilung aus dem Histogramm bestimmen.
kumulierte_aus_bytes = bitfolgen.kumulierte_verteilung_aus_histogramm(histogramm)
# Die zugehoerige Bitfolge zum Vergleich bestimmen (MSB zuerst je Byte).
bits = [b for zeichen in beispielbytes for b in [(zeichen >> 7) & 1,
        (zeichen >> 6) & 1, (zeichen >> 5) & 1, (zeichen >> 4) & 1,
        (zeichen >> 3) & 1, (zeichen >> 2) & 1, (zeichen >> 1) & 1,
        zeichen & 1]]
# Der klassische Einschritttest auf derselben Folge.
kumulierte_aus_bits = bitfolgen.einschritttest_bitfolge(bits)
# Beide Wege muessen dasselbe Ergebnis haben (Aequivalenzlemma).
assert kumulierte_aus_bytes == kumulierte_aus_bits
# Die Konsistenzbedingung l_1 = M muss gelten.
assert kumulierte_aus_bytes[0] == len(bits)
# Die Auswertung des Beispielblocks ausgeben.
print("Kumulierte Verteilung der Beispielbytes:", kumulierte_aus_bytes)

# Alles bestanden: Abschlussmeldung ausgeben.
print("Alle Beispiele bestanden.")
