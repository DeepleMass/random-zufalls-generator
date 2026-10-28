# bitfolgen - Rust-Implementierung des Papers

Implementierung des Papers *The Evaluation of Finite Bit Sequences*
(Referenz: `EvaluationOfFiniteBitSequences.pdf` im Repository-Root) in
Rust. Das Projekt ist ein Cargo-Workspace:

```
bitfolgen/              Kernbibliothek + CLI `auswertung`
bindings/python/        Python-Bindings (PyO3, Modulname `bitfolgen`)
examples/beispiel.py    Nutzung der Bindings aus Python
Makefile                Bauen, Testen, Python-Rad bauen und installieren
```

## Module der Kernbibliothek

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
setzen sich ueber Datei- und Blockgrenzen fort.

## Bauen und Testen (Makefile)

    make build       # Debug-Bau des Workspace
    make release     # Release-Bau (Standardziel)
    make test        # alle Tests: Beispiele, Lemmata und Saetze des Papers
    make pruefen     # cargo check ohne Artefakte

Die Tests in `bitfolgen/tests/paper_tests.rs` verifizieren unter
anderem den Satz zur Grundverteilung (N = 2..16), das
Konstruktionslemma, beide Identitaeten, das Kompositionsbeispiel
B = 341, die Kardinalitaetsformel fuer N <= 4 (vollstaendige
Enumeration) und die Ordnungsbehauptungen zu B_5.

## Einbindung in Python

Die Bibliothek steht ueber PyO3 als Python-Modul `bitfolgen`
bereit (Voraussetzung: `brew install maturin`):

    make python-install      # Rad bauen und nutzerweit installieren

(Variante fuer virtuelle Umgebungen: `make python-venv`.)

Danach in Python:

    import bitfolgen
    bitfolgen.grundverteilung_aus_kumulierter([16, 8, 4, 2, 1])  # [4, 2, 1, 0, 1]
    bitfolgen.iterativer_generator(7)
    bitfolgen.ist_mitglied([...], 7)

Fuer grosse Datenmengen nimmt `laufhistogramm_aus_bytes(daten)`
rohe `bytes` direkt entgegen (je Byte MSB zuerst; Laeufe greifen
ueber Blockgrenzen) und gibt das Lauf-Histogramm zurueck;
`kumulierte_verteilung_aus_histogramm` vollendet die Auswertung.
Waehrend der Byte-Verarbeitung wird der Python-Interpreter
freigegeben (GIL-frei). Ein vollstaendiges Beispiel:
`python3 examples/beispiel.py`; alle Doc-Kommentare erscheinen in
`help(bitfolgen)`.

## Konventionen

- Ein Kommentar je Anweisung erlaeutert Sinn und Zweck der Zeile.
- Alle Funktionen und Typen sind vollstaendig dokumentiert.
- Bezeichner sind ausgeschrieben und folgen dem Paper (Umlaute als
  ae/oe/ue).
- Byteweises Verarbeiten nutzt Lookup-Tabellen statt verschachtelter
  Fallunterscheidungen.
- Schleifen sind flach gehalten: fruehe Abbrueche statt weiterer
  Einrueckung.
