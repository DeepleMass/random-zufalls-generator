//! Der Einschritttest (Paper-Abschnitt 4) in beiden Varianten samt
//! der byte-weisen Lookup-Tabelle fuer die Auswertung ganzer
//! Dateibestaende.
//!
//! Der Einschritttest ermittelt die kumulierte Verteilung $L = (l_p)$
//! der Haeufigkeiten der Laeufe und Teillaeufe einer Bitfolge. Das
//! Aequivalenzlemma des Papers garantiert, dass beide Varianten
//! dasselbe Ergebnis liefern.

use std::sync::LazyLock;

use crate::bitfolge::Bit;

/// Die kumulierte Verteilung als Folge von Haeufigkeiten; Index $p-1$
/// entspricht der (Teil-)Lauf-Laenge $p$.
pub type KumulierteVerteilung = Vec<u64>;

/// Eintrag der Lookup-Tabelle fuer ein Byte: Die Laufaengen im
/// Inneren des Bytes, das erste und das letzte Bit.
///
/// Die Tabelle ersetzt verschachtelte Fallunterscheidungen beim
/// byte-weisen Einlesen: Fuer jeden der 256 Bytewerte stehen die
/// Laufstruktur und die Randbits direkt ablesbar bereit.
struct ByteLaufTabelle {
    /// Je Bytewert bis zu acht Laufaengen im Inneren des Bytes.
    laeufe: [[u8; 8]; 256],
    /// Je Bytewert die Anzahl der Laeufe im Inneren des Bytes (1 bis 8).
    anzahl: [usize; 256],
    /// Je Bytewert das erste (linksstehende, hoechstwertige) Bit.
    erstes_bit: [Bit; 256],
    /// Je Bytewert das letzte (rechtsstehende, niederwertige) Bit.
    letztes_bit: [Bit; 256],
}

impl ByteLaufTabelle {
    /// Erzeugt die Lookup-Tabelle einmalig und vollstaendig.
    ///
    /// Fuer jeden Bytewert werden die acht Bits von links nach rechts
    /// durchlaufen und zu Laeufen zusammengefasst; die Laengen und die
    /// Randbits werden abgelegt.
    fn erzeuge() -> Self {
        // Das Grundgeruest der Tabelle mit Nullen vorbelegen.
        let mut tabelle = ByteLaufTabelle {
            laeufe: [[0; 8]; 256],
            anzahl: [0; 256],
            erstes_bit: [0; 256],
            letztes_bit: [0; 256],
        };
        // Jeden moeglichen Bytewert von 0 bis 255 einzeln aufschluesseln.
        for bytewert in 0u16..256 {
            // Das erste Bit steht links; es eroeffnet zwingend den ersten Lauf.
            let erstes_bit = ((bytewert >> 7) & 1) as Bit;
            // Das letzte Bit steht rechts und ist das Ende des letzten Laufs.
            let letztes_bit = (bytewert & 1) as Bit;
            // Der erste Lauf hat zunaechst die Laenge 1.
            let mut laeufe = [1u8; 8];
            // Bisher wurde genau ein Lauf im Byte begonnen.
            let mut anzahl = 1usize;
            // Das zuletzt gesehene Bit beginnend mit dem ersten Bit.
            let mut voriges_bit = erstes_bit;
            // Die Bitpositionen 1 bis 7 von links nach rechts ablaufen.
            for bitposition in 1..8u16 {
                // Das Bit an der aktuellen Position durch Schieben gewinnen.
                let bit = ((bytewert >> (7 - bitposition)) & 1) as Bit;
                // Gleiches Bit verlaengert den laufenden (letzten) Lauf.
                if bit == voriges_bit {
                    laeufe[anzahl - 1] += 1;
                } else {
                    // Bitwechsel beginnt einen neuen Lauf mit Laenge 1.
                    laeufe[anzahl] = 1;
                    // Ein Lauf mehr ist zu verwalten.
                    anzahl += 1;
                    // Das neue Bit ist Referenz fuer den folgenden Vergleich.
                    voriges_bit = bit;
                }
            }
            // Die Indexposition des Bytewerts in den Feldern bestimmen.
            let stelle = bytewert as usize;
            // Die Laufstruktur dieses Bytewerts ablegen.
            tabelle.laeufe[stelle] = laeufe;
            // Die Anzahl der Laeufe dieses Bytewerts ablegen.
            tabelle.anzahl[stelle] = anzahl;
            // Das erste Bit dieses Bytewerts ablegen.
            tabelle.erstes_bit[stelle] = erstes_bit;
            // Das letzte Bit dieses Bytewerts ablegen.
            tabelle.letztes_bit[stelle] = letztes_bit;
        }
        // Die fertige Tabelle zurueckgeben.
        tabelle
    }
}

/// Die Lookup-Tabelle wird erst beim ersten Zugriff einmalig erzeugt
/// und steht danach allen Aufrufen gemeinsam zur Verfuegung.
static BYTE_LAUFTABELLE: LazyLock<ByteLaufTabelle> = LazyLock::new(ByteLaufTabelle::erzeuge);

/// Der Einschritttest fuer Bitfolgen (Algorithmus 2 des Papers):
/// Zaehlt bei jedem Bit der Folge alle Stellen 1 bis z, wobei z die
/// Laenge des bis zu diesem Bit begonnenen Laufs ist.
///
/// * `bits` - die auszuwertende Bitfolge.
/// * Rueckgabe: die kumulierte Verteilung $L = (l_p)$; Index $p-1$
///   entspricht der (Teil-)Lauf-Laenge $p$.
pub fn einschritttest_bitfolge(bits: &[Bit]) -> KumulierteVerteilung {
    // Die kumulierte Verteilung wachsen lassen, sobals Laengen auftreten.
    let mut kumulierte_verteilung: KumulierteVerteilung = Vec::new();
    // Der Zaehlerstand z: Laenge des bisher begonnenen Laufs.
    let mut zaehlerstand: usize = 0;
    // Das Bit des vorigen Schritts; `None` bedeutet: Sequenz beginnt.
    let mut vorgaenger_bit: Option<Bit> = None;
    // Jedes Bit der Folge Schritt fuer Schritt verarbeiten.
    for &aktuelles_bit in bits.iter() {
        // Ein Bitwechsel (oder der Anfang) setzt den Zaehler auf 1 zurueck.
        if vorgaenger_bit != Some(aktuelles_bit) {
            zaehlerstand = 1;
            // Das aktuelle Bit ist im naechsten Schritt der Vorgaenger.
            vorgaenger_bit = Some(aktuelles_bit);
        } else {
            // Ohne Wechsel verlaengert der Zaehler den begonnenen Lauf.
            zaehlerstand += 1;
        }
        // Die Verteilung auf die aktuelle Zaehlerlaenge verlaengern.
        while kumulierte_verteilung.len() < zaehlerstand {
            kumulierte_verteilung.push(0);
        }
        // Alle Stellen 1 bis z um je 1 erhoehen (innere Schleife des Papers).
        for stelle in 0..zaehlerstand {
            kumulierte_verteilung[stelle] += 1;
        }
    }
    // Die fertige kumulierte Verteilung zurueckgeben.
    kumulierte_verteilung
}

/// Der Einschritttest fuer Folgen von Laeufen (Algorithmus 1 des
/// Papers): Jeder Lauf der Laenge $l$ erhoeht die Stelle $i$
/// ($1 \le i \le l$) um $l - i + 1$.
///
/// * `lauflaengen` - die auszuwertende Lauffolge.
/// * Rueckgabe: die kumulierte Verteilung $L = (l_p)$.
pub fn einschritttest_laeufe(lauflaengen: &[u64]) -> KumulierteVerteilung {
    // Die kumulierte Verteilung wachsen lassen, sobald Laengen auftreten.
    let mut kumulierte_verteilung: KumulierteVerteilung = Vec::new();
    // Jeden Lauf der Lauffolge einzeln abarbeiten.
    for &lauflaenge in lauflaengen.iter() {
        // Die Verteilung auf die aktuelle Lauflaenge verlaengern.
        while kumulierte_verteilung.len() < lauflaenge as usize {
            kumulierte_verteilung.push(0);
        }
        // Jede Stelle $i$ von 1 bis $l$ um $l - i + 1$ erhoehen.
        for stelle in 1..=lauflaenge {
            // Der Teillauf-Beitrag dieses Laufs an der Stelle.
            let beitrag = lauflaenge - stelle + 1;
            // Beitrag an der Stelle (0-basiert) aufsummieren.
            kumulierte_verteilung[(stelle - 1) as usize] += beitrag;
        }
    }
    // Die fertige kumulierte Verteilung zurueckgeben.
    kumulierte_verteilung
}

/// Zaehler fuer Lauf-Haeufigkeiten ueber eine lange, byte-weise
/// eingelesene Bitfolge (die Arbeitsweise der experimentellen
/// Auswertung in Paper-Abschnitt 5).
///
/// Der Zaehler haelt einen moeglicherweise ueber Datei- und
/// Blockgrenzen fortgesetzten offenen Lauf offen und bucht ihn erst
/// am Ende (oder beim naechsten Bitwechsel) ab. Damit wirkt die
/// gesamte Datensammlung wie EINE Bitfolge.
#[derive(Debug, Default)]
pub struct Laufhaeufigkeitszaehler {
    /// Histogramm der abgeschlossenen Laeufe; Index $k-1$ ist die Lauflaenge $k$.
    histogramm: Vec<u64>,
    /// Die Laenge des derzeit offenen, noch nicht abgerechneten Laufs.
    offener_lauf: u64,
    /// Das letzte verarbeitete Bit; `None` bedeutet: noch keins verarbeitet.
    letztes_bit: Option<Bit>,
    /// Die Anzahl der insgesamt verarbeiteten Bytes.
    byteanzahl: u64,
}

impl Laufhaeufigkeitszaehler {
    /// Erzeugt einen frischen Zaehler mit leerem Histogramm.
    pub fn neu() -> Self {
        // Der Default-Konstruktor liefert exakt den gewuenschten Anfangszustand.
        Self::default()
    }

    /// Bucht einen abgeschlossenen Lauf der gegebenen Laenge ins
    /// Histogramm ein.
    ///
    /// * `lauflaenge` - die Laenge des abzuschliessenden Laufs.
    fn vermerke_geschlossenen_lauf(&mut self, lauflaenge: u64) {
        // Das Histogramm auf die noetige Laenge verlaengern.
        while self.histogramm.len() < lauflaenge as usize {
            self.histogramm.push(0);
        }
        // Die Haeufigkeit dieser Lauflaenge um 1 erhoehen.
        self.histogramm[(lauflaenge - 1) as usize] += 1;
    }

    /// Bucht mehrere abgeschlossene Laeufe auf einmal ein.
    ///
    /// * `lauflaengen` - die Laengen der abzuschliessenden Laeufe.
    fn vermerke_geschlossene_laeufe(&mut self, lauflaengen: &[u8]) {
        // Jede Laenge einzeln verbuchen.
        for &lauflaenge in lauflaengen.iter() {
            self.vermerke_geschlossenen_lauf(lauflaenge as u64);
        }
    }

    /// Verarbeitet einen Block roher Bytes in die Laufstatistik;
    /// die Laeufe setzen sich ueber Block- und Dateigrenzen fort.
    ///
    /// * `bytes` - die rohen Bytes des Blocks (je Byte MSB zuerst).
    pub fn verarbeite_bytes(&mut self, bytes: &[u8]) {
        // Die Anzahl der verarbeiteten Bytes mitschreiben.
        self.byteanzahl += bytes.len() as u64;
        // Jedes Byte des Blocks einzeln abarbeiten.
        for &bytewert in bytes.iter() {
            // Die Tabellenstelle dieses Bytewerts ein fuer allemal festlegen.
            let stelle = bytewert as usize;
            // Verweis auf die Lauflaengen dieses Bytewerts aus der Tabelle.
            let laeufe = &BYTE_LAUFTABELLE.laeufe[stelle];
            // Die Anzahl der Laeufe dieses Bytewerts aus der Tabelle.
            let anzahl = BYTE_LAUFTABELLE.anzahl[stelle];
            // Das erste Bit dieses Bytewerts aus der Tabelle.
            let erstes_bit = BYTE_LAUFTABELLE.erstes_bit[stelle];
            // Das letzte Bit dieses Bytewerts aus der Tabelle.
            let letztes_bit = BYTE_LAUFTABELLE.letztes_bit[stelle];
            // Fallunterscheidung nach dem Zustand am Byteanfang (flach gehalten).
            match self.letztes_bit {
                // Noch kein Bit verarbeitet: die Folge beginnt mitten im Byte.
                None => {
                    // Alle inneren Laeufe des Bytes sind abgeschlossen.
                    self.vermerke_geschlossene_laeufe(&laeufe[..anzahl - 1]);
                    // Nur der letzte Lauf des Bytes bleibt offen.
                    self.offener_lauf = laeufe[anzahl - 1] as u64;
                }
                // Das Byte beginnt mit demselben Bit wie die Folge endete.
                Some(voriges_bit) if voriges_bit == erstes_bit => {
                    // Frueher Abbruch: ein konstantes Byte verlaengert nur.
                    if anzahl == 1 {
                        // Der offene Lauf waechst um die konstante Bytelaenge 8.
                        self.offener_lauf += laeufe[0] as u64;
                        // Das letzte Bit fuer das naechste Byte merken.
                        self.letztes_bit = Some(letztes_bit);
                        // Naechstes Byte bearbeiten, ohne weiteren Zweig.
                        continue;
                    }
                    // Der offene Lauf endet im Byte: Laengen zusammenfuehren.
                    let geschlossene_laenge = self.offener_lauf + laeufe[0] as u64;
                    // Den zusammengefuehrten Lauf abschliessen und verbuchen.
                    self.vermerke_geschlossenen_lauf(geschlossene_laenge);
                    // Alle mittleren Laeufe des Bytes sind abgeschlossen.
                    self.vermerke_geschlossene_laeufe(&laeufe[1..anzahl - 1]);
                    // Nur der letzte Lauf des Bytes bleibt offen.
                    self.offener_lauf = laeufe[anzahl - 1] as u64;
                }
                // Der Wechsel passiert genau am Byteanfang: alles klar getrennt.
                Some(_) => {
                    // Der bis hierher offene Lauf ist mit dem Byteanfang beendet.
                    self.vermerke_geschlossenen_lauf(self.offener_lauf);
                    // Alle inneren Laeufe des Bytes sind abgeschlossen.
                    self.vermerke_geschlossene_laeufe(&laeufe[..anzahl - 1]);
                    // Nur der letzte Lauf des Bytes bleibt offen.
                    self.offener_lauf = laeufe[anzahl - 1] as u64;
                }
            }
            // Das letzte Bit dieses Bytes ist Vorgaenger des naechsten Bytes.
            self.letztes_bit = Some(letztes_bit);
        }
    }

    /// Schliesst die Folge ab: Der noch offene Lauf wird verbucht;
    /// danach sind keine weiteren Verarbeitungen vorgesehen.
    pub fn schliesse_ab(&mut self) {
        // Frueher Abbruch: ohne offenen Lauf ist nichts zu tun.
        if self.offener_lauf == 0 {
            return;
        }
        // Den letzten, bis zum Ende offenen Lauf verbuchen.
        self.vermerke_geschlossenen_lauf(self.offener_lauf);
        // Kein Lauf bleibt mehr offen.
        self.offener_lauf = 0;
        // Kein Vorgaengerbit bleibt mehr gesetzt.
        self.letztes_bit = None;
    }

    /// Das bisher aufgebaute Histogramm der Laufaengen als Ansicht.
    pub fn histogramm(&self) -> &[u64] {
        // Verweis auf das interne Histogramm.
        &self.histogramm
    }

    /// Die Anzahl der bisher verarbeiteten Bytes.
    pub fn byteanzahl(&self) -> u64 {
        // Die mitgezaehlte Byteanzahl zurueckgeben.
        self.byteanzahl
    }
}

/// Erzeugt die kumulierte Verteilung aus einem Lauf-Histogramm
/// (Dreiecksbeziehung des Papers, Abschnitt 6):
/// $l_p = \sum_{k \ge p} (k - p + 1) \cdot g_k$.
///
/// * `histogramm` - Haeufigkeit der Laeufe je Laenge; Index $k-1$.
/// * Rueckgabe: die kumulierte Verteilung $L = (l_p)$.
pub fn kumulierte_verteilung_aus_histogramm(histogramm: &[u64]) -> KumulierteVerteilung {
    // Die kumulierte Verteilung in bekannter Groesse mit Nullen anlegen.
    let mut kumulierte_verteilung = vec![0u64; histogramm.len()];
    // Je Lauflaenge k ihren Beitrag an alle Stellen p <= k aufaddieren.
    for (laufstelle, &haeufigkeit) in histogramm.iter().enumerate() {
        // Frueher Abbruch: unbesetzte Lauflaengen tragen nichts bei.
        if haeufigkeit == 0 {
            continue;
        }
        // Die Lauflaenge ist die Tabellenstelle plus 1.
        let lauflaenge = laufstelle as u64 + 1;
        // Jede Stelle p von 1 bis k erhaelt (k - p + 1) Laeufe als Beitrag.
        for stelle in 1..=lauflaenge {
            // Der Teillauf-Beitrag der Laeufe an der Stelle p.
            let beitrag = (lauflaenge - stelle + 1) * haeufigkeit;
            // Beitrag an der Stelle (0-basiert) aufsummieren.
            kumulierte_verteilung[(stelle - 1) as usize] += beitrag;
        }
    }
    // Die fertige kumulierte Verteilung zurueckgeben.
    kumulierte_verteilung
}
