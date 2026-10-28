//! Python-Bindings der Rust-Bibliothek `bitfolgen` (PyO3).
//!
//! Dieses Crate baut die Kernbibliothek als Python-Erweiterungsmodul
//! mit dem Namen `bitfolgen`. Die Funktionen spiegeln die API der
//! Rust-Bibliothek eins zu eins; die Doc-Kommentare erscheinen als
//! Docstrings in Python (`help(bitfolgen)`).
//!
//! Bau und Installation siehe Makefile (`make python`) bzw. README.

use pyo3::prelude::*;

/// Einschritttest, Variante fuer Bitfolgen (Algorithmus 2 des
/// Papers): Zaehlt bei jedem Bit alle Stellen 1 bis z, wobei z die
/// Laenge des bis zu diesem Bit begonnenen Laufs ist.
///
/// * `bits` - Liste der Bits (0 oder 1) der auszuwertenden Folge.
/// * Rueckgabe: Liste der kumulierten Haeufigkeiten $l_p$ (Index
///   $p-1$ entspricht der (Teil-)Lauf-Laenge $p$).
#[pyfunction]
fn einschritttest_bitfolge(bits: Vec<u8>) -> Vec<u64> {
    // Die Rust-Variante des Einschritttests auf der Bitfolge ausfuehren.
    kernbibliothek::einschritttest::einschritttest_bitfolge(&bits)
}

/// Einschritttest, Variante fuer Folgen von Laeufen (Algorithmus 1
/// des Papers): Jeder Lauf der Laenge $l$ erhoeht die Stelle $i$ um
/// $l - i + 1$.
///
/// * `lauflaengen` - Liste der Lauf-Laengen der auszuwertenden Folge.
/// * Rueckgabe: Liste der kumulierten Haeufigkeiten $l_p$.
#[pyfunction]
fn einschritttest_laeufe(lauflaengen: Vec<u64>) -> Vec<u64> {
    // Die Rust-Variante des Einschritttests auf der Lauffolge ausfuehren.
    kernbibliothek::einschritttest::einschritttest_laeufe(&lauflaengen)
}

/// Die Lauf-Laengen einer Bitfolge (Lauffolge des Papers,
/// Abschnitt 3).
///
/// * `bits` - Liste der Bits (0 oder 1).
/// * Rueckgabe: Liste der Lauf-Laengen in Reihenfolge ihres Auftretens.
#[pyfunction]
fn lauflaengen(bits: Vec<u8>) -> Vec<u64> {
    // Die Rust-Funktion der Grundlagen auf der Bitfolge ausfuehren.
    kernbibliothek::bitfolge::lauflaengen(&bits)
}

/// Das Lauf-Histogramm einer rohen Bytefolge: Haeufigkeit der Laeufe
/// je Laenge, wobei je Byte das hoechstwertige Bit zuerst genommen
/// wird und Laeufe Bytegrenzen uebergreifen koennen.
///
/// Diese Funktion ist der schnelle Einstieg fuer grosse Datenmengen
/// aus Python (uebergibt `bytes` direkt, ohne Umwandlung in Listen);
/// der Python-Interpreter wird waehrend der Berechnung freigegeben.
///
/// * `daten` - rohe Bytes (z. B. der Inhalt einer Sammeldatei).
/// * Rueckgabe: Liste der Lauf-Haeufigkeiten je Laenge.
#[pyfunction]
fn laufhistogramm_aus_bytes(py: Python<'_>, daten: &[u8]) -> Vec<u64> {
    // Die Berechnung ohne Python-Interpreter-Sperre ausfuehren (GIL-frei).
    py.allow_threads(|| {
        // Einen frischen Lauf-Haeufigkeitszaehler anlegen.
        let mut zaehler = kernbibliothek::einschritttest::Laufhaeufigkeitszaehler::neu();
        // Die rohen Bytes in den Zaehler einspeisen.
        zaehler.verarbeite_bytes(daten);
        // Die Folge abschliessen, damit der letzte offene Lauf verbucht wird.
        zaehler.schliesse_ab();
        // Das Histogramm als eigene Liste zurueckgeben.
        zaehler.histogramm().to_vec()
    })
}

/// Erzeugt die kumulierte Verteilung aus einem Lauf-Histogramm
/// (Dreiecksbeziehung des Papers, Abschnitt 6):
/// $l_p = \sum_{k \ge p} (k - p + 1) \cdot g_k$.
///
/// * `histogramm` - Liste der Lauf-Haeufigkeiten je Laenge.
/// * Rueckgabe: Liste der kumulierten Haeufigkeiten $l_p$.
#[pyfunction]
fn kumulierte_verteilung_aus_histogramm(histogramm: Vec<u64>) -> Vec<u64> {
    // Die Rust-Funktion auf dem uebergebenen Histogramm ausfuehren.
    kernbibliothek::einschritttest::kumulierte_verteilung_aus_histogramm(&histogramm)
}

/// Bestimmt die Grundverteilung aus der kumulierten Verteilung
/// durch die im Paper beschriebene Abgeltung (Abschnitt 6).
///
/// * `kumulierte_verteilung` - Liste der Haeufigkeiten $l_p$.
/// * Rueckgabe: Liste der Grundverteilung $g_p$ gleicher Laenge.
#[pyfunction]
fn grundverteilung_aus_kumulierter(kumulierte_verteilung: Vec<u64>) -> Vec<u64> {
    // Die Rust-Funktion der Abgeltung ausfuehren.
    kernbibliothek::klassen::grundverteilung_aus_kumulierter(&kumulierte_verteilung)
}

/// Mitgliedschaftstest fuer die Klasse $B_N$: Die Lauffolge liegt
/// genau dann in $B_N$, wenn der laengste Lauf die Laenge $N$ hat und
/// die kumulierte Verteilung exakt exponentiell faellt.
///
/// * `lauflaengen` - Liste der Lauf-Laengen der zu pruefenden Folge.
/// * `klassenindex` - der Klassenindex $N$.
/// * Rueckgabe: `True` genau dann, wenn die Folge in $B_N$ liegt.
#[pyfunction]
fn ist_mitglied(lauflaengen: Vec<u64>, klassenindex: u64) -> bool {
    // Den Rust-Mitgliedschaftstest auf der Lauffolge ausfuehren.
    kernbibliothek::klassen::ist_mitglied(&lauflaengen, klassenindex)
}

/// Die Summenidentitaet des Papers (Lemma): Wert der Summe
/// $T_m = \sum_{k=1}^{m} k \cdot 2^{m-k}$.
///
/// * `m` - obere Grenze der Summe.
/// * Rueckgabe: der Summenwert $T_m$.
#[pyfunction]
fn summenidentitaet_t(m: u64) -> u64 {
    // Die Rust-Funktion der Summenidentitaet ausfuehren.
    kernbibliothek::klassen::summenidentitaet_t(m)
}

/// Die rechte Seite der Abgeltungsidentitaet des Papers (Lemma).
///
/// * `n` - der Parameter $N$ der Identitaet.
/// * Rueckgabe: der Wert der rechten Seite (nach dem Lemma $2^N$).
#[pyfunction]
fn abgeltungsidentitaet_rechte_seite(n: u64) -> u64 {
    // Die Rust-Funktion der Abgeltungsidentitaet ausfuehren.
    kernbibliothek::klassen::abgeltungsidentitaet_rechte_seite(n)
}

/// Der iterative Generator (Algorithmus 3 des Papers): Lauffolge
/// eines Klassenvertreters von $B_N$ aus der geschlossenen Form.
///
/// * `klassenindex` - der Klassenindex $N$ mit $N \ge 1$.
/// * Rueckgabe: Liste der Lauf-Laengen des Vertreters.
#[pyfunction]
fn iterativer_generator(klassenindex: u64) -> Vec<u64> {
    // Den Rust-Generator fuer die Lauffolge des Vertreters ausfuehren.
    kernbibliothek::generatoren::iterativer_generator(klassenindex)
}

/// Der rekursive Uebergang (Algorithmus 4 des Papers): Ueberfuehrt
/// die Lauffolge eines Elements aus $B_N$ in ein Element aus $B_{N+1}$.
///
/// * `lauflaengen` - Liste der Lauf-Laengen des Ausgangselements.
/// * Rueckgabe: Liste der Lauf-Laengen des Zielelements.
#[pyfunction]
fn rekursiver_uebergang(lauflaengen: Vec<u64>) -> Vec<u64> {
    // Den Rust-UEbergang auf der Lauffolge ausfuehren.
    kernbibliothek::generatoren::rekursiver_uebergang(&lauflaengen)
}

/// Die zusammengesetzte Haeufigkeitsverteilung $_B H_n$ fuer alle
/// Lauf-Laengen $n = 1, \dots, N$ (Paper-Abschnitt 8).
///
/// * `zahl` - die natuerliche Zahl $B$ mit $B \ge 1$.
/// * Rueckgabe: Liste $(H_1, \dots, H_N)$.
#[pyfunction]
fn zusammengesetzte_haeufigkeit(zahl: u64) -> Vec<u64> {
    // Die Rust-Funktion der Komposition ausfuehren.
    kernbibliothek::komposition::zusammengesetzte_haeufigkeit(zahl)
}

/// Die kumulierte zusammengesetzte Haeufigkeit $C_n$ nach der
/// Identitaet des Papers: $C_n = (B_n + b_n)/2$.
///
/// * `zahl` - die natuerliche Zahl $B$.
/// * `stelle` - die Lauf-Laenge $n$ mit $n \ge 1$.
/// * Rueckgabe: der Wert $C_n$; 0 fuer $n > N$.
#[pyfunction]
fn kumulierte_zusammengesetzte_haeufigkeit(zahl: u64, stelle: u64) -> u64 {
    // Die Rust-Funktion der kumulierten Komposition ausfuehren.
    kernbibliothek::komposition::kumulierte_zusammengesetzte_haeufigkeit(zahl, stelle)
}

/// Die Laenge der zusammengesetzten Bitfolge:
/// $\sum_{n=1}^{N} n \cdot H_n$; nach dem Paper gleich $B$.
///
/// * `zahl` - die natuerliche Zahl $B$.
/// * Rueckgabe: die gewichtete Summe der Haeufigkeiten.
#[pyfunction]
fn zusammengesetzte_laenge(zahl: u64) -> u64 {
    // Die Rust-Funktion der gewichteten Gesamtlaenge ausfuehren.
    kernbibliothek::komposition::zusammengesetzte_laenge(zahl)
}

/// Das Python-Modul `bitfolgen`: Sammelt alle Funktionen des Bindings.
#[pymodule]
fn bitfolgen(modul: &Bound<'_, PyModule>) -> PyResult<()> {
    // Die Einschritttest-Variante fuer Bitfolgen bereitstellen.
    modul.add_function(wrap_pyfunction!(einschritttest_bitfolge, modul)?)?;
    // Die Einschritttest-Variante fuer Lauffolgen bereitstellen.
    modul.add_function(wrap_pyfunction!(einschritttest_laeufe, modul)?)?;
    // Die Lauf-Laengen-Bestimmung bereitstellen.
    modul.add_function(wrap_pyfunction!(lauflaengen, modul)?)?;
    // Den schnellen Byte-Histogramm-Einstieg bereitstellen.
    modul.add_function(wrap_pyfunction!(laufhistogramm_aus_bytes, modul)?)?;
    // Die Kumulation aus dem Histogramm bereitstellen.
    modul.add_function(wrap_pyfunction!(kumulierte_verteilung_aus_histogramm, modul)?)?;
    // Die Abgeltung zur Grundverteilung bereitstellen.
    modul.add_function(wrap_pyfunction!(grundverteilung_aus_kumulierter, modul)?)?;
    // Den Mitgliedschaftstest der Klassen bereitstellen.
    modul.add_function(wrap_pyfunction!(ist_mitglied, modul)?)?;
    // Die Summenidentitaet bereitstellen.
    modul.add_function(wrap_pyfunction!(summenidentitaet_t, modul)?)?;
    // Die Abgeltungsidentitaet bereitstellen.
    modul.add_function(wrap_pyfunction!(abgeltungsidentitaet_rechte_seite, modul)?)?;
    // Den iterativen Generator bereitstellen.
    modul.add_function(wrap_pyfunction!(iterativer_generator, modul)?)?;
    // Den rekursiven Uebergang bereitstellen.
    modul.add_function(wrap_pyfunction!(rekursiver_uebergang, modul)?)?;
    // Die zusammengesetzte Verteilung bereitstellen.
    modul.add_function(wrap_pyfunction!(zusammengesetzte_haeufigkeit, modul)?)?;
    // Die kumulierte zusammengesetzte Haeufigkeit bereitstellen.
    modul.add_function(wrap_pyfunction!(kumulierte_zusammengesetzte_haeufigkeit, modul)?)?;
    // Die gewichtete Gesamtlaenge bereitstellen.
    modul.add_function(wrap_pyfunction!(zusammengesetzte_laenge, modul)?)?;
    // Das Modul erfolgreich abschliessen.
    Ok(())
}
