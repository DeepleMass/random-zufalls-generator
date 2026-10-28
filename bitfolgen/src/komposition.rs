//! Zusammengesetzte Haeufigkeiten aus der Binaerdarstellung natuerlicher
//! Zahlen (Paper-Abschnitt 8).
//!
//! Jede natuerliche Zahl $B$ mit Binaerdarstellung $b_N \dots b_1$
//! entspricht einer eindeutigen Zusammensetzung paarweise verschiedener
//! Klassen $B_n$ mit $b_n = 1$. Die zusammengesetzte Haeufigkeit der
//! Lauf-Laenge $n$ ist $H_n = b_n + \sum_{i \ge n+2} b_i \cdot 2^{i-n-2}$.

/// Die zusammengesetzte Haeufigkeitsverteilung $_B H_n$ fuer die
/// Lauf-Laengen $n = 1, \dots, N$; Index $n-1$ entspricht der Laenge $n$.
pub type ZusammengesetzteVerteilung = Vec<u64>;

/// Die Anzahl der Binaerstellen von `zahl` (0 fuer die Zahl 0);
/// im Paper der Index $N$ der fuehrenden Stelle $b_N = 1$.
///
/// * `zahl` - die zu zerlegende natuerliche Zahl $B$.
/// * Rueckgabe: die Anzahl der Binaerstellen $N$.
fn binaerstellen(zahl: u64) -> u64 {
    // Die fuehrenden Nullbits bestimmen die Stellenzahl im 64-Bit-Wort.
    (64 - zahl.leading_zeros()) as u64
}

/// Das Bit $b_n$ der Binaerdarstellung von `zahl` an der Stelle $n$
/// (Stelle 1 ist das niederwertigste Bit).
///
/// * `zahl` - die natuerliche Zahl $B$.
/// * `stelle` - die abzufragende Stelle $n$ mit $n \ge 1$.
/// * Rueckgabe: das Bit $b_n$, also 0 oder 1.
fn bit_an_stelle(zahl: u64, stelle: u64) -> u64 {
    // Durch Schieben an die niederwertigste Position und Maskieren.
    (zahl >> (stelle - 1)) & 1
}

/// Die zusammengesetzte Haeufigkeitsverteilung $_B H_n$ fuer alle
/// Lauf-Laengen $n = 1, \dots, N$ nach der Formel des Papers:
/// $H_n = b_n + \sum_{i=n+2}^{N} b_i \cdot 2^{i-n-2}$.
///
/// * `zahl` - die natuerliche Zahl $B$ mit $B \ge 1$.
/// * Rueckgabe: die Folge $(H_1, \dots, H_N)$.
///
/// # Beispiel
///
/// ```
/// use bitfolgen::komposition::zusammengesetzte_haeufigkeit;
/// assert_eq!(zusammengesetzte_haeufigkeit(341),
///            vec![86, 42, 22, 10, 6, 2, 2, 0, 1]);
/// ```
pub fn zusammengesetzte_haeufigkeit(zahl: u64) -> ZusammengesetzteVerteilung {
    // Frueher Abbruch: die Zahl 0 besitzt keine Binaerstelle.
    if zahl == 0 {
        return Vec::new();
    }
    // Die fuehrende Binaerstelle N bestimmt die Laenge der Verteilung.
    let stellenanzahl = binaerstellen(zahl);
    // Die Ergebnisverteilung in bekannter Laenge anlegen.
    let mut haeufigkeiten = Vec::with_capacity(stellenanzahl as usize);
    // Jede Lauf-Laenge n von 1 bis N einzeln bestimmen.
    for stelle in 1..=stellenanzahl {
        // Der eigene Klassenbeitrag $b_n$ an der Stelle $n$.
        let eigener_beitrag = bit_an_stelle(zahl, stelle);
        // Die Summe der Beitraege beginnt mit dem eigenen Beitrag.
        let mut summe = eigener_beitrag;
        // Alle laengeren Klassen $B_i$ mit $i \ge n+2$ tragen bei.
        for laengere_stelle in (stelle + 2)..=stellenanzahl {
            // Frueher Abbruch: unbesetzte Klassen tragen nichts bei.
            if bit_an_stelle(zahl, laengere_stelle) == 0 {
                continue;
            }
            // Der Beitrag der Klasse $B_i$ zur Laenge $n$.
            let beitrag = 1u64 << (laengere_stelle - stelle - 2);
            // Den Beitrag auf die Summe aufschlagen.
            summe += beitrag;
        }
        // Die Haeufigkeit dieser Laenge in die Verteilung uebernehmen.
        haeufigkeiten.push(summe);
    }
    // Die fertige zusammengesetzte Verteilung zurueckgeben.
    haeufigkeiten
}

/// Die kumulierte zusammengesetzte Haeufigkeit $C_n$: Summe der
/// Haeufigkeiten der Laengen mindestens $n$; nach der Identitaet des
/// Papers gilt $C_n = (B_n + b_n)/2$ mit der Zahl $B_n$, deren
/// Binaerdarstellung die Bits $b_N \dots b_n$ sind.
///
/// * `zahl` - die natuerliche Zahl $B$.
/// * `stelle` - die Lauf-Laenge $n$ mit $n \ge 1$.
/// * Rueckgabe: der Wert $C_n$; 0 fuer $n > N$.
///
/// # Beispiel
///
/// ```
/// use bitfolgen::komposition::kumulierte_zusammengesetzte_haeufigkeit;
/// assert_eq!(kumulierte_zusammengesetzte_haeufigkeit(341, 1), 171);
/// ```
pub fn kumulierte_zusammengesetzte_haeufigkeit(zahl: u64, stelle: u64) -> u64 {
    // Die fuehrende Binaerstelle N der Zahl.
    let stellenanzahl = binaerstellen(zahl);
    // Frueher Abbruch: jenseits der Binaerdarstellung ist nichts besetzt.
    if stelle < 1 || stelle > stellenanzahl {
        return 0;
    }
    // Die Zahl $B_n$ ist die Binaerdarstellung $b_N \dots b_n$ als Wert.
    let restzahl = zahl >> (stelle - 1);
    // Das Bit $b_n$ ist das niederwertigste Bit der Restzahl.
    let bit_an_der_stelle = restzahl & 1;
    // Die Identitaet liefert $(B_n + b_n)/2$.
    (restzahl + bit_an_der_stelle) / 2
}

/// Die Laenge der zusammengesetzten Bitfolge: $\sum_{n=1}^{N} n \cdot H_n$;
/// nach dem Paper muss sie gleich $B$ sein.
///
/// * `zahl` - die natuerliche Zahl $B$.
/// * Rueckgabe: die gewichtete Summe der zusammengesetzten Haeufigkeiten.
///
/// # Beispiel
///
/// ```
/// use bitfolgen::komposition::zusammengesetzte_laenge;
/// assert_eq!(zusammengesetzte_laenge(341), 341);
/// ```
pub fn zusammengesetzte_laenge(zahl: u64) -> u64 {
    // Die zusammengesetzte Verteilung fuer alle Laengen bestimmen.
    let verteilung = zusammengesetzte_haeufigkeit(zahl);
    // Die gewichtete Summe beginnt bei Null.
    let mut gesamtlaenge = 0u64;
    // Jede Laenge n mit ihrer Haeufigkeit gewichtet aufsummieren.
    for (laufstelle, &haeufigkeit) in verteilung.iter().enumerate() {
        // Die Lauf-Laenge ist die Tabellenstelle plus 1.
        let stelle = laufstelle as u64 + 1;
        // Der gewichtete Beitrag $n \cdot H_n$ dieser Laenge.
        let beitrag = stelle * haeufigkeit;
        // Den Beitrag auf die Gesamtlaenge aufschlagen.
        gesamtlaenge += beitrag;
    }
    // Die gewichtete Gesamtlaenge zurueckgeben.
    gesamtlaenge
}
