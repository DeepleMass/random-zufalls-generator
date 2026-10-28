//! Generatoren fuer Klassenvertreter von $B_N$ (Paper-Abschnitt 7).
//!
//! Der iterative Generator erzeugt die Lauffolge eines Vertreters
//! direkt aus der geschlossenen Form des Grundverteilungs-Satzes; der
//! rekursive Uebergang beschreibt die Konstruktion von $B_{N-1}$ nach
//! $B_N$ (Lemma der Konstruktion): Jeder Lauf wird um ein Bit
//! erweitert und um $2^{N-3}$ Laeufe der Laenge 1 ergaenzt.

use crate::bitfolge::Bitfolge;
use crate::klassen::laengster_lauf;

/// Der iterative Generator (Algorithmus 3 des Papers): Erzeugt die
/// Lauffolge eines Klassenvertreters von $B_N$ aus der geschlossenen
/// Form der Grundverteilung - ein Lauf der Laenge $N$, dann fuer
/// $i = N-2$ bis 1 je $2^{N-i-2}$ Laeufe der Laenge $i$.
///
/// * `klassenindex` - der Klassenindex $N$ mit $N \ge 1$.
/// * Rueckgabe: die Lauffolge eines Vertreters von $B_N$.
pub fn iterativer_generator(klassenindex: u64) -> Vec<u64> {
    // Die Lauffolge beginnt mit dem einzigen laengsten Lauf der Laenge N.
    let mut lauflaengen: Vec<u64> = vec![klassenindex];
    // Frueher Abbruch: fuer N <= 2 gibt es keine weiteren Laeufe.
    if klassenindex < 3 {
        return lauflaengen;
    }
    // Die Ergaenzungs-Laengen von N-2 abwaerts bis 1 durchlaufen.
    for lauflaenge in (1..=(klassenindex - 2)).rev() {
        // Die Haeufigkeit dieser Laenge nach der geschlossenen Form.
        let haeufigkeit = 1u64 << (klassenindex - lauflaenge - 2);
        // Die Laenge entsprechend oft an die Lauffolge anhaengen.
        for _ in 0..haeufigkeit {
            lauflaengen.push(lauflaenge);
        }
    }
    // Die fertige Lauffolge des Klassenvertreters zurueckgeben.
    lauflaengen
}

/// Der rekursive Uebergang (Algorithmus 4 des Papers): Ueberfuehrt
/// die Lauffolge eines Elements aus $B_N$ in ein Element aus
/// $B_{N+1}$, indem jeder Lauf um ein Bit erweitert und um
/// $2^{N-2}$ Laeufe der Laenge 1 ergaenzt wird (Lemma der
/// Konstruktion mit Zielklasse $N+1$).
///
/// * `lauflaengen` - die Lauffolge eines Elements aus $B_N$.
/// * Rueckgabe: die Lauffolge eines Elements aus $B_{N+1}$.
pub fn rekursiver_uebergang(lauflaengen: &[u64]) -> Vec<u64> {
    // Der laengste Lauf N der Eingabe steuert die Ergaenzung.
    let klassenindex = laengster_lauf(lauflaengen);
    // Alle Lauf-Laengen um 1 erweitern (Schritt 1 des Lemmas).
    let mut neue_lauflaengen: Vec<u64> = lauflaengen.iter().map(|l| l + 1).collect();
    // Frueher Abbruch: fuer N < 2 ist keine Ergaenzung noetig.
    if klassenindex < 2 {
        return neue_lauflaengen;
    }
    // Die Anzahl der neuen Laeufe der Laenge 1 nach dem Lemma.
    let ergaenzung = 1u64 << (klassenindex - 2);
    // Die neuen Laeufe der Laenge 1 anhaengen (Schritt 2 des Lemmas).
    for _ in 0..ergaenzung {
        neue_lauflaengen.push(1);
    }
    // Die Lauffolge des Elements aus B_{N+1} zurueckgeben.
    neue_lauflaengen
}

/// Erzeugt die Bitfolge eines Klassenvertreters von $B_N$: die
/// Realisierung der Lauffolge des iterativen Generators.
///
/// * `klassenindex` - der Klassenindex $N$ mit $N \ge 1$.
/// * Rueckgabe: eine Bitfolge, die in $B_N$ liegt.
pub fn klassenvertreter_bitfolge(klassenindex: u64) -> Bitfolge {
    // Die Lauffolge des Vertreters aus dem iterativen Generator holen.
    let lauflaengen = iterativer_generator(klassenindex);
    // Die Lauffolge als Bitfolge realisieren (alternierend, Startbit 0).
    Bitfolge::aus_lauflaengen(&lauflaengen)
}
