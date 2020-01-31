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

/**
 * driver is the core function of egef.
 *  
 * @param inputStream This is the input stream. The highest ist entropy,
 * the fastest the driver is done with producing the output stream of bit
 * having maximal entropy according to paper.
 * 
 * @param specification The number of bits the output stream shall provide.
 * (not to be understood as number of Bytes)
 * 
 * @param outputStream This is the output stream. It entropy is said to be maximal.
 * Say it is a random bit series
 *
 */
bool driver(
    istream &inputStream,
    uint64_t specification,
    ostream &outputStream)
{

  if (specification <= 0ull) // if the specification is zero // de-DE Ist der Vorgabewert gleich Null
    return true;             // there is nothing to do // de-DE Dann ist nichts zu tun

  uint64_t
      currentAmount = 0ull,         // the amount of the current run length in the ground distribution // de-DE Die Häufigkeit der Läufe in der Vorgabeverteilung max 0xFFFFFFFF00000000ull
      down = 0xFFFFFFFF00000000ull, // upper bound for the log search // de-DE Wert für die obere Grenze der logarithmische Suche max 0xFFFFFFFF00000000ull
      top = 0ull,                   // lower bound for the log search // de-DE Wert für die unter Grenze der logarithmische Suche
      rang = 0ull,                  // the current rang // de-DE logarithmisch ermittelten Rang max max 0xFFFFFFFF00000000ull
      currentCumulation = 0ull,     // the current runs cumulation // de-DE kumulation der Läufe (nur für die logarithmische Suche) max max 0xFFFFFFFF00000000ull
      inputPuffer = 0ull;           // the input buffer // de-DE Der Eingabepuffer anlegen (immer 64 bits)

  uint64_t
      outputBuffer = 0u,        // the output buffer (64 bits)  // de-DE Der Ausgabepuffer 8bits
      inputCounter = 0u,        // the number of read input  bit  // de-DE Die Menge der einglesenen Bits  (max 64u)
      outputCounter = 8u,       // the output buffer counter and the index of specification // de-DE Lege die Zähler für die Puffer und den Index über den Vorgabevektor an (max 8)
      size = 0u,                // the size of ground distribution// de-DE Lege die Größe der Vorgabeverteilung an (max 64u)
      outputSpecification = 0u; // the length of next run // de-DE Die Länge des Laufs, der als nächster ausgegeben wird  (max 64u)

  vector<uint64_t> simpleDistribution = // the specification vector // de-DE Lege den Vorgabevector an
      getSpecification(specification);  // initialize using the specification and ground distribution // de-DE Initialisiere den Vorgabevector

  vector<pair<uint64_t, uint64_t>> theSpecifiedDistribution; // the vector of pair for run length and distribution // de-DE Den Vektor der Häufigkeitsverteilung anlegen

  while (!simpleDistribution.empty()) // we process all of it until it is empty
  {

    if (0u < simpleDistribution.back()) // we populate the vector of pairs

      theSpecifiedDistribution.push_back(                                   // a pair of run length and amount // de-DE ein paar [Lauflänge,Häufigkeit]
          make_pair(simpleDistribution.size(), simpleDistribution.back())), // to be pushed back // de-DE Hinten anfügen
          currentAmount += simpleDistribution.back();                       // increment the current amount // de-DE Die Laufmenge aufzählen

    simpleDistribution.pop_back();                                          // remove the last entry from the simple distribution // de-DE Den letzten Eintrag der einfachen Verteilung löschen
  }

  size = theSpecifiedDistribution.size(); // get the size of the specified distribution // de-DE Die Länge der Verteilung vormerken

  pair<uint64_t, uint64_t>
      *dataBegin = theSpecifiedDistribution.data(), // a pointer to the data begin // de-DE Begin der Daten
      *specificationPointer = dataBegin;            // a pointer to the specification distribution // de-DE Lege einen Zeiger über die Vorgabeverteilung

  bool
      releaseIt = false,   // at the first loop we don`t process the output // de-DE Einmaliges Auslassen der Ausgabe beim ersten Durchlauf veranlassen
      readOut = false,     // set when the input has been read out (stops processing) // de-DE Ob die Eingabe ausgelesen wurde (wenn ja, dann Abbruch)
      inputStatus = false, // the status of input // de-DE Den eingabeStatus anlegen
      outputStatus = true; // the status of output // de-DE Den ausgabeStatus anlegen.
  
  while (0u < size) // while the specification is not satisfied // de-DE Solange die Vorgabe nicht vollständig abgearbeitet ist
  {

    // special case if the specification distribution has only one element // de-DE = gesonderte Initialisierung, wenn die Vorgabeverteilung nur einen Element besitzt 
    if (size <= 1u) //  // de-DE Ist die Vorgabe auf eine Stelle reduziert?
      releaseIt = true, // set the release flag
      specificationPointer = dataBegin; // set the specification pointer to the first index of distribution // de-DE Den Vorgabezeiger auf den Datenanfang stellen

    outputSpecification = specificationPointer->first; // set the current tmp output specification // de-DE Die Vorgabe der Ausgabe speichern

    // processing the output // de-DE = Abarbeitung der Ausgabe =
    if (releaseIt) // if the output shall be released // de-DE Ist die ausgabe freigegeben (am Ende des ersten Durchlaufs) ?
    {

      while (0ull < outputSpecification) // until the output specification is not zero // de-DE Bis der Vorgabewert gleich Null ist.
      {

        outputBuffer |= outputStatus ? 0x1u : 0x0u, // the lower bit of output buffer // de-DE Den kleinste Bit des Ausgabepuffers (zurück) setzen
            outputSpecification -= 1u,              // decrement the output specification // de-DE Die Ausgabevorgabe dekrementieren
            outputCounter -= 1u;                    // decrement the output counter // de-DE Den Ausgabezähler in jedem Fall dekrementieren

        if (outputCounter <= 0u)          // if the output counter is zero // de-DE Ist der Ausgabezähler auf Null

          outputStream.put(outputBuffer), // write the output buffer on the output stream // de-DE Ausgabepuffer ausschreiben
              outputCounter = 8u,         // reset the output counter // de-DE Den Ausgabezähler zurücksetzen
              outputBuffer = 0u;          // reset the output buffer // de-DE Den Ausgabepuffer reinitialisieren
        else

          outputBuffer <<= 1u; // shift the output buffer to the left // de-DE Den Ausgabepuffer um eins nach links verschieben
      }

      currentAmount -= 1u,                    // decrement the current amount // de-DE Die Menge der Läufe dekrementieren
          outputStatus = !outputStatus,       // toggle the output status // de-DE Die ausgabeStatus umstellen
          specificationPointer->second -= 1u; // decrement the specified amount // de-DE Die Häufigkeit dekrementieren

      if (specificationPointer->second <= 0u)     // is this specified amount zero // de-DE Ist die Häufigkeit auf Null
        theSpecifiedDistribution.erase(theSpecifiedDistribution.begin() + (specificationPointer - dataBegin)), // remove this from the ground distribution // de-DE Die Häufigkeit aus der Vorgabe löschen
        size = theSpecifiedDistribution.size();                                                            // decrement the ground distribution // de-DE Die Länge der Vorgabeverteilung dekrementieren

      if (size <= 0u) // if the specification is not satisfied  // de-DE Ist die Vorgabeverteilung abgearbeitet ?
      {

        if ((bool)(specification & 7u))        // if the specification is a multiple of eight // de-DE Ist die Vorgabe durch 8 teilbar ?

          outputBuffer <<= outputCounter - 1u, // shift the output buffer to the left // de-DE Den Ausgabepuffer ggf. nach links verschieben
              outputStream.put(outputBuffer);  // write the output buffer into out to the output stream // de-DE Ausgabepuffer ausschreiben
        break;                                 // the process has reached its end // de-DE Ende der Verarbeitung, wenn die Vorgabeverteilung leer ist
      }

      if (size <= 1u) // if the specification has only one entry left // de-DE Hat die Vorgabeverteilung noch nur eine Stelle

        continue;     // repeat from then begin or stop if the specifications are satisfied // de-DE Verarbeitung am Anfang fortsetzen oder beenden, wenn die Vorgabeverteilung leer ist
    }

    // process the input // de-DE = Abarbeitung der Eingabe =

    rang = 0ull, // the rang is set to zero
    top = 0ull,               // the top is set to zero // de-DE Unten ab Null beginnen
        down = currentAmount; // the lower bound is set to the current amout // de-DE Oben ab dem Anzahl der Läufe beginnen

    while (top < down) // this is always true // de-DE diese Bedingung ist immer wahr
    {

      if (inputCounter <= 0u) // if the input buffer has been read // de-DE Ist der Eingabepuffer abgearbeitet
      {

        inputStream.read((char *)&inputPuffer, 8u),      // fill the input buffer // de-DE Den Eingabepuffer mit der Eingabe füllen
            inputCounter = inputStream.gcount() << 3u,   // set the number or read bits // de-DE Die Anzahl der gelesenen Bits setzen (0 oder 8)
            readOut = inputCounter <= 0u ? true : false; // if the input stream has been read out // de-DE Wurde die Eingabe ausgelesen ?

        if (readOut) // if the input buffer has been read out // de-DE Ist die eingabeZaehler gleich 0 (0 Bits wurden gelesen)
          break;     // abort the logarithm search // de-DE Die logarithmische Suche abbrechen

        inputPuffer <<= 64u - inputCounter; // shift the input buffer to the left // de-DE Den EingabePuffer ggf. nach links verschieben
      }

      inputPuffer <<= 1u,                                            // shifts the input buffer to the left // de-DE Der Eingabepuffer nach link verschieben
          inputCounter -= 1u,                                        // decrement the input counter // de-DE Die eingabeZähler verringern
          inputStatus = (bool)(inputPuffer & 0x8000000000000000ull); // get the value of the input status // de-DE Den Eingabestatus ermitteln

      if (top + 1u < down) // the distance between bounds is at least one // de-DE Ist der Abstand zwichen den Grenzen strikt größer als eins
      {
        if (inputStatus)                // is the input bit true // de-DE Ist der Eingabebit wahr ?

          down -= ((down - top) >> 1u); // decrement the upper bound // de-DE den Wert der oberen Grenze verringern
        else                            // if the input run has not reached its end // de-DE Ist der Lauf der Eingabe nicht zu Ende ?

          top += ((down - top) >> 1u);  // increment the lower bound // de-DE Den Wert der unteren Grenze erhöhen
        continue;                       // have to read some new bits // de-DE erzwingt das Auslesen eines Eingabebits

      }

      break; // stops the computation // de-DE beendet die Eingabe, da unten + 1u == oben
    }

    rang = inputStatus ? top : down; // compute the rang // de-DE den Rang abschließend bestimmen

    if (readOut) // if the input counter is zero // de-DE Ist die Eingabezähler gleich 0 (0 Bits wurden gelesen)
      break;     // abort the computation // de-DE Dann muss die Verarbeitung komplett unterbrochen werden

    // find the next run lengths // de-DE = Bestimmung des passenden Laufs  =
    specificationPointer = dataBegin,          // initialize the specification pointer // de-DE Den Vorgabezeiger initialisieren
        currentCumulation = dataBegin->second; // reset the current cumulation // de-DE Die Laufkumulation zurücksetzen

    while (currentCumulation < rang and specificationPointer + 1u < dataBegin + size) // until the current cumulation is smaller than the rang // de-DE Solange die Laufkumulation kleiner ist als der Rang
      ++specificationPointer,                                                         // increment the specification pointer // de-DE Den Vorgabezeiger inkrementieren
          currentCumulation += specificationPointer->second;                          // increment the current cumulation // de-DE Die Laufkumulation aufstocken
  
    // this is done only at the first loop // de-DE = Nur beim ersten Durchlauf
    if (!releaseIt)
      outputStatus = inputStatus, // initialize the output status  // de-DE Den Ausgabestatus initialisieren
          releaseIt = true;       // set the ouput free // de-DE Die Ausgabe frei geben (nur beim ersten mal relevant)
  }

  return currentAmount == 0ull ? true : false; // return if the driver has end well // de-DE Geeigneter Rückgabewert berechnen
}