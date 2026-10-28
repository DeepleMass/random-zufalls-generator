//! Klassen $B_N$: Grundverteilung, Mitgliedschaft und Identitaeten
//! (Paper-Abschnitt 6).
//!
//! Die Grundverteilung zaehlt die Laeufe je Laenge; sie ist durch die
//! kumulierte Verteilung eindeutig bestimmt und wird aus ihr durch
//! die rekursive Abgeltung von oben gewonnen (Gleichung der
//! rekursiven Auflösung im Paper).

use crate::einschritttest::einschritttest_laeufe;

/// Die Grundverteilung $G = (g_p)$: Anzahl der Laeufe je Laenge;
/// Index $p-1$ entspricht der Lauf-Laenge $p$.
pub type Grundverteilung = Vec<u64>;

/// Bestimmt die Grundverteilung aus der kumulierten Verteilung durch
/// die im Paper beschriebene Abgeltung: Die rekursive Auflösung von
/// oben bucht jede Stelle $p$, nachdem die Beitraege aller laengeren
/// Laeufe abgezogen sind.
///
/// * `kumulierte_verteilung` - die kumulierte Verteilung $L = (l_p)$.
/// * Rueckgabe: die Grundverteilung $G = (g_p)$ gleicher Laenge.
///
/// # Beispiel
///
/// ```
/// use bitfolgen::klassen::grundverteilung_aus_kumulierter;
/// assert_eq!(grundverteilung_aus_kumulierter(&[16, 8, 4, 2, 1]),
///            vec![4, 2, 1, 0, 1]);
/// ```
pub fn grundverteilung_aus_kumulierter(kumulierte_verteilung: &[u64]) -> Grundverteilung {
    // Die laengste vorkommende Lauf-Laenge entspricht der Feldlaenge.
    let laengste_lauflaenge = kumulierte_verteilung.len() as u64;
    // Die Grundverteilung gleicher Laenge mit Nullen anlegen.
    let mut grundverteilung = vec![0u64; kumulierte_verteilung.len()];
    // Die Stellen von oben (laengste Laenge) nach unten abarbeiten.
    for stelle in (1..=laengste_lauflaenge).rev() {
        // Der noch offene Haeufigkeitsanteil beginnt mit dem Wert $l_p$.
        let mut offener_anteil = kumulierte_verteilung[(stelle - 1) as usize];
        // Die Beitraege aller bereits bekannten laengeren Laeufe abziehen.
        for laengere_stelle in (stelle + 1)..=laengste_lauflaenge {
            // Der Abzug ist $(k - p + 1) \cdot g_k$ des laengeren Laufs.
            let abzug = ((laengere_stelle - stelle + 1) as u64)
                * grundverteilung[(laengere_stelle - 1) as usize];
            // Den Abzug vom offenen Anteil buchen.
            offener_anteil -= abzug;
        }
        // Der verbleibende Anteil ist die Haeufigkeit $g_p$ der Laenge $p$.
        grundverteilung[(stelle - 1) as usize] = offener_anteil;
    }
    // Die fertige Grundverteilung zurueckgeben.
    grundverteilung
}

/// Bestimmt den laengsten Lauf einer Lauffolge.
///
/// * `lauflaengen` - die zu pruefende Lauffolge.
/// * Rueckgabe: das Maximum der Lauf-Laengen, 0 fuer die leere Folge.
///
/// # Beispiel
///
/// ```
/// use bitfolgen::klassen::laengster_lauf;
/// assert_eq!(laengster_lauf(&[1, 3, 2]), 3);
/// ```
pub fn laengster_lauf(lauflaengen: &[u64]) -> u64 {
    // Das Maximum ueber alle Lauf-Laengen bestimmen; leere Folge gibt 0.
    lauflaengen.iter().copied().max().unwrap_or(0)
}

/// Der Mitgliedschaftstest fuer die Klasse $B_N$: Eine Bitfolge mit
/// der gegebenen Lauffolge liegt genau dann in $B_N$, wenn der
/// laengste Lauf die Laenge $N$ hat und die kumulierte Verteilung
/// exakt exponentiell faellt: $l_n = 2^{N-n}$ fuer alle $1 \le n \le N$.
///
/// * `lauflaengen` - die Lauffolge der zu pruefenden Bitfolge.
/// * `klassenindex` - der Klassenindex $N$ der vermuteten Klasse.
/// * Rueckgabe: `true` genau dann, wenn die Folge in $B_N$ liegt.
///
/// # Beispiel
///
/// ```
/// use bitfolgen::klassen::ist_mitglied;
/// assert!(ist_mitglied(&[5, 3, 2, 2, 1, 1, 1, 1], 5));
/// assert!(!ist_mitglied(&[2, 2], 2));
/// ```
pub fn ist_mitglied(lauflaengen: &[u64], klassenindex: u64) -> bool {
    // Frueher Abbruch: der laengste Lauf muss exakt die Klassenlaenge sein.
    if laengster_lauf(lauflaengen) != klassenindex {
        return false;
    }
    // Frueher Abbruch: fuer N > 63 passt 2^(N-n) nicht mehr in u64.
    if klassenindex > 63 {
        return false;
    }
    // Die kumulierte Verteilung der Lauffolge berechnen.
    let kumulierte_verteilung = einschritttest_laeufe(lauflaengen);
    // Frueher Abbruch: der laengste Index P muss ebenfalls N sein.
    if kumulierte_verteilung.len() as u64 != klassenindex {
        return false;
    }
    // Jede Stelle $n$ gegen den geforderten Wert $2^{N-n}$ pruefen.
    for stelle in 1..=klassenindex {
        // Der geforderte exponentielle Wert an der Stelle $n$.
        let geforderter_wert = 1u64 << (klassenindex - stelle);
        // Frueher Abbruch: jede Abweichung verhindert die Mitgliedschaft.
        if kumulierte_verteilung[(stelle - 1) as usize] != geforderter_wert {
            return false;
        }
    }
    // Alle Bedingungen sind erfuellt: die Folge liegt in $B_N$.
    true
}

/// Die Summenidentitaet des Papers (Lemma): der Wert der Summe
/// $T_m = \sum_{k=1}^{m} k \cdot 2^{m-k}$.
///
/// * `m` - obere Grenze der Summe.
/// * Rueckgabe: der Summenwert $T_m$ (nach dem Lemma $2^{m+1} - m - 2$).
///
/// # Beispiel
///
/// ```
/// use bitfolgen::klassen::summenidentitaet_t;
/// assert_eq!(summenidentitaet_t(3), 11); // 1*4 + 2*2 + 3*1
/// ```
pub fn summenidentitaet_t(m: u64) -> u64 {
    // Die Summe beginnt bei Null.
    let mut summe = 0u64;
    // Jeden Summanden $k \cdot 2^{m-k}$ einzeln aufaddieren.
    for k in 1..=m {
        // Der Summand dieser Ordnung.
        let summand = k * (1u64 << (m - k));
        // Den Summanden auf die Summe aufschlagen.
        summe += summand;
    }
    // Den Summenwert zurueckgeben.
    summe
}

/// Die rechte Seite der Abgeltungsidentitaet des Papers (Lemma):
/// $(N+1) + 1_{\{N>1\}} \cdot \sum_{n=0}^{N-2} (N-1-n) \cdot 2^n$.
///
/// * `n` - der Parameter $N$ der Identitaet.
/// * Rueckgabe: der Wert der rechten Seite (nach dem Lemma $2^N$).
///
/// # Beispiel
///
/// ```
/// use bitfolgen::klassen::abgeltungsidentitaet_rechte_seite;
/// assert_eq!(abgeltungsidentitaet_rechte_seite(4), 16);
/// ```
pub fn abgeltungsidentitaet_rechte_seite(n: u64) -> u64 {
    // Frueher Abbruch: fuer N < 2 verschwindet die Summe (Indikator Null).
    if n < 2 {
        // Es bleibt nur der Summand $(N+1)$.
        return n + 1;
    }
    // Die rechte Seite beginnt mit dem Laufbeitrag $(N+1)$.
    let mut rechte_seite = n + 1;
    // Die Summenglieder fuer die Exponenten 0 bis N-2 aufaddieren.
    for exponent in 0..=(n - 2) {
        // Das Summenglied $(N-1-n) \cdot 2^n$ (Paper-Index $n$ ist der Exponent).
        let glied = (n - 1 - exponent) * (1u64 << exponent);
        // Das Glied auf die rechte Seite aufschlagen.
        rechte_seite += glied;
    }
    // Den Wert der rechten Seite zurueckgeben.
    rechte_seite
}
