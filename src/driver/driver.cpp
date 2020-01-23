/*
Auf Branch Generatoren
Volllogarithmischer Vorschub mit Entfernung der abgearbeiteten Häufigkeiten.
*/

#include <vector>
#include <cstdint>
/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <ostream>
#include <istream>

#include "../../header/specifications/groundspecification.h"

using namespace std;

bool treiber(
  istream &eineEingabe,
  uint64_t vorgabeWert,
  ostream &eineAusgabe)
{

 if ( vorgabeWert <= 0ull ) // Ist der Vorgabewert gleich Null
  return true; // Dann ist nichts zu tun

 uint64_t
        laufMenge = 0ull, // Die Häufigkeit der Läufe in der Vorgabeverteilung max 0xFFFFFFFF00000000ull
             oben = 0xFFFFFFFF00000000ull, // Wert für die obere Grenze der logarithmische Suche max 0xFFFFFFFF00000000ull
            unten = 0ull,// Wert für die unter Grenze der logarithmische Suche
             rang = 0ull, // logarithmisch ermittelten Rang max max 0xFFFFFFFF00000000ull
   laufKumulation = 0ull, // kumulation der Läufe (nur für die logarithmische Suche) max max 0xFFFFFFFF00000000ull
    eingabePuffer = 0ull; // Der Eingabepuffer anlegen (immer 64 bits)

 uint64_t
     ausgabePuffer = 0u, // Der Ausgabepuffer 8bits
    eingabeZaehler = 0u, // Die Menge der einglesenen Bits  (max 64u)
    ausgabeZaehler = 8u, // Lege die Zähler für die Puffer und den Index über den Vorgabevektor an (max 8)
              size = 0u, // Lege die Größe der Vorgabeverteilung an (max 64u)
    ausgabeVorgabe = 0u; // Die Länge des Laufs, der als nächster ausgegeben wird  (max 64u)

 vector <uint64_t> einfacheVerteilung = // Lege den Vorgabevector an
  grundVorgabe(vorgabeWert); // Initialisiere den Vorgabevector

 vector< pair <uint64_t,uint64_t> > dieVorgabeverteilung; // Den Vektor der Häufigkeitsverteilung anlegen

 while (!einfacheVerteilung.empty())
 {
  if ( 0u < einfacheVerteilung.back() )
   dieVorgabeverteilung.push_back( // ein paar [Lauflänge,Häufigkeit]
    make_pair(einfacheVerteilung.size(), einfacheVerteilung.back())), // Hinten anfügen
   laufMenge += einfacheVerteilung.back(); // Die Laufmenge aufzählen
  einfacheVerteilung.pop_back(); // Den letzten Eintrag der einfachen Verteilung löschen
 }

 size = dieVorgabeverteilung.size(); // Die Länge der Verteilung vormerken

 pair <uint64_t,uint64_t>
    *datenBeginn = dieVorgabeverteilung.data(), // Begin der Daten
  *vorgabeZeiger = datenBeginn; // Lege einen Zeiger über die Vorgabeverteilung

 bool
      ausgabeFrei = false, // Einmaliges Auslassen der Ausgabe beim ersten Durchlauf veranlassen
       ausgelesen = false, // Ob die Eingabe ausgelesen wurde (wenn ja, dann Abbruch)
    eingabeStatus = false, // Den eingabeStatus anlegen
    ausgabeStatus = true; // Den ausgabeStatus anlegen.

 //#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
 //#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
 while ( 0u < size ) // Solange die Vorgabe nicht vollständig abgearbeitet ist
 {

  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = gesonderte Initialisierung, wenn die Vorgabeverteilung nur einen Element besitzt  = = = =
  if ( size <= 1u ) // Ist die Vorgabe auf eine Häufigkeit reduziert?
      ausgabeFrei = true,
     vorgabeZeiger = datenBeginn; // Den Vorgabezeiger auf den Datenanfang stellen

  ausgabeVorgabe = vorgabeZeiger -> first; // Die Vorgabe der Ausgabe speichern

  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = = = = = = = Abarbeitung der Ausgabe = = = = = = = = = = = = = = = = = = = = = = = = = = =
  if (ausgabeFrei) // Ist die ausgabe freigegeben (am Ende des ersten Durchlaufs) ?
  {

   while ( 0ull < ausgabeVorgabe ) // Bis der Vorgabewert gleich Null ist.
   {
    ausgabePuffer  |= ausgabeStatus? 0x1u : 0x0u, // Den kleinste Bit des Ausgabepuffers (zurück) setzen
    ausgabeVorgabe -= 1u, // Die Ausgabevorgabe dekrementieren
    ausgabeZaehler -= 1u; // Den Ausgabezähler in jedem Fall dekrementieren

    if ( ausgabeZaehler <= 0u ) // Ist der Ausgabezähler auf Null
     eineAusgabe.put(ausgabePuffer), // Ausgabepuffer ausschreiben
     ausgabeZaehler = 8u, // Den Ausgabezähler zurücksetzen
      ausgabePuffer = 0u; // Den Ausgabepuffer reinitialisieren
    else
     ausgabePuffer <<= 1u; // Den Ausgabepuffer um eins nach links verschieben
   }

   laufMenge -= 1u, // Die Menge der Läufe dekrementieren
   ausgabeStatus = !ausgabeStatus, // Die ausgabeStatus umstellen
   vorgabeZeiger -> second -= 1u; // Die Häufigkeit dekrementieren

   if ( vorgabeZeiger -> second <= 0u ) // Ist die Häufgkeit auf Null
    dieVorgabeverteilung.erase ( dieVorgabeverteilung.begin()+(vorgabeZeiger-datenBeginn)), // Die Häufigkeit aus der Vorgabe löschen
    size = dieVorgabeverteilung.size(); // Die Länge der Vorgabeverteilung dekrementieren

   if ( size <= 0u ) // Ist die Vorgabeverteilung abgearbeitet ?
   {
    if ( (bool)(vorgabeWert & 7u) ) // Ist die Vorgabe durch 8 teilbar ?
     ausgabePuffer <<= ausgabeZaehler - 1u , // Den Ausgabepuffer ggf. nach links verschieben
     eineAusgabe.put(ausgabePuffer); // Ausgabepuffer ausschreiben
    break; // Ende der Verarbeitung, wenn die Vorgabeverteilung leer ist
   }

   if (size <= 1u) // Hat die Vorgabeverteilung noch nur eine Stelle
    continue; // Verarbeitung am Anfang fortsetzen oder beenden, wenn die Vorgabeverteilung leer ist
 }
 // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
 // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = = = = = = = Abarbeitung der Eingabe = = = = = = = = = = = = = = = = = = = = = = = = = = =
   rang = 0ull,
  unten = 0ull, // Unten ab Null beginnen
   oben = laufMenge; // Oben ab dem Anzahl der Läufe beginnen

   while ( unten < oben ) // diese Bedingung ist immer wahr
  {

   if ( eingabeZaehler <= 0u ) // Ist der Eingabepuffer abgearbeitet
   {
    eineEingabe.read((char*)&eingabePuffer,8u), // Den Eingabepuffer mit der Eingabe füllen
    eingabeZaehler = eineEingabe.gcount() << 3u, // Die Anzahl der gelesenen Bits setzen (0 oder 8)
    ausgelesen = eingabeZaehler <= 0u ? true : false ; // Wurde die Eingabe ausgelesen ?

    if ( ausgelesen ) // Ist die eingabeZaehler gleich 0 (0 Bits wurden gelesen)
     break; // Die logarithmische Suche abbrechen

    eingabePuffer <<= 64u - eingabeZaehler; // Den EingabePuffer ggf. nach links verschieben
   }

   eingabePuffer <<= 1u, // Der Eingabepuffer nach link verschieben
   eingabeZaehler -= 1u, // Die eingabeZaehler verringern
     eingabeStatus = (bool)(eingabePuffer & 0x8000000000000000ull); // Den Eingabestatus ermitteln

   if (unten + 1u < oben) // Ist der Abstand zwichen den Grenzen strikt größer als eins
   {
    if ( eingabeStatus ) // Ist der Eingabebit wahr ?
     oben -= ((oben-unten)>>1u); // den Wert der oberen Grenze verringern
    else // Ist der Lauf der Eingabe nicht zu Ende ?
     unten += ((oben-unten)>>1u); // Den Wert der unteren Grenze erhöhen
    continue; // erzwingt das Auslesen eines Eingabebits
   }

   break; // beendet die Eingabe, da unten + 1u == oben 
  }

  rang = eingabeStatus ? unten : oben ; // den Rang abschließend bestimmen

  if (ausgelesen) // Ist die eingabeZaehler gleich 0 (0 Bits wurden gelesen)
   break; // Dann muss die Verarbeitung komplett unterbrochen werden

  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = = = = = = = Bestimmung des passenden Laufs  = = = = = = = = = = = = = = = = = = = = = = =
   vorgabeZeiger = datenBeginn, // Den Vorgabezeiger initialisieren
  laufKumulation = datenBeginn -> second ; // Die Laufkumulation zurücksetzen

  while ( laufKumulation < rang and vorgabeZeiger + 1u < datenBeginn + size )  // Solange die Laufkumulation kleiner ist als der Rang
   ++ vorgabeZeiger , // Den Vorgabezeiger inkrementieren
     laufKumulation += vorgabeZeiger -> second ; // Die Laufkumulation auftsocken
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // = = = = = = = = = Nur beim ersten Durchlauf = = = = = = = = = = = = = = = = = = = = = = = = = =
  if (!ausgabeFrei)
   ausgabeStatus = eingabeStatus , // Den Ausgabestatus initialisieren
     ausgabeFrei = true ; // Die Ausgabe frei geben (nur beim ersten mal relevant)
 }

 return laufMenge == 0ull ? true : false ; // Geeigneter Rückgabewert berechnen
}
 //#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
 //#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-

