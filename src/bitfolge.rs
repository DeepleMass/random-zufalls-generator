//! Grundlagen: endliche Bitfolgen, Laeufe und Lauffolgen.
//!
//! Dieses Modul setzt Paper-Abschnitt 3 um: die Menge der endlichen
//! Bitfolgen, die Definition des Laufs als konstante, in beiden
//! Richtungen maximale Teilfolge, die eindeutige Laeuflerlegung
//! (Lemma der Laeuflerlegung) und die Identifikation von Laeufen mit
//! ihrer Laenge.

/// Ein einzelnes Bit einer Bitfolge, entweder 0 oder 1.
pub type Bit = u8;

/// Eine endliche Bitfolge im Sinne der Definition des Papers
/// (Abschnitt 3): eine Folge $(b_m)_{1 \le m \le M}$ mit
/// $b_m \in \{0,1\}$.
///
/// Die Folge speichert jedes Bit als eigenes Byte (`0` oder `1`).
/// Diese Darstellung ist bewusst nicht bitverdichtet: Sie haelt alle
/// Indizes des Papiers direkt lesbar.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Bitfolge {
    /// Die Bits der Folge in natuerlicher Reihenfolge, jedes 0 oder 1.
    bits: Vec<Bit>,
}

/// Ein Lauf im Sinne der Definition des Papers (Abschnitt 3): eine
/// konstante, in beiden Richtungen maximale Teilfolge der Bitfolge,
/// hier vertreten durch den konstanten Bitwert und die Laenge.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Lauf {
    /// Der konstante Bitwert des Laufs, 0 oder 1.
    pub bitwert: Bit,
    /// Die Laenge des Laufs (Anzahl der Bits).
    pub laenge: u64,
}

impl Bitfolge {
    /// Erzeugt eine Bitfolge aus einer Textdarstellung wie
    /// `"0101001100011111"`; alle Zeichen muessen `0` oder `1` sein.
    ///
    /// * `darstellung` - Text, der nur aus den Zeichen 0 und 1 besteht.
    /// * Rueckgabe: die entsprechende Bitfolge.
    pub fn aus_text(darstellung: &str) -> Self {
        // Ergebnisvektor mit ausreichend Platz fuer alle Zeichen anlegen.
        let mut bits: Vec<Bit> = Vec::with_capacity(darstellung.len());
        // Jedes Zeichen der Textdarstellung einzeln betrachten.
        for zeichen in darstellung.bytes() {
            // Zeichen `0` ist Bit 0, jedes andere zulaessige Zeichen `1` ist Bit 1.
            let bit = (zeichen == b'1') as Bit;
            // Nur die beiden zulaessigen Zeichen akzeptieren, sonst Fehler.
            assert!(
                zeichen == b'0' || zeichen == b'1',
                "unzulaessiges Zeichen in der Textdarstellung"
            );
            // Das uebersetzte Bit an die Folge anhaengen.
            bits.push(bit);
        }
        // Die fertige Bitfolge zurueckgeben.
        Self { bits }
    }

    /// Erzeugt eine Bitfolge aus rohen Bytes, wobei je Byte das
    /// hoechstwertige Bit zuerst genommen wird (die Konvention der
    /// experimentellen Auswertung).
    ///
    /// * `bytes` - rohe Bytes der Sammeldatei.
    /// * Rueckgabe: die Bitfolge der Laenge `8 * bytes.len()`.
    pub fn aus_bytes(bytes: &[u8]) -> Self {
        // Ergebnisvektor mit exakt der bekannten Bitanzahl anlegen.
        let mut bits: Vec<Bit> = Vec::with_capacity(bytes.len() * 8);
        // Je Byte alle acht Bits, hoechstwertiges Bit zuerst, betrachten.
        for &bytewert in bytes.iter() {
            // Die Bitposition innerhalb des Bytes von links (0) nach rechts (7).
            for bitposition in 0..8 {
                // Das Bit an der Position durch Schieben und Maskieren gewinnen.
                let bit = (bytewert >> (7 - bitposition)) & 1;
                // Das gewonnene Bit an die Folge anhaengen.
                bits.push(bit);
            }
        }
        // Die fertige Bitfolge zurueckgeben.
        Self { bits }
    }

    /// Erzeugt eine Bitfolge als Realisierung einer Lauffolge
        /// (Lemma der Laeuflerlegung): Die Laeufe folgen aufeinander und
        /// tragen abwechselnde Bitwerte, beginnend mit dem Bit 0.
    ///
    /// * `lauflaengen` - Folge der Laufaengen.
        /// * Rueckgabe: die realisierte Bitfolge.
    pub fn aus_lauflaengen(lauflaengen: &[u64]) -> Self {
        // Gesamtlaenge als Summe aller Laufaengen im Voraus bestimmen.
        let gesamtlaenge: u64 = lauflaengen.iter().sum();
        // Ergebnisvektor mit der bekannten Gesamtlaenge anlegen.
        let mut bits: Vec<Bit> = Vec::with_capacity(gesamtlaenge as usize);
        // Das Bit des ersten Laufs ist per Vereinbarung 0.
        let mut bitwert: Bit = 0;
        // Jeden Lauf der Lauffolge einzeln realisieren.
        for &lauflaenge in lauflaengen.iter() {
            // Den Lauf als laenge viele Bits des aktuellen Bitwerts anhaengen.
            for _ in 0..lauflaenge {
                bits.push(bitwert);
            }
            // Aufeinanderfolgende Laeufe tragen komplementaere Bitwerte.
            bitwert = 1 - bitwert;
        }
        // Die fertige Bitfolge zurueckgeben.
        Self { bits }
    }

    /// Die Laenge $M$ der Bitfolge (Anzahl der Bits).
    pub fn laenge(&self) -> u64 {
        // Die Anzahl der gespeicherten Bits als natuerliche Zahl.
        self.bits.len() as u64
    }

    /// Die Bits der Folge als zusammenhaengende Ansicht.
    pub fn bits(&self) -> &[Bit] {
        // Direkter Verweis auf den internen Bitvektor.
        &self.bits
    }
}

/// Zerlegt eine Bitfolge eindeutig in Laeufe (Lemma der
/// Laeuflerlegung des Papers, Abschnitt 3).
///
/// * `bits` - die zu zerlegende Bitfolge.
/// * Rueckgabe: die Laeufe in Reihenfolge ihres Auftretens, jeweils
///   mit Bitwert und Laenge.
pub fn laufzerlegung(bits: &[Bit]) -> Vec<Lauf> {
    // Ergebnisliste der Laeufe anlegen.
    let mut laeufe: Vec<Lauf> = Vec::new();
    // Das Bit des vorigen Schritts; `None` bedeutet: Sequenz beginnt.
    let mut vorgaenger_bit: Option<Bit> = None;
    // Jedes Bit der Folge Schritt fuer Schritt betrachten.
    for &aktuelles_bit in bits.iter() {
        // Fallunterscheidung nach dem Verhaeltnis zum vorigen Bit.
        match vorgaenger_bit {
            // Am Sequenzanfang beginnt zwingend ein neuer Lauf.
            None => laeufe.push(Lauf { bitwert: aktuelles_bit, laenge: 1 }),
            // Gleiches Bit wie zuvor verlaengert den letzten Lauf um 1.
            Some(bitwert) if bitwert == aktuelles_bit => laeufe.last_mut().unwrap().laenge += 1,
            // Ein Bitwechsel beginnt zwingend einen neuen Lauf.
            Some(_) => laeufe.push(Lauf { bitwert: aktuelles_bit, laenge: 1 }),
        }
        // Das aktuelle Bit ist im naechsten Schritt der Vorgaenger.
        vorgaenger_bit = Some(aktuelles_bit);
    }
    // Die vollstaendige Laufzerlegung zurueckgeben.
    laeufe
}

/// Bestimmt die Lauffolge einer Bitfolge, also die Folge der
/// Laufaengen (Paper-Abschnitt 3: Da alle Haeufigkeitsbetrachtungen
/// nur die Laengen der Laeufe einbeziehen, identifizieren wir Laeufe
/// mit ihrer Laenge).
///
/// * `bits` - die zu untersuchende Bitfolge.
/// * Rueckgabe: die Laufaengen in Reihenfolge ihres Auftretens.
pub fn lauflaengen(bits: &[Bit]) -> Vec<u64> {
    // Die Laufzerlegung als Grundlage nehmen.
    let laeufe = laufzerlegung(bits);
    // Ergebnisliste in ausreichender Groesse anlegen.
    let mut laengen: Vec<u64> = Vec::with_capacity(laeufe.len());
    // Von jedem Lauf nur die Laenge uebernehmen.
    for lauf in laeufe.into_iter() {
        laengen.push(lauf.laenge);
    }
    // Die Lauffolge zurueckgeben.
    laengen
}
