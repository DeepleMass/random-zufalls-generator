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
vector<uint64_t> getSpecification(uint64_t vorgabeWert)
{

 if (vorgabeWert==0ull) // Ist der Vorgabewert gleich Null
  return vector<uint64_t>(0); // Einen leeren Vektor zurükgeben
 vorgabeWert = 0x7FFFFFFFFFFFFFFFull < vorgabeWert ? 0x7FFFFFFFFFFFFFFFull : vorgabeWert ;

 uint64_t
        MStellen = 0ull, // Die Anzahl der stellen anlegen
      tmpVorgabe = vorgabeWert, // Eine tmp Kopie von vorgabeWert anlegen
  tmpHaeufigkeit = 1ull; // eine temporäre Häufigkeit

 while ( 0 < (vorgabeWert>>MStellen) ) // solange tmpVorgabe nicht Null ist 
  MStellen++; // Die Anzahl der Stellen Dekrementieren

 vector<uint64_t> dieVerteilung = vector<uint64_t>(MStellen,0ull); // Einen Vector für die Verteilung anlegen

/*
 es gelten die Bitblöcke 
 1 2 3 4 5 6 7 8 . . . MStellen
*/

 uint64_t
  *datenBeginn = dieVerteilung.data(), // einen Zeiger für den Datenbeginn anlegen
   *datenREnde = datenBeginn -1u ,
  *zeigerIndex = datenBeginn, // einen Laufzeiger anlegen
  *LaufIndex = datenBeginn; // einen Laufindex anlgeben

 tmpVorgabe = vorgabeWert; // Den temporären Vorgabewert initialisieren

 do // Abarbeiten des längsten Bitblock der relevanten elementaren Verteilungen
 {
  if ( tmpVorgabe & 1ull ) // Ist die binäre Stelle relevant (also nicht Null)
   *zeigerIndex = 1ull; // Die gezeigte Stelle um eins  Zeiger auf
  tmpVorgabe >>= 1u; // den temporären Wert nach rechts verschieben
  ++zeigerIndex; // den Zeiger um eine Stelle im Verteilungsvektor verschieben
 } while ( 0ull < tmpVorgabe ); // solange der temporäre Wert nicht Null ist;


// Abarbeiten der restlichen Bitblöcke der relevanten elementaren Verteilungen
 zeigerIndex = datenBeginn-2u, // Den Zeigerindex am Anfang des Vector verlegen
 LaufIndex = datenBeginn-2u; // Den Laufindex am Anfang des Vector verlegen
// vorgabeWert>>=2u; // Den Vorgabewert um 2 Stellen nach rechts verschieben

 while (0ull < vorgabeWert) // solange der Vorgabewert nicht abgearbeitet ist
 {
  if (vorgabeWert & 1ull ) // Ist die binäre Stelle relevant (also nicht Null)
  {

   tmpHaeufigkeit = 1ull;
   LaufIndex = zeigerIndex; // Den Laufindex auf die geeignete Stelle positionieren

   while ( datenREnde < LaufIndex)
        *LaufIndex  += tmpHaeufigkeit, // die aktuelle stelle um die temporäre Häufigkeit erhöhen
    tmpHaeufigkeit <<= 1u, // 
    LaufIndex --; // Die aktuelle Stelle ist eine weitere nach links

  }

  vorgabeWert>>= 1u, // Den Vorgabewert um eine Stelle nach rechts verschieben
  zeigerIndex ++; // den Zeigerindex inkrementieren
 }

/*

 while ( 0ull < vorgabeWert ) // solange der Vorgabewert nicht abgearbeitet ist
 {

  if (vorgabeWert & 1ull ) // Ist die binäre Stelle relevant (also nicht Null)
  {
   *LaufIndex += 1<<(zeigerIndex-LaufIndex); // die Laufende Stelle um Eins erhöhen
   if (LaufIndex==datenBeginn) // ist die erste Stelle erreicht
    vorgabeWert &= ~1ull; // den Vorgabewert 
   --LaufIndex; // den Index auf die nächste Stelle verschieben
  }
  else // Ist die binäre Stelle nicht relevant (also gleich Null)
  {
   vorgabeWert>>=1; // Den Vorgabewert durch 2 Teilen
   ++zeigerIndex; // Den Zeiger index um eins nach rechts verschieben
   LaufIndex=zeigerIndex; // Den Laufindex auf den Zeigerindex verschieben
  }

 } 
*/
 return dieVerteilung;

}
