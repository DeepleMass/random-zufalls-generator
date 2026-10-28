# bitfolgen - Rust-Implementierung des Papers

Implementierung des Papers *The Evaluation of Finite Bit Sequences*
(Referenz: `EvaluationOfFiniteBitSequences.pdf` im Repository-Root) in
Rust. Die Module folgen der Gliederung des Papers:

| Modul                | Paper-Abschnitt                                        |
|----------------------|--------------------------------------------------------|
| `bitfolge`           | 3: Bitfolgen, Laeufe, Laeuflerlegung                   |
| `einschritttest`     | 4: Einschritttest in beiden Varianten, Lookup-Tabelle   |
| `klassen`            | 6: Grundverteilung, Klassen B_N, Mitgliedschaft       |
| `generatoren`        | 7: Iterativer und rekursiver Generator                 |
| `komposition`        | 8: Zusammengesetzte Haeufigkeiten aus Binaerdarstellung |

Das Kommandozeilenwerkzeug `auswertung` (Paper-Abschnitt 5) wertet
einen Datenbestand von Sammeldateien aus:

    cargo run --release --bin auswertung -- <verzeichnis>

Die Dateien werden namenssortiert (= chronologisch bei
Zeitstempel-Dateinamen) zu einer einzigen Bitfolge verkettet; Laeufe
setzen sich ueber Datei- und Blockgrenzen fort. Die Ausgabe prueft die
Konsistenzbedingung l_1 = M und enthaelt die Hauefigkeitentabelle samt
Chi-Quadrat-Anpassungstest gegen die geometrische Verteilung.

## Bauen und Testen

    cargo build --release      # Kompilation
    cargo test                 # Beispiele, Lemmata und Saetze des Papers als Tests

Die Tests in `tests/paper_tests.rs` verifizieren unter anderem den
Satz zur Grundverteilung (N = 2..16), das Konstruktionslemma, beide
Identitaeten, das Kompositionsbeispiel B = 341, die Kardinalitaetsformel
fuer N <= 4 (vollstaendige Enumeration) und die Ordnungsbehauptungen
zu B_5.

## Konventionen

- Ein Kommentar je Anweisung erlaeutert Sinn und Zweck der Zeile.
- Alle Funktionen und Typen sind vollstaendig dokumentiert.
- Bezeichner sind ausgeschrieben und folgen dem Paper (Umlaute als
  ae/oe/ue).
- Byteweises Verarbeiten nutzt Lookup-Tabellen statt verschachtelter
  Fallunterscheidungen.
- Schleifen sind flach gehalten: fruehe Abbrueche statt weiterer
  Einrueckung.
