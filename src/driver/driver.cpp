/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <vector>
#include <cstdint>


#include <ostream>
#include <istream>

#include "../../header/specifications/groundspecification.h"

using namespace std;

bool driver(
    istream &inputStream,
    uint64_t specification,
    ostream &outputStream)
{

  if (specification <= 0ull) // Ist der Vorgabewert gleich Null
    return true;             // Dann ist nichts zu tun

  uint64_t
      currentAmount = 0ull,         // Die Häufigkeit der Läufe in der Vorgabeverteilung max 0xFFFFFFFF00000000ull
      down = 0xFFFFFFFF00000000ull, // Wert für die obere Grenze der logarithmische Suche max 0xFFFFFFFF00000000ull
      top = 0ull,                   // Wert für die unter Grenze der logarithmische Suche
      rang = 0ull,                  // logarithmisch ermittelten Rang max max 0xFFFFFFFF00000000ull
      currentCumulation = 0ull,     // kumulation der Läufe (nur für die logarithmische Suche) max max 0xFFFFFFFF00000000ull
      inputPuffer = 0ull;           // Der Eingabepuffer anlegen (immer 64 bits)

  uint64_t
      outputBuffer = 0u,        // Der Ausgabepuffer 8bits
      inputCounter = 0u,        // Die Menge der einglesenen Bits  (max 64u)
      outputCounter = 8u,       // Lege die Zähler für die Puffer und den Index über den Vorgabevektor an (max 8)
      size = 0u,                // Lege die Größe der Vorgabeverteilung an (max 64u)
      outputSpecification = 0u; // Die Länge des Laufs, der als nächster ausgegeben wird  (max 64u)

  vector<uint64_t> einfacheVerteilung = // Lege den Vorgabevector an
      getSpecification(specification);  // Initialisiere den Vorgabevector

  vector<pair<uint64_t, uint64_t>> theSpecifiedDistribution; // Den Vektor der Häufigkeitsverteilung anlegen

  while (!einfacheVerteilung.empty())
  {
    if (0u < einfacheVerteilung.back())
      theSpecifiedDistribution.push_back(                                   // ein paar [Lauflänge,Häufigkeit]
          make_pair(einfacheVerteilung.size(), einfacheVerteilung.back())), // Hinten anfügen
          currentAmount += einfacheVerteilung.back();                       // Die Laufmenge aufzählen
    einfacheVerteilung.pop_back();                                          // Den letzten Eintrag der einfachen Verteilung löschen
  }

  size = theSpecifiedDistribution.size(); // Die Länge der Verteilung vormerken

  pair<uint64_t, uint64_t>
      *dataBegin = theSpecifiedDistribution.data(), // Begin der Daten
      *specificationPointer = dataBegin;            // Lege einen Zeiger über die Vorgabeverteilung

  bool
      releaseIt = false,   // Einmaliges Auslassen der Ausgabe beim ersten Durchlauf veranlassen
      readOut = false,     // Ob die Eingabe ausgelesen wurde (wenn ja, dann Abbruch)
      inputStatus = false, // Den eingabeStatus anlegen
      outputStatus = true; // Den ausgabeStatus anlegen.

  //#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
  //#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
  while (0u < size) // Solange die Vorgabe nicht vollständig abgearbeitet ist
  {

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = gesonderte Initialisierung, wenn die Vorgabeverteilung nur einen Element besitzt  = = = =
    if (size <= 1u) // Ist die Vorgabe auf eine Häufigkeit reduziert?
      releaseIt = true,
      specificationPointer = dataBegin; // Den Vorgabezeiger auf den Datenanfang stellen

    outputSpecification = specificationPointer->first; // Die Vorgabe der Ausgabe speichern

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = Abarbeitung der Ausgabe = = = = = = = = = = = = = = = = = = = = = = = = = = =
    if (releaseIt) // Ist die ausgabe freigegeben (am Ende des ersten Durchlaufs) ?
    {

      while (0ull < outputSpecification) // Bis der Vorgabewert gleich Null ist.
      {
        outputBuffer |= outputStatus ? 0x1u : 0x0u, // Den kleinste Bit des Ausgabepuffers (zurück) setzen
            outputSpecification -= 1u,              // Die Ausgabevorgabe dekrementieren
            outputCounter -= 1u;                    // Den Ausgabezähler in jedem Fall dekrementieren

        if (outputCounter <= 0u)          // Ist der Ausgabezähler auf Null
          outputStream.put(outputBuffer), // Ausgabepuffer ausschreiben
              outputCounter = 8u,         // Den Ausgabezähler zurücksetzen
              outputBuffer = 0u;          // Den Ausgabepuffer reinitialisieren
        else
          outputBuffer <<= 1u; // Den Ausgabepuffer um eins nach links verschieben
      }

      currentAmount -= 1u,                    // Die Menge der Läufe dekrementieren
          outputStatus = !outputStatus,       // Die ausgabeStatus umstellen
          specificationPointer->second -= 1u; // Die Häufigkeit dekrementieren

      if (specificationPointer->second <= 0u)                                                                  // Ist die Häufgkeit auf Null
        theSpecifiedDistribution.erase(theSpecifiedDistribution.begin() + (specificationPointer - dataBegin)), // Die Häufigkeit aus der Vorgabe löschen
            size = theSpecifiedDistribution.size();                                                            // Die Länge der Vorgabeverteilung dekrementieren

      if (size <= 0u) // Ist die Vorgabeverteilung abgearbeitet ?
      {
        if ((bool)(specification & 7u))        // Ist die Vorgabe durch 8 teilbar ?
          outputBuffer <<= outputCounter - 1u, // Den Ausgabepuffer ggf. nach links verschieben
              outputStream.put(outputBuffer);  // Ausgabepuffer ausschreiben
        break;                                 // Ende der Verarbeitung, wenn die Vorgabeverteilung leer ist
      }

      if (size <= 1u) // Hat die Vorgabeverteilung noch nur eine Stelle
        continue;     // Verarbeitung am Anfang fortsetzen oder beenden, wenn die Vorgabeverteilung leer ist
    }
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = Abarbeitung der Eingabe = = = = = = = = = = = = = = = = = = = = = = = = = = =
    rang = 0ull,
    top = 0ull,               // Unten ab Null beginnen
        down = currentAmount; // Oben ab dem Anzahl der Läufe beginnen

    while (top < down) // diese Bedingung ist immer wahr
    {

      if (inputCounter <= 0u) // Ist der Eingabepuffer abgearbeitet
      {
        inputStream.read((char *)&inputPuffer, 8u),      // Den Eingabepuffer mit der Eingabe füllen
            inputCounter = inputStream.gcount() << 3u,   // Die Anzahl der gelesenen Bits setzen (0 oder 8)
            readOut = inputCounter <= 0u ? true : false; // Wurde die Eingabe ausgelesen ?

        if (readOut) // Ist die eingabeZaehler gleich 0 (0 Bits wurden gelesen)
          break;     // Die logarithmische Suche abbrechen

        inputPuffer <<= 64u - inputCounter; // Den EingabePuffer ggf. nach links verschieben
      }

      inputPuffer <<= 1u,                                            // Der Eingabepuffer nach link verschieben
          inputCounter -= 1u,                                        // Die eingabeZaehler verringern
          inputStatus = (bool)(inputPuffer & 0x8000000000000000ull); // Den Eingabestatus ermitteln

      if (top + 1u < down) // Ist der Abstand zwichen den Grenzen strikt größer als eins
      {
        if (inputStatus)                // Ist der Eingabebit wahr ?
          down -= ((down - top) >> 1u); // den Wert der oberen Grenze verringern
        else                            // Ist der Lauf der Eingabe nicht zu Ende ?
          top += ((down - top) >> 1u);  // Den Wert der unteren Grenze erhöhen
        continue;                       // erzwingt das Auslesen eines Eingabebits
      }

      break; // beendet die Eingabe, da unten + 1u == oben
    }

    rang = inputStatus ? top : down; // den Rang abschließend bestimmen

    if (readOut) // Ist die eingabeZaehler gleich 0 (0 Bits wurden gelesen)
      break;     // Dann muss die Verarbeitung komplett unterbrochen werden

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = Bestimmung des passenden Laufs  = = = = = = = = = = = = = = = = = = = = = = =
    specificationPointer = dataBegin,          // Den Vorgabezeiger initialisieren
        currentCumulation = dataBegin->second; // Die Laufkumulation zurücksetzen

    while (currentCumulation < rang and specificationPointer + 1u < dataBegin + size) // Solange die Laufkumulation kleiner ist als der Rang
      ++specificationPointer,                                                         // Den Vorgabezeiger inkrementieren
          currentCumulation += specificationPointer->second;                          // Die Laufkumulation auftsocken
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    // = = = = = = = = = Nur beim ersten Durchlauf = = = = = = = = = = = = = = = = = = = = = = = = = =
    if (!releaseIt)
      outputStatus = inputStatus, // Den Ausgabestatus initialisieren
          releaseIt = true;       // Die Ausgabe frei geben (nur beim ersten mal relevant)
  }

  return currentAmount == 0ull ? true : false; // Geeigneter Rückgabewert berechnen
}
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
