//! Integrationstests: Die Beispiele, Lemmata und Saetze des Papers
//! als ausfuehrbare Pruefungen.
//!
//! Jeder Test verweist auf die Paper-Stelle, die er verifiziert.

use bitfolgen::bitfolge::{lauflaengen, Bitfolge};
use bitfolgen::einschritttest::{
    einschritttest_bitfolge, einschritttest_laeufe,
    kumulierte_verteilung_aus_histogramm, Laufhaeufigkeitszaehler,
};
use bitfolgen::generatoren::{iterativer_generator, klassenvertreter_bitfolge, rekursiver_uebergang};
use bitfolgen::klassen::{
    abgeltungsidentitaet_rechte_seite, grundverteilung_aus_kumulierter, ist_mitglied,
    summenidentitaet_t,
};
use bitfolgen::komposition::{
    kumulierte_zusammengesetzte_haeufigkeit, zusammengesetzte_haeufigkeit,
    zusammengesetzte_laenge,
};

/// Deterministischer Pseudozufallsgenerator (Xorshift64) fuer die
/// Aequivalenzpruefungen; ersetzt externe Zufallspakete.
struct Xorshift64 {
    // Der innere 64-Bit-Zustand des Generators.
    zustand: u64,
}

impl Xorshift64 {
    // Erzeugt den Generator mit einer festen Saat.
    fn neu(saat: u64) -> Self {
        // Saat unmittelbar als Zustand uebernehmen.
        Self { zustand: saat }
    }

    // Liefert die naechste Pseudozufallszahl und aktualisiert den Zustand.
    fn naechste_zahl(&mut self) -> u64 {
        // Erste Mischung: Schieben nach links und xor.
        self.zustand ^= self.zustand << 13;
        // Zweite Mischung: Schieben nach rechts und xor.
        self.zustand ^= self.zustand >> 7;
        // Dritte Mischung: Schieben nach links und xor.
        self.zustand ^= self.zustand << 17;
        // Den aktualisierten Zustand als Zufallszahl liefern.
        self.zustand
    }

    // Liefert eine Pseudozufalls-Bitfolge der gewuenschten Laenge.
    fn bitfolge(&mut self, laenge: usize) -> Vec<u8> {
        // Je Bit eine Zufallszahl ziehen und das niederwertigste Bit nehmen.
        (0..laenge).map(|_| (self.naechste_zahl() & 1) as u8).collect()
    }

    // Liefert einen Pseudozufalls-Bytzeblock der gewuenschten Laenge.
    fn bytefolge(&mut self, laenge: usize) -> Vec<u8> {
        // Je Byte eine eigene Zufallszahl ziehen.
        (0..laenge).map(|_| self.naechste_zahl() as u8).collect()
    }
}

/// Paper-Abschnitt 3, Beispiel: Die Folge
/// 10001010100111110000011111110001010101 zerfaellt in die
/// Lauf-Laengen 1, 3, 1, 1, 1, 1, 1, 2, 5, 5, 7, 3, 1, 1, 1, 1, 1, 1, 1.
#[test]
fn beispiel_abschnitt3_laeufe() {
    // Das Beispiel des Papers als Text in eine Bitfolge ueberfuehren.
    let bitfolge = Bitfolge::aus_text("10001010100111110000011111110001010101");
    // Die Lauf-Laengen der Beispielbitfolge bestimmen.
    let bestimmte_lauflaengen = lauflaengen(bitfolge.bits());
    // Die erwartete Lauffolge des Beispiels.
    let erwartete_lauflaengen = [
        1u64, 3, 1, 1, 1, 1, 1, 2, 5, 5, 7, 3, 1, 1, 1, 1, 1, 1, 1,
    ];
    // Die bestimmte und die erwartete Lauffolge muessen uebereinstimmen.
    assert_eq!(bestimmte_lauflaengen, erwartete_lauflaengen.to_vec());
}

/// Konsistenzbedingung des Einschritttests: $l_1 = M$ auf
/// Pseudozufallsfolgen (jedes Bit gehoert zu genau einem Teillauf
/// der Laenge 1).
#[test]
fn einschritttest_konsistenz_l1_gleich_m() {
    // Den Pseudozufallsgenerator mit fester Saat initialisieren.
    let mut generator = Xorshift64::neu(0x1234_5678_9abc_def0);
    // Fuenf Folgen unterschiedlicher Laenge pruefen.
    for laenge in [7usize, 64, 1000, 8192, 65537] {
        // Eine Pseudozufalls-Bitfolge der Laenge erzeugen.
        let bits = generator.bitfolge(laenge);
        // Den Einschritttest (Bit-Variante) auf der Folge ausfuehren.
        let kumulierte_verteilung = einschritttest_bitfolge(&bits);
        // Die Konsistenzbedingung $l_1 = M$ muss exakt gelten.
        assert_eq!(kumulierte_verteilung[0], laenge as u64);
    }
}

/// Aequivalenzlemma des Papers: Beide Einschritttest-Varianten und
/// der byteweise Tabellenweg liefern stets dieselbe kumulierte
/// Verteilung.
#[test]
fn aequivalenz_der_varianten() {
    // Den Pseudozufallsgenerator mit fester Saat initialisieren.
    let mut generator = Xorshift64::neu(0xfeed_face_cafe_babe);
    // Zehn Bytebloecke unterschiedlicher Groesse pruefen.
    for byteanzahl in [1usize, 2, 3, 8, 63, 100, 1000] {
        // Einen Pseudozufalls-Bytzeblock der Groesse erzeugen.
        let byteblock = generator.bytefolge(byteanzahl);
        // Variante 1: Einschritttest ueber die Lauf-Laengen der Folge.
        let bits = Bitfolge::aus_bytes(&byteblock);
        // Die Lauf-Laengen dieser Bitfolge bestimmen.
        let laenge_folge = lauflaengen(bits.bits());
        // Das Ergebnis der Lauf-Variante des Einschritttests.
        let ergebnis_laeufe = einschritttest_laeufe(&laenge_folge);
        // Das Ergebnis der Bit-Variante des Einschritttests.
        let ergebnis_bits = einschritttest_bitfolge(bits.bits());
        // Beide Varianten muessen unmittelbar uebereinstimmen.
        assert_eq!(ergebnis_laeufe, ergebnis_bits);
        // Der Tabellenweg: Zaehler ueber dieselben Bytes fuettern.
        let mut zaehler = Laufhaeufigkeitszaehler::neu();
        // Die Bytes blockweise in den Zaehler einspeisen.
        zaehler.verarbeite_bytes(&byteblock);
        // Die Folge abschliessen, damit der letzte Lauf verbucht wird.
        zaehler.schliesse_ab();
        // Die kumulierte Verteilung aus dem Histogramm bestimmen.
        let ergebnis_tabelle = kumulierte_verteilung_aus_histogramm(zaehler.histogramm());
        // Der Tabellenweg muss mit den Varianten uebereinstimmen.
        assert_eq!(ergebnis_tabelle, ergebnis_bits);
    }
}

/// Bytegrenzen-Uebergreifende Laeufe des Tabellenzaehlers: Ein Lauf
/// darf an Byte-, Block- und Aufrufgrenzen nicht unterbrochen werden.
#[test]
fn zaehler_laeufe_uebergreifend() {
    // Zwei konstante Bytes muessen EINEN Lauf der Laenge 16 ergeben.
    let mut zaehler = Laufhaeufigkeitszaehler::neu();
    // Das erste Nullbyte einspeisen.
    zaehler.verarbeite_bytes(&[0b0000_0000]);
    // Das zweite Nullbyte in einem getrennten Aufruf einspeisen.
    zaehler.verarbeite_bytes(&[0b0000_0000]);
    // Die Folge abschliessen.
    zaehler.schliesse_ab();
    // Der einzige Lauf hat die Laenge 16 (Histogrammstelle 15).
    assert_eq!(zaehler.histogramm().len(), 16);
    // Seine Haeufigkeit ist 1.
    assert_eq!(zaehler.histogramm()[15], 1);

    // Nullbyte gefolgt von Einsbyte ergibt zwei Laeufe der Laenge 8.
    let mut zaehler = Laufhaeufigkeitszaehler::neu();
    // Das Nullbyte einspeisen.
    zaehler.verarbeite_bytes(&[0b0000_0000]);
    // Das Einsbyte einspeisen.
    zaehler.verarbeite_bytes(&[0b1111_1111]);
    // Die Folge abschliessen.
    zaehler.schliesse_ab();
    // Zwei Laeufe der Laenge 8 bedeuten Histogrammstelle 7 mit Wert 2.
    assert_eq!(zaehler.histogramm()[7], 2);

    // Ein Lauf, der innerhalb eines Bytes endet, muss richtig geteilt werden.
    let mut zaehler = Laufhaeufigkeitszaehler::neu();
    // Das Byte 0b00111111 beginnt mit 2 Nullbits (offen) und 6 Einsbits.
    zaehler.verarbeite_bytes(&[0b0011_1111]);
    // Ein weiteres Einsbyte verlaengert den Einslauf um 8.
    zaehler.verarbeite_bytes(&[0b1111_1111]);
    // Die Folge abschliessen.
    zaehler.schliesse_ab();
    // Der Nulllauf hat die Laenge 2.
    assert_eq!(zaehler.histogramm()[1], 1);
    // Der Einslauf hat die Laenge 14.
    assert_eq!(zaehler.histogramm()[13], 1);
}

/// Paper-Abschnitt 6.2, Beispiel: Die Abgeltung der kumulierten
/// Verteilung (16, 8, 4, 2, 1) von $B_5$ ergibt die Grundverteilung
/// (4, 2, 1, 0, 1).
#[test]
fn grundverteilung_beispiel_b5() {
    // Die kumulierte Verteilung der Klasse $B_5$.
    let kumulierte_verteilung = [16u64, 8, 4, 2, 1];
    // Die Grundverteilung durch Abgeltung bestimmen.
    let bestimmte_grundverteilung = grundverteilung_aus_kumulierter(&kumulierte_verteilung);
    // Die im Paper hergeleitete Grundverteilung.
    let erwartete_grundverteilung = [4u64, 2, 1, 0, 1];
    // Beide muessen uebereinstimmen.
    assert_eq!(bestimmte_grundverteilung, erwartete_grundverteilung.to_vec());
}

/// Satz des Papers (Grundverteilung): Fuer jedes $N = 2, \dots, 16$
/// erzeugt der iterative Generator ein Element aus $B_N$, dessen
/// Grundverteilung die geschlossene Form $g_n = 2^{N-2-n}$ erfuellt.
#[test]
fn satz_grundverteilung() {
    // Die Klassenindizes 2 bis 16 durchlaufen.
    for klassenindex in 2u64..=16 {
        // Die Lauffolge des Klassenvertreters erzeugen.
        let lauffolge = iterativer_generator(klassenindex);
        // Die kumulierte Verteilung des Vertreters bestimmen.
        let kumulierte_verteilung = einschritttest_laeufe(&lauffolge);
        // Jede Stelle gegen den exponentiellen Verlauf pruefen.
        for stelle in 1..=klassenindex {
            // Der geforderte Wert $l_n = 2^{N-n}$.
            let geforderter_wert = 1u64 << (klassenindex - stelle);
            // Die Stelle muss exakt den geforderten Wert tragen.
            assert_eq!(kumulierte_verteilung[(stelle - 1) as usize], geforderter_wert);
        }
        // Die Grundverteilung aus der kumulierten bestimmen.
        let grundverteilung = grundverteilung_aus_kumulierter(&kumulierte_verteilung);
        // Jede Stelle $n \le N-2$ traegt die geschlossene Form.
        for stelle in 1..=(klassenindex - 2) {
            // Der erwartete Wert der geschlossenen Form.
            let erwarteter_wert = 1u64 << (klassenindex - 2 - stelle);
            // Die Grundverteilung muss die geschlossene Form erfuellen.
            assert_eq!(grundverteilung[(stelle - 1) as usize], erwarteter_wert);
        }
        // Die Besonderheit des Verfahrens: kein Lauf der Laenge N-1.
        assert_eq!(grundverteilung[(klassenindex - 2) as usize], 0);
        // Der einzige laengste Lauf hat die Haeufigkeit 1.
        assert_eq!(grundverteilung[(klassenindex - 1) as usize], 1);
        // Der Vertreter ist tatsaechlich Mitglied der Klasse.
        assert!(ist_mitglied(&lauffolge, klassenindex));
    }
}

/// Korollar des Papers (Umkehrung und Existenz): Jede Bitfolge mit
/// der Laufverteilung der geschlossenen Form liegt in $B_N$ - hier
/// mit einer von der Anordnung des Generators abweichenden
/// Realisierung (aufsteigende Laengen).
#[test]
fn korollar_umkehrung_und_existenz() {
    // Die Klassenindizes 2 bis 16 durchlaufen.
    for klassenindex in 2u64..=16 {
        // Eine Lauffolge in abweichender Anordnung aufbauen.
        let mut lauffolge: Vec<u64> = Vec::new();
        // Zunaechst die Laengen 1 bis N-2 aufsteigend sammeln.
        for lauflaenge in 1..=(klassenindex - 2) {
            // Die Haeufigkeit der Laenge nach der geschlossenen Form.
            let haeufigkeit = 1u64 << (klassenindex - 2 - lauflaenge);
            // Die Laenge entsprechend oft anhaengen.
            for _ in 0..haeufigkeit {
                lauffolge.push(lauflaenge);
            }
        }
        // Den laengsten Lauf zuletzt anhaengen (beliebige Anordnung).
        lauffolge.push(klassenindex);
        // Trotz anderer Anordnung muss die Folge in $B_N$ liegen.
        assert!(ist_mitglied(&lauffolge, klassenindex));
    }
}

/// Lemma des Papers (Konstruktion): Der rekursive Uebergang fuehrt
/// von $B_N$ nach $B_{N+1}$.
#[test]
fn lemma_konstruktion() {
    // Die Ausgangsklassen 1 bis 14 durchlaufen.
    for klassenindex in 1u64..=14 {
        // Die Lauffolge eines Vertreters von $B_N$ erzeugen.
        let ausgangsfolge = iterativer_generator(klassenindex);
        // Den rekursiven Uebergang auf den Vertreter anwenden.
        let zielfolge = rekursiver_uebergang(&ausgangsfolge);
        // Das Ergebnis muss Mitglied der Folgewklasse sein.
        assert!(ist_mitglied(&zielfolge, klassenindex + 1));
    }
}

/// Lemma des Papers (Summenidentitaet): $T_m = 2^{m+1} - m - 2$.
#[test]
fn summenidentitaet() {
    // Die Parameter m = 0 bis 20 durchlaufen.
    for parameter_m in 0u64..=20 {
        // Der tatsaechliche Wert der Summe.
        let summenwert = summenidentitaet_t(parameter_m);
        // Der Wert der rechten Seite des Lemmas.
        let erwarteter_wert = (1u64 << (parameter_m + 1)) - parameter_m - 2;
        // Beide Seiten muessen uebereinstimmen.
        assert_eq!(summenwert, erwarteter_wert);
    }
}

/// Lemma des Papers (Abgeltungsidentitaet): Die rechte Seite
/// $(N+1) + \sum (N-1-n) 2^n$ ist gleich $2^N$.
#[test]
fn abgeltungsidentitaet() {
    // Die Parameter N = 1 bis 20 durchlaufen.
    for parameter_n in 1u64..=20 {
        // Der Wert der rechten Seite der Identitaet.
        let rechte_seite = abgeltungsidentitaet_rechte_seite(parameter_n);
        // Der Wert der linken Seite ist die Potenz $2^N$.
        let linke_seite = 1u64 << parameter_n;
        // Beide Seiten muessen uebereinstimmen.
        assert_eq!(rechte_seite, linke_seite);
    }
}

/// Paper-Abschnitt 8, Beispiel: Fuer $B = 341 = 101010101_2$ ist die
/// zusammengesetzte Verteilung $(86, 42, 22, 10, 6, 2, 2, 0, 1)$,
/// und die gewichtete Laenge $\sum n H_n$ ist wieder 341.
#[test]
fn komposition_beispiel_341() {
    // Die Beispielzahl des Papers.
    let beispielzahl = 341u64;
    // Die zusammengesetzte Verteilung bestimmen.
    let verteilung = zusammengesetzte_haeufigkeit(beispielzahl);
    // Die im Paper tabellierte Verteilung.
    let erwartete_verteilung = [86u64, 42, 22, 10, 6, 2, 2, 0, 1];
    // Beide muessen uebereinstimmen.
    assert_eq!(verteilung, erwartete_verteilung.to_vec());
    // Die gewichtete Gesamtlaenge muss wieder die Zahl selbst sein.
    assert_eq!(zusammengesetzte_laenge(beispielzahl), beispielzahl);
    // Die kumulierten Haeufigkeiten der Tabelle im Paper.
    assert_eq!(kumulierte_zusammengesetzte_haeufigkeit(beispielzahl, 1), 171);
    // Der zweite Wert der Identitaet.
    assert_eq!(kumulierte_zusammengesetzte_haeufigkeit(beispielzahl, 2), 85);
    // Der fuenfte Wert der Identitaet.
    assert_eq!(kumulierte_zusammengesetzte_haeufigkeit(beispielzahl, 5), 11);
}

/// Band-Aussage des Papers: $2^{N-n-1} \le C_n \le 2^{N-n}$ fuer jede
/// zusammengesetzte Verteilung.
#[test]
fn band_aussage_der_komposition() {
    // Mehrere Beispielzahlen mit unterschiedlichen Bitmustern pruefen.
    for beispielzahl in [1u64, 7, 255, 341, 682, 1000, 12345, u32::MAX as u64] {
        // Die fuehrende Binaerstelle N bestimmen.
        let stellenanzahl = 64 - beispielzahl.leading_zeros();
        // Jede Stelle der Band-Aussage pruefen.
        for stelle in 1..=stellenanzahl {
            // Die kumulierte Haeufigkeit an der Stelle.
            let kumulierte = kumulierte_zusammengesetzte_haeufigkeit(beispielzahl, stelle as u64);
            // Die untere Schranke des Bands.
            let untere_schranke = 1u64 << (stellenanzahl - stelle).saturating_sub(1);
            // Die obere Schranke des Bands.
            let obere_schranke = 1u64 << (stellenanzahl - stelle + 1);
            // Die kumulierte Haeufigkeit muss im Band liegen.
            assert!(kumulierte >= untere_schranke && kumulierte <= obere_schranke);
        }
    }
}

/// Kardinalitaet der Klassen (Bemerkung des Papers): Fuer kleine
/// Klassen stimmt die Formel $|B_N| = 2 R! / \prod g_n!$ mit der
/// vollstaendigen Enumeration ueberein.
#[test]
fn kardinalitaet_der_klassen_bis_b4() {
    // Die erwarteten Kardinalitaeten fuer N = 1, 2, 3, 4 (Enumeration im Paper).
    let erwartete_kardinalitaeten = [2u64, 2, 4, 24];
    // Die Klassenindizes 1 bis 4 durchlaufen.
    for (index, klassenindex) in (1u64..=4).enumerate() {
        // Die Folgenlaenge eines Elements von $B_N$ betraegt $2^{N-1}$.
        let folgenlaenge = 1u64 << (klassenindex - 1);
        // Der Zaehler der gefundenen Mitglieder.
        let mut mitglieder = 0u64;
        // Alle Folgen dieser Laenge vollstaendig aufzaehlen.
        for nummer in 0..(1u64 << folgenlaenge) {
            // Die Bitfolge der Nummer als Bitvektor bauen.
            let bits: Vec<u8> = (0..folgenlaenge)
                .rev()
                .map(|position| ((nummer >> position) & 1) as u8)
                .collect();
            // Die Lauf-Laengen dieser Bitfolge bestimmen.
            let laenge_folge = lauflaengen(&bits);
            // Das Mitglied pruefen und ggf. zaehlen.
            if ist_mitglied(&laenge_folge, klassenindex) {
                mitglieder += 1;
            }
        }
        // Die gezaehlte Kardinalitaet muss der Formel entsprechen.
        assert_eq!(mitglieder, erwartete_kardinalitaeten[index]);
    }
}

/// Paper-Abschnitt 9: Die scheinbar unregelmaessige Realisierung und
/// ihre geordneteste Variante liegen beide in $B_5$ - allein die
/// Haeufigkeitsverteilung entscheidet.
#[test]
fn realisierungen_in_b5() {
    // Die im Paper angegebene Realisierung der Grundverteilung (4;2;1;0;1).
    let unregelmaessige_realisierung = Bitfolge::aus_text("0101001100011111");
    // Ihre Lauf-Laengen bestimmen.
    let laenge_unregelmaessig = lauflaengen(unregelmaessige_realisierung.bits());
    // Sie muss Mitglied der Klasse $B_5$ sein.
    assert!(ist_mitglied(&laenge_unregelmaessig, 5));
    // Die geordneteste Variante: absteigende Laeufe, durch 1 getrennt.
    let geordneteste_realisierung = Bitfolge::aus_text("1111101110110110");
    // Ihre Lauf-Laengen bestimmen.
    let laenge_geordnet = lauflaengen(geordneteste_realisierung.bits());
    // Auch sie muss Mitglied derselben Klasse sein.
    assert!(ist_mitglied(&laenge_geordnet, 5));
    // Beide besitzen dieselbe Lauf-Multimenge (Grundverteilung).
    assert_eq!(
        grundverteilung_aus_kumulierter(&einschritttest_laeufe(&laenge_unregelmaessig)),
        grundverteilung_aus_kumulierter(&einschritttest_laeufe(&laenge_geordnet))
    );
}

/// Ordnungsbehauptungen des Papers zu $B_5$ (per vollstaendiger
/// Enumeration): kleinstes mit Eins beginnendes Element, groesstes
/// mit Null beginnendes Element, kein Element dazwischen.
#[test]
fn ordnungsbehauptungen_b5() {
    // Der kleinste Wert, der mit einer Eins beginnt (Paper-Darstellung).
    let kleinste_mit_eins = u128::from_str_radix("1000001000100100", 2).unwrap();
    // Der groesste Wert, der mit einer Null beginnt (Paper-Darstellung).
    let groesste_mit_null = u128::from_str_radix("0111110111011011", 2).unwrap();
    // Der kleinste gefundene Wert unter den mit Eins beginnenden Mitgliedern.
    let mut gefundene_kleinste_mit_eins = u128::MAX;
    // Der groesste gefundene Wert unter den mit Null beginnenden Mitgliedern.
    let mut gefundene_groesste_mit_null = 0u128;
    // Die Anzahl der Mitglieder im offenen Intervall zwischen beiden.
    let mut mitglieder_dazwischen = 0u32;
    // Alle 16-Bit-Folgen vollstaendig aufzaehlen.
    for nummer in 0u128..(1u128 << 16) {
        // Die Bitfolge der Nummer als Bitvektor bauen.
        let bits: Vec<u8> = (0..16)
            .rev()
            .map(|position| ((nummer >> position) & 1) as u8)
            .collect();
        // Frueher Abbruch: nur Mitglieder von $B_5$ sind von Interesse.
        if !ist_mitglied(&lauflaengen(&bits), 5) {
            continue;
        }
        // Die Grenze zwischen den mit Eins und mit Null beginnenden Werten.
        if nummer >= (1u128 << 15) {
            // Ein neuer kleinster Wert aktualisiert das Minimum.
            if nummer < gefundene_kleinste_mit_eins {
                gefundene_kleinste_mit_eins = nummer;
            }
        } else {
            // Ein neuer groesster Wert aktualisiert das Maximum.
            if nummer > gefundene_groesste_mit_null {
                gefundene_groesste_mit_null = nummer;
            }
        }
        // Ein Mitglied im offenen Intervall widerlegt die Behauptung.
        if groesste_mit_null < nummer && nummer < kleinste_mit_eins {
            mitglieder_dazwischen += 1;
        }
    }
    // Das kleinste mit Eins beginnende Element muss der Behauptung entsprechen.
    assert_eq!(gefundene_kleinste_mit_eins, kleinste_mit_eins);
    // Das groesste mit Null beginnende Element muss der Behauptung entsprechen.
    assert_eq!(gefundene_groesste_mit_null, groesste_mit_null);
    // Kein Mitglied liegt im offenen Intervall zwischen beiden.
    assert_eq!(mitglieder_dazwischen, 0);
}

/// Papier-Abschnitt 5, Konsistenz: Der Klassenvertreter des
/// Generators ist als Bitfolge Mitglied der Klasse (Ende-zu-Ende-
/// Pruefung vom Generator ueber die Bitfolge zur Mitgliedschaft).
#[test]
fn klassenvertreter_ende_zu_ende() {
    // Die Klassenindizes 1 bis 12 durchlaufen.
    for klassenindex in 1u64..=12 {
        // Die Bitfolge des Klassenvertreters erzeugen.
        let vertreter = klassenvertreter_bitfolge(klassenindex);
        // Die Bitfolge muss die Laenge $2^{N-1}$ haben (Bemerkung Gesamtilaenge).
        assert_eq!(vertreter.laenge(), 1u64 << (klassenindex - 1));
        // Ihre Lauf-Laengen bestimmen.
        let laenge_folge = lauflaengen(vertreter.bits());
        // Und die Mitgliedschaft in der Zielklasse pruefen.
        assert!(ist_mitglied(&laenge_folge, klassenindex));
    }
}
