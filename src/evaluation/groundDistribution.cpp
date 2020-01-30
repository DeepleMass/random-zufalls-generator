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

/**
* groundDistribution 
* @param specification The number of bits the output stream shall provide.
* (not to be understood as number of Bytes)
*
* @param inputStream  This is the input stream. The highest ist entropy,
 * the fastest the driver is done with producing the output stream of bit
 * having maximal entropy according to paper.
*
* @return a vector of integer 
*/
vector<uint64_t> getGroundDistribution(uint64_t specification, istream &inputStream)
{

       if (specification == 0ull)          //  // de-DE Ist der Vorgabewert auf 0 ?
              return vector<uint64_t>(0u); //  // de-DE so ist nichts zu tun

       uint64_t
           octet = (uint64_t)inputStream.get(),           //  // de-DE Den Eingabepuffer anlegen
           inputStreamCount = inputStream.gcount() << 3u; //  // de-DE Den Zählerindex des Eingabepuffers anlegen (0 oder 8 Bits)

       if (inputStreamCount == 0u)         //  // de-DE Sind Null Bits gelesen worden ?
              return vector<uint64_t>(0u); //  // de-DE Einen leeren Vektor zurückgeben

       if (specification <= inputStreamCount)    //  // de-DE Ist der Eingabepuffer größer oder gleich der Vorgabewert ?
              inputStreamCount = specification;  //  // de-DE Der Eingabepuffer reduzieren
       else                                      //  // de-DE Ist der Eingabepuffer kleiner als der Vorgabewert ?
              specification -= inputStreamCount; //  // de-DE Den Vorgabewert um die Länge des Eingabepuffer verringern

       vector<uint64_t> theGroundDistribution; //  // de-DE Einen Vektor für die Grundverteilung anlegen

       //  // de-DE if (dieGrundverteilung.capacity() < 40u)
       theGroundDistribution.reserve(40u); //  // de-DE Eine typische Länge für die Verteilung

       uint64_t
           size = 0u; //  // de-DE gibt die Länge des Vektors der kumulierten Verteilung an.

       uint64_t
           *dataBegin = theGroundDistribution.data(), //  // de-DE Zeiger zum Beginn der Verteilung
           *dataEnd = dataBegin - 1u,                 //  // de-DE Zeiger vor dem Beginn der Verteilung
               *currentAmount = dataBegin - 1u,       //  // de-DE Zeiger zur Stelle der Verteilung, entsprechend der aktuellen Lauflänge
                   *dataBeginBackup = dataBegin;      //  // de-DE Backupzeiger für den Beginn des Vectors (nur bei realloc relevant)

       int64_t diffBeginAfterRealloc = 0ull; //  // de-DE Abstand zwischen den alten und den neuen Vektoralloziierung

       bool
           inputStatus = (bool)(octet & 0x80u), //  // de-DE Eine Variable für den Vergleich des Status der Eingabe anlegen
           earlyStop = false,                   //  // de-DE Einen Flag, ob die Eingabe VorgabeErreicht wurde, auf false setzen
           specificationFulfilled = false;      //  // de-DE Dieser Flag wird gesestzt, wenn der Vorgabewert vor dem EOF erreicht wird

       //  // de-DE # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + #
       while (true) //  // de-DE Endlos bearbeiten
       {
              //  // de-DE *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
              if (currentAmount > dataEnd + size) //  // de-DE Ist der Vector nicht ausreichend groß (Element eingefüen) ?
              {
                     theGroundDistribution.push_back(0ull); //  // de-DE Den Vector um eine Stelle ausweiten
                     if (theGroundDistribution.data() - dataBeginBackup != 0)
                            dataBegin = theGroundDistribution.data(),                //  // de-DE Zeiger zum Beginn der Verteilung
                                diffBeginAfterRealloc = dataBegin - dataBeginBackup, //  // de-DE Den Abastand zwischen neuen und alten VeKtor
                                dataBeginBackup = dataBegin,                         //  // de-DE Backupzeiger für den Beginn des Vectors (nur bei realloc relevant)
                                dataEnd = dataBegin - 1u,                            //  // de-DE Zeiger vor dem Beginn der Verteilung
                                currentAmount += diffBeginAfterRealloc;              //  // de-DE Zeiger zur Stelle der Verteilung,
                     ++size;                                                         //  // de-DE Die Variable anpassen
              }
              //  // de-DE *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *

              if (specificationFulfilled and inputStreamCount == 0u)
              {
                     *currentAmount += 1;
                     break;
              }

              //  // de-DE + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
              if ((bool)(octet & 0x80u) != inputStatus) //  // de-DE Ist der Status verändert ?
              {
                     if (!(*currentAmount < 0xFFFFFFFFFFFFFFFFull)) //  // de-DE steht einen Variablenüberlauf bevor ?
                            break;                                  //  // de-DE führt zum Abbruch

                     *currentAmount += 1; //  // de-DE Den aktuellen Wert der angezeigten Stelle um ein erhöhen

                     if (earlyStop) //  // de-DE Liegt ein Abbruchzwang vor ?
                            break;  //  // de-DE Die bearbeitung Abbrechen

                     inputStatus = !inputStatus,    //  // de-DE Den Eingabestatus umschalten
                         currentAmount = dataBegin; //  // de-DE Den aktuellen Lauf auf Datenanfang zurücksetzen
              }
              //  // de-DE + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
              else
                     ++currentAmount;
              //  // de-DE + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =

              octet <<= 1u,               //  // de-DE Den Eingabepuffer um eine Stelle nach links verschieben
                  inputStreamCount -= 1u; //  // de-DE Den Zählerindex des Eingabepuffers um eins verringern

              //  // de-DE + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
              if (inputStreamCount == 0u and !specificationFulfilled) //  // de-DE ist der Lauf Null und die Eingabe noch nicht eingelesen ?
              {
                     octet = inputStream.get(),                         //  // de-DE Den Eingabepuffer aus der Eingabe füllen
                         inputStreamCount = inputStream.gcount() << 3u; //  // de-DE Die Anzahl der gelesenen Bits setzen (0 oder 8)

                     if (inputStreamCount == 0u)    //  // de-DE Konnte keinen Wert aus der Eingabe gelesen werden
                            octet = 0u,             //  // de-DE oktet auf null zurückstzen
                                inputStatus = true, //  // de-DE den Eingabestatus auf true (erzwingt der increment der aktuellen Stelle)
                                earlyStop = true;

                     if (specification < inputStreamCount)    //  // de-DE Ist der Eingabepuffer größer oder gleich der Vorgabewert ?
                            inputStreamCount = specification, //  // de-DE Den Eingabepuffer bis auf den Vorgabewert reduzieren
                                specificationFulfilled = true;
                     else                                      //  // de-DE Ist der Eingabepuffer kleiner als der Vorgabewert ?
                            specification -= inputStreamCount; //  // de-DE Den Vorgabewert um die Länge des Eingabepuffer verringern
              }

              //  // de-DE + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + =
       }
       //  // de-DE # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + # + #

       return theGroundDistribution; //  // de-DE Die Grundverteilung zurückgeben
}
