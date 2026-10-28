//! Das Kommandozeilenwerkzeug `auswertung` (Paper-Abschnitt 5):
//! Wertet einen Datenbestand von Sammeldateien mit dem Einschritttest
//! aus - Namenssortierung der Dateien entspricht der chronologischen
//! Ordnung der Zeitstempel-Dateinamen.
//!
//! Aufruf: `auswertung <verzeichnis>`
//!
//! Die Ausgabe umfasst die Bitanzahl $M$, die Tabelle der Haeufigkeiten
//! $l_n$ mit Logarithmus und Erwartung $M \cdot 2^{1-n}$, den
//! Chi-Quadrat-Anpassungstest der Lauf-Laengen gegen die geometrische
//! Verteilung und die geschaetzte Uebergangswahrscheinlichkeit.

use std::env;
use std::fs;
use std::io::Read;
use std::path::PathBuf;
use std::process::ExitCode;

use bitfolgen::einschritttest::kumulierte_verteilung_aus_histogramm;
use bitfolgen::einschritttest::Laufhaeufigkeitszaehler;

/// Der Einstiegspunkt des Programms; erwartet das Datenverzeichnis
/// als erstes Argument.
fn main() -> ExitCode {
    // Die Kommandozeilenargumente ohne Programmnamen betrachten.
    let argumente: Vec<String> = env::args().skip(1).collect();
    // Frueher Abbruch: ohne Argument kann kein Verzeichnis gelesen werden.
    if argumente.is_empty() {
        // Hinweis auf die erwartete Aufrufform geben.
        eprintln!("Aufruf: auswertung <verzeichnis>");
        // Das Programm mit Fehlercode beenden.
        return ExitCode::FAILURE;
    }
    // Das Datenverzeichnis aus dem ersten Argument uebernehmen.
    let verzeichnis = PathBuf::from(&argumente[0]);
    // Den Zaehler fuer die gesamte, verkettete Bitfolge anlegen.
    let mut zaehler = Laufhaeufigkeitszaehler::neu();
    // Alle Sammeldateien des Verzeichnisses namenssortiert ermitteln.
    let dateinamen = match sammle_dateinamen(&verzeichnis) {
        // Der glueckliche Fall liefert die sortierte Namensliste.
        Ok(dateinamen) => dateinamen,
        // Lesefehler werden mit Klartext gemeldet.
        Err(fehlermeldung) => {
            // Den Fehler auf die Standardfehlerausgabe schreiben.
            eprintln!("Fehler beim Lesen des Verzeichnisses: {fehlermeldung}");
            // Das Programm mit Fehlercode beenden.
            return ExitCode::FAILURE;
        }
    };
    // Frueher Abbruch: ein leeres Verzeichnis hat nichts zu auszuwerten.
    if dateinamen.is_empty() {
        // Den Sachverhalt auf die Standardfehlerausgabe schreiben.
        eprintln!("Keine Dateien in {} gefunden.", verzeichnis.display());
        // Das Programm mit Fehlercode beenden.
        return ExitCode::FAILURE;
    }
    // Jede Sammeldatei in den Zaehler einspeisen, chronologisch sortiert.
    for dateiname in dateinamen.iter() {
        // Den vollen Pfad der Datei zusammensetzen.
        let dateipfad = verzeichnis.join(dateiname);
        // Die Datei in den Zaehler einspeisen; Fehler melden und abbrechen.
        if let Err(fehlermeldung) = verarbeite_datei(&mut zaehler, &dateipfad) {
            // Den Fehler mit Dateinamen auf die Fehlerausgabe schreiben.
            eprintln!("Fehler bei {}: {fehlermeldung}", dateipfad.display());
            // Das Programm mit Fehlercode beenden.
            return ExitCode::FAILURE;
        }
    }
    // Die Folge abschliessen: der letzte offene Lauf wird verbucht.
    zaehler.schliesse_ab();
    // Die Bitanzahl M ist das Achtfache der Byteanzahl.
    let bitanzahl = zaehler.byteanzahl() * 8;
    // Die kumulierte Verteilung aus dem Lauf-Histogramm berechnen.
    let kumulierte_verteilung = kumulierte_verteilung_aus_histogramm(zaehler.histogramm());
    // Konsistenzpruefung des Papers: $l_1$ muss exakt M sein.
    assert!(
        kumulierte_verteilung[0] == bitanzahl,
        "Konsistenzverletzung l_1 = M"
    );
    // Die Kopfzeile der Ausgabe mit den Sammelzahlen drucken.
    println!("Dateien:          {}", dateinamen.len());
    // Die Anzahl der Bytes des Datenbestands drucken.
    println!("Bytes:            {}", zaehler.byteanzahl());
    // Die Bitanzahl M des Datenbestands drucken.
    println!("Bits M:           {bitanzahl}");
    // Die Laenge des laengsten Laufs P drucken.
    println!("Laengster Lauf P: {}", kumulierte_verteilung.len());
    // Eine Leerzeile vor der Tabelle ausgeben.
    println!();
    // Den Tabellenkopf mit den Spaltenbeschriftungen ausgeben.
    println!(" n |      l_n |  log2(l_n) |  M*2^(1-n)");
    // Die Trennlinie unter dem Tabellenkopf ausgeben.
    println!("---+----------+------------+----------");
    // Die Tabelle auf die ersten 25 Zeilen begrenzen (Rest im Chi-Quadrat).
    for (laufstelle, &haeufigkeit) in kumulierte_verteilung.iter().enumerate().take(25) {
        // Die Lauf-Laenge ist die Tabellenstelle plus 1.
        let stelle = laufstelle as u64 + 1;
        // Der Zweierlogarithmus der Haeufigkeit als Gleitkommazahl.
        let logarithmus = (haeufigkeit as f64).log2();
        // Die theoretische Erwartung $M \cdot 2^{1-n}$.
        let erwartung = bitanzahl as f64 * 2f64.powi(1 - stelle as i32);
        // Die Tabellenzeile der Laenge n ausgeben.
        println!("{stelle:2} | {haeufigkeit:8} | {logarithmus:10.2} | {erwartung:8.0}");
    }
    // Eine Leerzeile nach der Tabelle ausgeben.
    println!();
    // Den Chi-Quadrat-Anpassungstest ausfuehren und dessen Ergebnis drucken.
    drucke_chi_quadrat(zaehler.histogramm());
    // Die geschaetzte Uebergangswahrscheinlichkeit bestimmen und drucken.
    drucke_uebergangswahrscheinlichkeit(zaehler.histogramm(), bitanzahl);
    // Das Programm erfolgreich beenden.
    ExitCode::SUCCESS
}

/// Ermittelt die namenssortierte Liste aller Dateien eines
/// Verzeichnisses (Unterverzeichnisse werden uebergangen).
///
/// * `verzeichnis` - das zu durchsuchende Verzeichnis.
/// * Rueckgabe: sortierte Dateinamen oder eine Fehlermeldung.
fn sammle_dateinamen(verzeichnis: &PathBuf) -> Result<Vec<String>, String> {
    // Das Verzeichnis zum Lesen oeffnen; die Ursache des Fehlers liefern.
    let lese_handle = fs::read_dir(verzeichnis).map_err(|fehler| fehler.to_string())?;
    // Die Namensliste in ausreichender Groesse anlegen.
    let mut dateinamen: Vec<String> = Vec::new();
    // Jeden Verzeichniseintrag einzeln pruefen.
    for eintrag in lese_handle {
        // Den Eintrag entfalten; Fehler als String weitergeben.
        let eintrag = eintrag.map_err(|fehler| fehler.to_string())?;
        // Die Dateieigenschaften des Eintrags bestimmen.
        let eigenschaften = fs::metadata(eintrag.path()).map_err(|fehler| fehler.to_string())?;
        // Frueher Abbruch: Unterverzeichnisse sind keine Sammeldateien.
        if !eigenschaften.is_file() {
            continue;
        }
        // Den Dateinamen als Text uebernehmen; Fehler als String weitergeben.
        let dateiname = eintrag.file_name().into_string().map_err(|name| {
            format!("unzulaessiger Dateiname: {:?}", name)
        })?;
        // Den Dateinamen in die Liste aufnehmen.
        dateinamen.push(dateiname);
    }
    // Die Namensliste lexikographisch sortieren (= chronologisch).
    dateinamen.sort();
    // Die sortierte Liste zurueckgeben.
    Ok(dateinamen)
}

/// Liest eine Datei blockweise und speist sie in den Zaehler; die
/// Laeufe setzen sich ueber Block- und Dateigrenzen fort.
///
/// * `zaehler` - der fortlaufende Lauf-Haeufigkeitszaehler.
/// * `dateipfad` - der Pfad der zu verarbeitenden Datei.
/// * Rueckgabe: leer bei Erfolg, sonst eine Fehlermeldung.
fn verarbeite_datei(zaehler: &mut Laufhaeufigkeitszaehler, dateipfad: &PathBuf) -> Result<(), String> {
    // Die Datei zum Lesen oeffnen; Fehler als String weitergeben.
    let mut datei_handle = fs::File::open(dateipfad).map_err(|fehler| fehler.to_string())?;
    // Der Puffer eines Blocks; 1 MiB balancing zwischen Syscalls und Speicher.
    let mut block = vec![0u8; 1 << 20];
    // Solange Bloecke gelesen werden, diese verarbeiten.
    loop {
        // Den Block fuellen; Fehler als Klartext weitergeben.
        let gelesen = datei_handle
            .read(&mut block)
            .map_err(|fehler| fehler.to_string())?;
        // Frueher Abbruch: Null gelesen heisst Dateiende erreicht.
        if gelesen == 0 {
            break;
        }
        // Den gelesenen Blockausschnitt in den Zaehler einspeisen.
        zaehler.verarbeite_bytes(&block[..gelesen]);
    }
    // Erfolg ohne Fehlermeldung melden.
    Ok(())
}

/// Fuehrt den Chi-Quadrat-Anpassungstest des Lauf-Histogramms gegen
/// die geometrische Verteilung $g_n = R \cdot 2^{-n}$ aus und druckt
/// Statistik, Freiheitsgrade und Verhaeltnis.
///
/// * `histogramm` - die Lauf-Haeufigkeiten je Laenge.
fn drucke_chi_quadrat(histogramm: &[u64]) {
    // Die Gesamtzahl R der Laeufe als Summe des Histogramms.
    let laufanzahl: u64 = histogramm.iter().sum();
    // Die Chi-Quadrat-Statistik beginnend bei Null.
    let mut chi_quadrat = 0f64;
    // Die Anzahl der Freiheitsgrade beginnend bei Null.
    let mut freiheitsgrade = 0u32;
    // Jede Lauflaenge mit ausreichender Erwartung in die Statistik aufnehmen.
    for (laufstelle, &haeufigkeit) in histogramm.iter().enumerate() {
        // Die Lauf-Laenge ist die Tabellenstelle plus 1.
        let stelle = laufstelle as f64 + 1.0;
        // Die Erwartung der Haeufigkeit unter der geometrischen Verteilung.
        let erwartung = laufanzahl as f64 * 2f64.powf(-stelle);
        // Frueher Abbruch: Bloeke unter 10 sind fuer die Naeherung unbrauchbar.
        if erwartung < 10.0 {
            break;
        }
        // Der quadratische Beitrag dieses Blocks zur Statistik.
        let beitrag = (haeufigkeit as f64 - erwartung).powi(2) / erwartung;
        // Den Beitrag auf die Statistik aufschlagen.
        chi_quadrat += beitrag;
        // Einen weiteren Freiheitsgrad vermerken.
        freiheitsgrade += 1;
    }
    // Die Statistik mit Freiheitsgraden und Einordnung drucken.
    println!("Chi-Quadrat:      {chi_quadrat:.1} bei {freiheitsgrade} Freiheitsgraden");
    // Das Verhaeltnis zur Freiheitsgradzahl als Groesse der Abweichung drucken.
    println!("Verhaeltnis:      {:.2} (Werte um 1 sprechen fuer die geometrische Verteilung)",
        chi_quadrat / freiheitsgrade as f64);
}

/// Bestimmt und druckt die geschaetzte Uebergangswahrscheinlichkeit
/// $\hat p$ (Anteil der Bitwechsel unter allen Bitpaaren) samt
/// Standardabweichung und Abweichung von $1/2$ in Sigma-Einheiten.
///
/// * `histogramm` - die Lauf-Haeufigkeiten je Laenge.
/// * `bitanzahl` - die Gesamtzahl M der Bits.
fn drucke_uebergangswahrscheinlichkeit(histogramm: &[u64], bitanzahl: u64) {
    // Die Gesamtzahl R der Laeufe als Summe des Histogramms.
    let laufanzahl: u64 = histogramm.iter().sum();
    // Frueher Abbruch: ohne Paare ist keine Schaetzung moeglich.
    if bitanzahl < 2 {
        return;
    }
    // Die Schaetzung p = (R-1)/(M-1) als Anteil der Wechsel je Paar.
    let schaetzung = (laufanzahl - 1) as f64 / (bitanzahl - 1) as f64;
    // Die Standardabweichung der Schaetzung unter p = 1/2.
    let sigma = (0.25 / (bitanzahl - 1) as f64).sqrt();
    // Die Abweichung der Schaetzung von 1/2 in Sigma-Einheiten.
    let abweichung_in_sigma = (schaetzung - 0.5) / sigma;
    // Die Schaetzung mit Unsicherheit drucken.
    println!("Uebergangsrate:   {schaetzung:.5} +- {sigma:.5} (Erwartung 0.5)");
    // Die Abweichung in Sigma-Einheiten mit Einordnung drucken.
    println!("Abweichung:       {abweichung_in_sigma:.2} Sigma");
}
