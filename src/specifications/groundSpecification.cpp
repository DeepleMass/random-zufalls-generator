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
// ###########################################################
vector<uint64_t> getSpecification(uint64_t specificaton)
{

  if (specificaton == 0ull)     // Ist der Vorgabewert gleich Null
    return vector<uint64_t>(0); // Einen leeren Vektor zurükgeben

#ifndef _FULL_
#define _FULL_ 0x7FFFFFFFFFFFFFFFull
  specificaton = _FULL_ < specificaton ? _FULL_ : specificaton;
#endif

  uint64_t
      MStellen = 0ull,                 // Die Anzahl der stellen anlegen
      tmpSpecification = specificaton, // Eine tmp Kopie von vorgabeWert anlegen
      tmpCumulation = 1ull;            // eine temporäre Häufigkeit

  while (0 < (specificaton >> MStellen)) // solange tmpVorgabe nicht Null ist
    MStellen++;                          // Die Anzahl der Stellen Dekrementieren

  vector<uint64_t> theDistribution = vector<uint64_t>(MStellen, 0ull); // Einen Vector für die Verteilung anlegen

  /*
 es gelten die Bitblöcke 
 1 2 3 4 5 6 7 8 . . . MStellen
*/

  uint64_t
      *dataBegin = theDistribution.data(), // einen Zeiger für den Datenbeginn anlegen
      *dataEnd = dataBegin - 1u,
      *pointerIndex = dataBegin,     // einen Laufzeiger anlegen
          *currentIndex = dataBegin; // einen Laufindex anlgeben

  tmpSpecification = specificaton; // Den temporären Vorgabewert initialisieren

  do // Abarbeiten des längsten Bitblock der relevanten elementaren Verteilungen
  {
    if (tmpSpecification & 1ull)     // Ist die binäre Stelle relevant (also nicht Null)
      *pointerIndex = 1ull;          // Die gezeigte Stelle um eins  Zeiger auf
    tmpSpecification >>= 1u;         // den temporären Wert nach rechts verschieben
    ++pointerIndex;                  // den Zeiger um eine Stelle im Verteilungsvektor verschieben
  } while (0ull < tmpSpecification); // solange der temporäre Wert nicht Null ist;

  // Abarbeiten der restlichen Bitblöcke der relevanten elementaren Verteilungen
  pointerIndex = dataBegin - 2u,     // Den Zeigerindex am Anfang des Vector verlegen
      currentIndex = dataBegin - 2u; // Den Laufindex am Anfang des Vector verlegen
                                     // vorgabeWert>>=2u; // Den Vorgabewert um 2 Stellen nach rechts verschieben

  while (0ull < specificaton) // solange der Vorgabewert nicht abgearbeitet ist
  {
    if (specificaton & 1ull) // Ist die binäre Stelle relevant (also nicht Null)
    {

      tmpCumulation = 1ull;
      currentIndex = pointerIndex; // Den Laufindex auf die geeignete Stelle positionieren

      while (dataEnd < currentIndex)
        *currentIndex += tmpCumulation, // die aktuelle stelle um die temporäre Häufigkeit erhöhen
            tmpCumulation <<= 1u,       //
            currentIndex--;             // Die aktuelle Stelle ist eine weitere nach links
    }

    specificaton >>= 1u, // Den Vorgabewert um eine Stelle nach rechts verschieben
        pointerIndex++;  // den Zeigerindex inkrementieren
  }

  return theDistribution;
}
