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
vector<uint64_t> getGrundDistribution(uint64_t specification, istream &inputStream)
{

       if (specification == 0ull)          // Ist der Vorgabewert auf 0 ?
              return vector<uint64_t>(0u); // so ist nichts zu tun

       uint64_t
           octet = (uint64_t)inputStream.get(),           // Den Eingabepuffer anlegen
           inputStreamCount = inputStream.gcount() << 3u; // Den Zählerindex des Eingabepuffers anlegen (0 oder 8 Bits)

       if (inputStreamCount == 0u)         // Sind Null Bits gelesen worden ?
              return vector<uint64_t>(0u); // Einen leeren Vektor zurückgeben

       if (specification <= inputStreamCount)    // Ist der Eingabepuffer größer oder gleich der Vorgabewert ?
              inputStreamCount = specification;  // Der Eingabepuffer reduzieren
       else                                      // Ist der Eingabepuffer kleiner als der Vorgabewert ?
              specification -= inputStreamCount; // Den Vorgabewert um die Länge des Eingabepuffer verringern

       vector<uint64_t> theGroundDistribution; // Einen Vektor für die Grundverteilung anlegen

       // if (dieGrundverteilung.capacity() < 40u)
       theGroundDistribution.reserve(40u); // Eine typische Länge für die Verteilung

       uint64_t
           size = 0u; // gibt die Länge des Vektors der kumulierten Verteilung an.

       uint64_t
           *dataBegin = theGroundDistribution.data(), // Zeiger zum Beginn der Verteilung
           *dataEnd = dataBegin - 1u,                 // Zeiger vor dem Beginn der Verteilung
               *currentAmount = dataBegin - 1u,       // Zeiger zur Stelle der Verteilung, entsprechend der aktuellen Lauflänge
                   *dataBeginBackup = dataBegin;      // Backupzeiger für den Beginn des Vectors (nur bei realloc relevant)

       int64_t diffBeginAfterRealloc = 0ull; // Abstand zwischen den alten und den neuen Vektoralloziierung

       bool
           inputStatus = (bool)(octet & 0x80u), // Eine Variable für den Vergleich des Status der Eingabe anlegen
           earlyStop = false,                   // Einen Flag, ob die Eingabe VorgabeErreicht wurde, auf false setzen
           specificationFulfilled = false;      // Dieser Flag wird gesestzt, wenn der Vorgabewert vor dem EOF erreicht wird

       // # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + #
       while (true) // Endlos bearbeiten
       {
              // *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
              if (currentAmount > dataEnd + size) // Ist der Vector nicht ausreichend groß (Element eingefüen) ?
              {
                     theGroundDistribution.push_back(0ull); // Den Vector um eine Stelle ausweiten
                     if (theGroundDistribution.data() - dataBeginBackup != 0)
                            dataBegin = theGroundDistribution.data(),                // Zeiger zum Beginn der Verteilung
                                diffBeginAfterRealloc = dataBegin - dataBeginBackup, // Den Abastand zwischen neuen und alten VeKtor
                                dataBeginBackup = dataBegin,                         // Backupzeiger für den Beginn des Vectors (nur bei realloc relevant)
                                dataEnd = dataBegin - 1u,                            // Zeiger vor dem Beginn der Verteilung
                                currentAmount += diffBeginAfterRealloc;              // Zeiger zur Stelle der Verteilung,
                     ++size;                                                         // Die Variable anpassen
              }
              // *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *

              if (specificationFulfilled and inputStreamCount == 0u)
              {
                     *currentAmount += 1;
                     break;
              }

              // + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
              if ((bool)(octet & 0x80u) != inputStatus) // Ist der Status verändert ?
              {
                     if (!(*currentAmount < 0xFFFFFFFFFFFFFFFFull)) // steht einen Variablenüberlauf bevor ?
                            break;                                  // führt zum Abbruch

                     *currentAmount += 1; // Den aktuellen Wert der angezeigten Stelle um ein erhöhen

                     if (earlyStop) // Liegt ein Abbruchzwang vor ?
                            break;  // Die bearbeitung Abbrechen

                     inputStatus = !inputStatus,    // Den Eingabestatus umschalten
                         currentAmount = dataBegin; // Den aktuellen Lauf auf Datenanfang zurücksetzen
              }
              // + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
              else
                     ++currentAmount;
              // + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =

              octet <<= 1u,               // Den Eingabepuffer um eine Stelle nach links verschieben
                  inputStreamCount -= 1u; // Den Zählerindex des Eingabepuffers um eins verringern

              // + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
              if (inputStreamCount == 0u and !specificationFulfilled) // ist der Lauf Null und die Eingabe noch nicht eingelesen ?
              {
                     octet = inputStream.get(),                         // Den Eingabepuffer aus der Eingabe füllen
                         inputStreamCount = inputStream.gcount() << 3u; // Die Anzahl der gelesenen Bits setzen (0 oder 8)

                     if (inputStreamCount == 0u)    // Konnte keinen Wert aus der Eingabe gelesen werden
                            octet = 0u,             // oktet auf null zurückstzen
                                inputStatus = true, // den Eingabestatus auf true (erzwingt der increment der aktuellen Stelle)
                                earlyStop = true;

                     if (specification < inputStreamCount)    // Ist der Eingabepuffer größer oder gleich der Vorgabewert ?
                            inputStreamCount = specification, // Den Eingabepuffer bis auf den Vorgabewert reduzieren
                                specificationFulfilled = true;
                     else                                      // Ist der Eingabepuffer kleiner als der Vorgabewert ?
                            specification -= inputStreamCount; // Den Vorgabewert um die Länge des Eingabepuffer verringern
              }

              // + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
       }
       // # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + #

       return theGroundDistribution; // Die Grundverteilung zurückgeben
}
