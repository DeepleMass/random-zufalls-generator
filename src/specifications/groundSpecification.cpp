/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <vector>
#include <cstdint>

using namespace std;
/**
 * get specification produces a distribution of bits runs according
 * to a given specification, which is the length of the random bits
 * series to be computed
 * @param specification is the number of bit to be randomly generated
 * @return a distribution as vector of integer where the index+1 is the
 * run length and the value is the number of runs to be produced
 */ 
vector<uint64_t> getSpecification(uint64_t specificaton)
{

  if (specificaton == 0ull)     //  // de-DE Ist der Vorgabewert gleich Null
    return vector<uint64_t>(0); //  // de-DE Einen leeren Vektor zurückgeben

#ifndef _FULL_
#define _FULL_ 0x7FFFFFFFFFFFFFFFull
  specificaton = _FULL_ < specificaton ? _FULL_ : specificaton;
#endif

  uint64_t
      numberOfIndices = 0ull,                 //  // de-DE Die Anzahl der stellen anlegen
      tmpSpecification = specificaton, //  // de-DE Eine tmp Kopie von vorgabeWert anlegen
      tmpCumulation = 1ull;            //  // de-DE eine temporäre Häufigkeit

  while (0 < (specificaton >> numberOfIndices)) //  // de-DE solange tmpVorgabe nicht Null ist
    numberOfIndices++;                          //  // de-DE Die Anzahl der Stellen Dekrementieren

  vector<uint64_t> theDistribution = 
    vector<uint64_t>(numberOfIndices, 0ull); //  // de-DE Einen Vector für die Verteilung anlegen

  /*
 es gelten die Bitblöcke 
 1 2 3 4 5 6 7 8 . . . MStellen
*/

  uint64_t
      *dataBegin = theDistribution.data(), //  // de-DE einen Zeiger für den Datenbeginn anlegen
      *dataEnd = dataBegin - 1u,
      *pointerIndex = dataBegin,     //  // de-DE einen Laufzeiger anlegen
          *currentIndex = dataBegin; //  // de-DE einen Laufindex anlegen

  tmpSpecification = specificaton; //  // de-DE Den temporären Vorgabewert initialisieren

  do //  // de-DE Abarbeiten des längsten Bitblock der relevanten elementaren Verteilungen
  {
    if (tmpSpecification & 1ull)     //  // de-DE Ist die binäre Stelle relevant (also nicht Null)
      *pointerIndex = 1ull;          //  // de-DE Die gezeigte Stelle um eins  Zeiger auf
    tmpSpecification >>= 1u;         //  // de-DE den temporären Wert nach rechts verschieben
    ++pointerIndex;                  //  // de-DE den Zeiger um eine Stelle im Verteilungsvektor verschieben
  } while (0ull < tmpSpecification); //  // de-DE solange der temporäre Wert nicht Null ist;

  //  // de-DE Abarbeiten der restlichen Bitblöcke der relevanten elementaren Verteilungen
  pointerIndex = dataBegin - 2u,     //  // de-DE Den Zeigerindex am Anfang des Vector verlegen
      currentIndex = dataBegin - 2u; //  // de-DE Den Laufindex am Anfang des Vector verlegen
                                     //  // de-DE vorgabeWert>>=2u; //  // de-DE Den Vorgabewert um 2 Stellen nach rechts verschieben

  while (0ull < specificaton) //  // de-DE solange der Vorgabewert nicht abgearbeitet ist
  {
    if (specificaton & 1ull) //  // de-DE Ist die binäre Stelle relevant (also nicht Null)
    {

      tmpCumulation = 1ull;
      currentIndex = pointerIndex; //  // de-DE Den Laufindex auf die geeignete Stelle positionieren

      while (dataEnd < currentIndex)
        *currentIndex += tmpCumulation, //  // de-DE die aktuelle stelle um die temporäre Häufigkeit erhöhen
            tmpCumulation <<= 1u,       //  // de-DE
            currentIndex--;             //  // de-DE Die aktuelle Stelle ist eine weitere nach links
    }

    specificaton >>= 1u, //  // de-DE Den Vorgabewert um eine Stelle nach rechts verschieben
        pointerIndex++;  //  // de-DE den Zeigerindex inkrementieren
  }

  return theDistribution;
}
