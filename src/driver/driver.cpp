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

  if (specification <= 0ull) //  // de-DE Ist der Vorgabewert gleich Null
    return true;             //  // de-DE Dann ist nichts zu tun

  uint64_t
      currentAmount = 0ull,         //  // de-DE Die Häufigkeit der Läufe in der Vorgabeverteilung max 0xFFFFFFFF00000000ull
      down = 0xFFFFFFFF00000000ull, //  // de-DE Wert für die obere Grenze der logarithmische Suche max 0xFFFFFFFF00000000ull
      top = 0ull,                   //  // de-DE Wert für die unter Grenze der logarithmische Suche
      rang = 0ull,                  //  // de-DE logarithmisch ermittelten Rang max max 0xFFFFFFFF00000000ull
      currentCumulation = 0ull,     //  // de-DE kumulation der Läufe (nur für die logarithmische Suche) max max 0xFFFFFFFF00000000ull
      inputPuffer = 0ull;           //  // de-DE Der Eingabepuffer anlegen (immer 64 bits)

  uint64_t
      outputBuffer = 0u,        //  // de-DE Der Ausgabepuffer 8bits
      inputCounter = 0u,        //  // de-DE Die Menge der einglesenen Bits  (max 64u)
      outputCounter = 8u,       //  // de-DE Lege die Zähler für die Puffer und den Index über den Vorgabevektor an (max 8)
      size = 0u,                //  // de-DE Lege die Größe der Vorgabeverteilung an (max 64u)
      outputSpecification = 0u; //  // de-DE Die Länge des Laufs, der als nächster ausgegeben wird  (max 64u)

  vector<uint64_t> einfacheVerteilung = //  // de-DE Lege den Vorgabevector an
      getSpecification(specification);  //  // de-DE Initialisiere den Vorgabevector

  vector<pair<uint64_t, uint64_t>> theSpecifiedDistribution; //  // de-DE Den Vektor der Häufigkeitsverteilung anlegen

  while (!einfacheVerteilung.empty())
  {
    if (0u < einfacheVerteilung.back())
      theSpecifiedDistribution.push_back(                                   //  // de-DE ein paar [Lauflänge,Häufigkeit]
          make_pair(einfacheVerteilung.size(), einfacheVerteilung.back())), //  // de-DE Hinten anfügen
          currentAmount += einfacheVerteilung.back();                       //  // de-DE Die Laufmenge aufzählen
    einfacheVerteilung.pop_back();                                          //  // de-DE Den letzten Eintrag der einfachen Verteilung löschen
  }

  size = theSpecifiedDistribution.size(); //  // de-DE Die Länge der Verteilung vormerken

  pair<uint64_t, uint64_t>
      *dataBegin = theSpecifiedDistribution.data(), //  // de-DE Begin der Daten
      *specificationPointer = dataBegin;            //  // de-DE Lege einen Zeiger über die Vorgabeverteilung

  bool
      releaseIt = false,   //  // de-DE Einmaliges Auslassen der Ausgabe beim ersten Durchlauf veranlassen
      readOut = false,     //  // de-DE Ob die Eingabe ausgelesen wurde (wenn ja, dann Abbruch)
      inputStatus = false, //  // de-DE Den eingabeStatus anlegen
      outputStatus = true; //  // de-DE Den ausgabeStatus anlegen.

  //  // de-DE#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
  //  // de-DE#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
  while (0u < size) //  // de-DE Solange die Vorgabe nicht vollständig abgearbeitet ist
  {

    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = gesonderte Initialisierung, wenn die Vorgabeverteilung nur einen Element besitzt  = = = =
    if (size <= 1u) //  // de-DE Ist die Vorgabe auf eine Häufigkeit reduziert?
      releaseIt = true,
      specificationPointer = dataBegin; //  // de-DE Den Vorgabezeiger auf den Datenanfang stellen

    outputSpecification = specificationPointer->first; //  // de-DE Die Vorgabe der Ausgabe speichern

    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = Abarbeitung der Ausgabe = = = = = = = = = = = = = = = = = = = = = = = = = = =
    if (releaseIt) //  // de-DE Ist die ausgabe freigegeben (am Ende des ersten Durchlaufs) ?
    {

      while (0ull < outputSpecification) //  // de-DE Bis der Vorgabewert gleich Null ist.
      {
        outputBuffer |= outputStatus ? 0x1u : 0x0u, //  // de-DE Den kleinste Bit des Ausgabepuffers (zurück) setzen
            outputSpecification -= 1u,              //  // de-DE Die Ausgabevorgabe dekrementieren
            outputCounter -= 1u;                    //  // de-DE Den Ausgabezähler in jedem Fall dekrementieren

        if (outputCounter <= 0u)          //  // de-DE Ist der Ausgabezähler auf Null
          outputStream.put(outputBuffer), //  // de-DE Ausgabepuffer ausschreiben
              outputCounter = 8u,         //  // de-DE Den Ausgabezähler zurücksetzen
              outputBuffer = 0u;          //  // de-DE Den Ausgabepuffer reinitialisieren
        else
          outputBuffer <<= 1u; //  // de-DE Den Ausgabepuffer um eins nach links verschieben
      }

      currentAmount -= 1u,                    //  // de-DE Die Menge der Läufe dekrementieren
          outputStatus = !outputStatus,       //  // de-DE Die ausgabeStatus umstellen
          specificationPointer->second -= 1u; //  // de-DE Die Häufigkeit dekrementieren

      if (specificationPointer->second <= 0u)                                                                  //  // de-DE Ist die Häufgkeit auf Null
        theSpecifiedDistribution.erase(theSpecifiedDistribution.begin() + (specificationPointer - dataBegin)), //  // de-DE Die Häufigkeit aus der Vorgabe löschen
            size = theSpecifiedDistribution.size();                                                            //  // de-DE Die Länge der Vorgabeverteilung dekrementieren

      if (size <= 0u) //  // de-DE Ist die Vorgabeverteilung abgearbeitet ?
      {
        if ((bool)(specification & 7u))        //  // de-DE Ist die Vorgabe durch 8 teilbar ?
          outputBuffer <<= outputCounter - 1u, //  // de-DE Den Ausgabepuffer ggf. nach links verschieben
              outputStream.put(outputBuffer);  //  // de-DE Ausgabepuffer ausschreiben
        break;                                 //  // de-DE Ende der Verarbeitung, wenn die Vorgabeverteilung leer ist
      }

      if (size <= 1u) //  // de-DE Hat die Vorgabeverteilung noch nur eine Stelle
        continue;     //  // de-DE Verarbeitung am Anfang fortsetzen oder beenden, wenn die Vorgabeverteilung leer ist
    }
    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = Abarbeitung der Eingabe = = = = = = = = = = = = = = = = = = = = = = = = = = =
    rang = 0ull,
    top = 0ull,               //  // de-DE Unten ab Null beginnen
        down = currentAmount; //  // de-DE Oben ab dem Anzahl der Läufe beginnen

    while (top < down) //  // de-DE diese Bedingung ist immer wahr
    {

      if (inputCounter <= 0u) //  // de-DE Ist der Eingabepuffer abgearbeitet
      {
        inputStream.read((char *)&inputPuffer, 8u),      //  // de-DE Den Eingabepuffer mit der Eingabe füllen
            inputCounter = inputStream.gcount() << 3u,   //  // de-DE Die Anzahl der gelesenen Bits setzen (0 oder 8)
            readOut = inputCounter <= 0u ? true : false; //  // de-DE Wurde die Eingabe ausgelesen ?

        if (readOut) //  // de-DE Ist die eingabeZaehler gleich 0 (0 Bits wurden gelesen)
          break;     //  // de-DE Die logarithmische Suche abbrechen

        inputPuffer <<= 64u - inputCounter; //  // de-DE Den EingabePuffer ggf. nach links verschieben
      }

      inputPuffer <<= 1u,                                            //  // de-DE Der Eingabepuffer nach link verschieben
          inputCounter -= 1u,                                        //  // de-DE Die eingabeZaehler verringern
          inputStatus = (bool)(inputPuffer & 0x8000000000000000ull); //  // de-DE Den Eingabestatus ermitteln

      if (top + 1u < down) //  // de-DE Ist der Abstand zwichen den Grenzen strikt größer als eins
      {
        if (inputStatus)                //  // de-DE Ist der Eingabebit wahr ?
          down -= ((down - top) >> 1u); //  // de-DE den Wert der oberen Grenze verringern
        else                            //  // de-DE Ist der Lauf der Eingabe nicht zu Ende ?
          top += ((down - top) >> 1u);  //  // de-DE Den Wert der unteren Grenze erhöhen
        continue;                       //  // de-DE erzwingt das Auslesen eines Eingabebits
      }

      break; //  // de-DE beendet die Eingabe, da unten + 1u == oben
    }

    rang = inputStatus ? top : down; //  // de-DE den Rang abschließend bestimmen

    if (readOut) //  // de-DE Ist die eingabeZaehler gleich 0 (0 Bits wurden gelesen)
      break;     //  // de-DE Dann muss die Verarbeitung komplett unterbrochen werden

    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = Bestimmung des passenden Laufs  = = = = = = = = = = = = = = = = = = = = = = =
    specificationPointer = dataBegin,          //  // de-DE Den Vorgabezeiger initialisieren
        currentCumulation = dataBegin->second; //  // de-DE Die Laufkumulation zurücksetzen

    while (currentCumulation < rang and specificationPointer + 1u < dataBegin + size) //  // de-DE Solange die Laufkumulation kleiner ist als der Rang
      ++specificationPointer,                                                         //  // de-DE Den Vorgabezeiger inkrementieren
          currentCumulation += specificationPointer->second;                          //  // de-DE Die Laufkumulation auftsocken
    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    //  // de-DE = = = = = = = = = Nur beim ersten Durchlauf = = = = = = = = = = = = = = = = = = = = = = = = = =
    if (!releaseIt)
      outputStatus = inputStatus, //  // de-DE Den Ausgabestatus initialisieren
          releaseIt = true;       //  // de-DE Die Ausgabe frei geben (nur beim ersten mal relevant)
  }

  return currentAmount == 0ull ? true : false; //  // de-DE Geeigneter Rückgabewert berechnen
}
//  // de-DE#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
//  // de-DE#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
