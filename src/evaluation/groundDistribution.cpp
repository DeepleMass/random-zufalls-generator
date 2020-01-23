/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <vector>
#include <cstdint>
#include <istream>

using namespace std;

// ################################################################################
vector<uint64_t> grundverteilung(uint64_t vorgabeWert,istream & eineEingabe)
{

 if (vorgabeWert == 0ull) // Ist der Vorgabewert auf 0 ?
  return vector<uint64_t>(0u); // so ist nichts zu tun

 uint64_t
         oktet = (uint64_t) eineEingabe.get(), // Den Eingabepuffer anlegen
  eingabeMenge = eineEingabe.gcount()<<3u; // Den Zählerindex des Eingabepuffers anlegen (0 oder 8 Bits)

 if (eingabeMenge == 0u) // Sind Null Bits gelesen worden ?
  return vector<uint64_t>(0u); // Einen leeren Vektor zurückgeben

 if (vorgabeWert <= eingabeMenge) // Ist der Eingabepuffer größer oder gleich der Vorgabewert ?
  eingabeMenge = vorgabeWert; // Der Eingabepuffer reduzieren
 else // Ist der Eingabepuffer kleiner als der Vorgabewert ?
  vorgabeWert -= eingabeMenge; // Den Vorgabewert um die Länge des Eingabepuffer verringern

 vector<uint64_t> dieGrundverteilung; // Einen Vektor für die Grundverteilung anlegen

// if (dieGrundverteilung.capacity() < 40u)
  dieGrundverteilung.reserve(40u); // Eine typische Länge für die Verteilung

 uint64_t
        size = 0u; // gibt die Länge des Vektors der kumulierten Verteilung an.

 uint64_t
        *datenBegin = dieGrundverteilung.data(), // Zeiger zum Beginn der Verteilung
        *datenREnde = datenBegin-1u, // Zeiger vor dem Beginn der Verteilung
         *laufMenge = datenBegin-1u, // Zeiger zur Stelle der Verteilung, entsprechend der aktuellen Lauflänge
  *datenBeginBackup = datenBegin; // Backupzeiger für den Beginn des Vectors (nur bei realloc relevant)

 int64_t diffBeginNachRealloc = 0ull; // Abstand zwischen den alten und den neuen Vektoralloziierung

 bool
    eingabeStatus = (bool)(oktet & 0x80u), // Eine Variable für den Vergleich des Status der Eingabe anlegen
     abbruchZwang = false, // Einen Flag, ob die Eingabe VorgabeErreicht wurde, auf false setzen
  VorgabeErreicht = false; // Dieser Flag wird gesestzt, wenn der Vorgabewert vor dem EOF erreicht wird

 // # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + #
 while (true) // Endlos bearbeiten
 {
   // *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * 
   if (laufMenge > datenREnde+size) // Ist der Vector nicht ausreichend groß (Element eingefüen) ?
   {
    dieGrundverteilung.push_back(0ull); // Den Vector um eine Stelle ausweiten
    if (dieGrundverteilung.data()-datenBeginBackup!=0)
               datenBegin = dieGrundverteilung.data(), // Zeiger zum Beginn der Verteilung
     diffBeginNachRealloc = datenBegin-datenBeginBackup,// Den Abastand zwischen neuen und alten VeKtor
         datenBeginBackup = datenBegin, // Backupzeiger für den Beginn des Vectors (nur bei realloc relevant)
               datenREnde = datenBegin-1u, // Zeiger vor dem Beginn der Verteilung
               laufMenge += diffBeginNachRealloc; // Zeiger zur Stelle der Verteilung,
    ++size; // Die Variable anpassen
   }
   // *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * 

   if (VorgabeErreicht and eingabeMenge == 0u)
   {
    *laufMenge += 1;
    break;
   }

  // + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
  if ((bool)(oktet & 0x80u) != eingabeStatus) // Ist der Status verändert ?
  {
   if (!(*laufMenge < 0xFFFFFFFFFFFFFFFFull)) // steht einen Variablenüberlauf bevor ?
    break; // führt zum Abbruch

   *laufMenge += 1; // Den aktuellen Wert der angezeigten Stelle um ein erhöhen

   if (abbruchZwang) // Liegt ein Abbruchzwang vor ?
    break; // Die bearbeitung Abbrechen

   eingabeStatus = !eingabeStatus, // Den Eingabestatus umschalten
       laufMenge = datenBegin; // Den aktuellen Lauf auf Datenanfang zurücksetzen
  }
  // + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
  else 
    ++laufMenge;
  // + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =

         oktet <<= 1u, // Den Eingabepuffer um eine Stelle nach links verschieben
  eingabeMenge  -= 1u; // Den Zählerindex des Eingabepuffers um eins verringern

  // + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
  if (eingabeMenge == 0u and !VorgabeErreicht) // ist der Lauf Null und die Eingabe noch nicht eingelesen ?
  {
          oktet = eineEingabe.get(), // Den Eingabepuffer aus der Eingabe füllen
   eingabeMenge = eineEingabe.gcount()<<3u; // Die Anzahl der gelesenen Bits setzen (0 oder 8)

   if (eingabeMenge == 0u) // Konnte keinen Wert aus der Eingabe gelesen werden
            oktet = 0u, // oktet auf null zurückstzen
    eingabeStatus = true, // den Eingabestatus auf true (erzwingt der increment der aktuellen Stelle)
     abbruchZwang = true;

   if (vorgabeWert < eingabeMenge) // Ist der Eingabepuffer größer oder gleich der Vorgabewert ?
       eingabeMenge = vorgabeWert, // Den Eingabepuffer bis auf den Vorgabewert reduzieren
    VorgabeErreicht = true;
   else // Ist der Eingabepuffer kleiner als der Vorgabewert ?
    vorgabeWert -= eingabeMenge; // Den Vorgabewert um die Länge des Eingabepuffer verringern
  }

  // + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
 }
 // # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + #

 return dieGrundverteilung; // Die Grundverteilung zurückgeben
}

