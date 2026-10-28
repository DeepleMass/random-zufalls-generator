//! Implementierung des Papers \enquote{The Evaluation of Finite Bit
//! Sequences}.
//!
//! Diese Kistenbibliothek (crate) setzt die Inhalte des Papers in Rust
//! um. Die Module folgen der Gliederung des Papers:
//!
//! | Modul           | Paper-Abschnitt                                        |
//! |-----------------|--------------------------------------------------------|
//! | [`bitfolge`]    | 3: Grundlagen, Laeufe und Teillaeufe                   |
//! | [`einschritttest`] | 4: Der Einschritttest in beiden Varianten          |
//! | [`klassen`]     | 6: Kumulierte/Grundverteilung, Klassen `B_N`          |
//! | [`generatoren`] | 7: Iterativer und rekursiver Generator                 |
//! | [`komposition`] | 8: Zusammengesetzte Haeufigkeitsverteilungen           |
//!
//! Das Kommandozeilenwerkzeug `auswertung` (Abschnitt 5 des Papers)
//! wertet einen Datenbestand von Sammeldateien aus.
//!
//! Alle Bezeichner folgen dem Paper; Sonderzeichen werden als
//! Umlaut-Umschreibung geschrieben (ae, oe, ue).

/// Grundlagen: Bitfolgen, Laeufe, Lauffolgen (Paper-Abschnitt 3).
pub mod bitfolge;

/// Der Einschritttest in beiden Varianten samt Lookup-Tabelle
/// (Paper-Abschnitt 4).
pub mod einschritttest;

/// Kumulierte und Grundverteilung, Klassen `B_N`, Mitgliedschaft
/// (Paper-Abschnitt 6).
pub mod klassen;

/// Iterativer und rekursiver Generator fuer Klassenvertreter
/// (Paper-Abschnitt 7).
pub mod generatoren;

/// Zusammengesetzte Haeufigkeiten aus der Binaerdarstellung
/// natuerlicher Zahlen (Paper-Abschnitt 8).
pub mod komposition;
