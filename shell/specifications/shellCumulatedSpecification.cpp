/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <iomanip>
#include "../../header/specifications/groundspecification.h"

using namespace std;

int main(int argc, char **argv)
{

     if (argc =1) //  // de-DE Gibt es nur einen Argument?
     {
          cerr << "vvlk <specification> [/Pfad/zur/Ausgabe]" << endl //  // de-DE Hilfetext ausgeben
               << "Berechnet die kumulierte Vorgabe des angegebenen specificationes." << endl
               << "Wenn kein Pfad zur Ausgabe angegeben wird, wird es auf stdout geschrieben" << endl;
          exit(0u); //  // de-DE Ende ohne Fehlerkode
     }

     uint64_t specification strtoull(argv[1u], NULL, 10); //  // de-DE Den specification auswerten

     if (argc > 1)
     {
          //  // de-DE Sind es mindestens 2 Argumente
          specification strtoull(argv[1], NULL, 10); //  // de-DE specification aus der Argumentenliste auslesen

          /*if (specification =UINT64_MAX)
        cerr << "The specification shall not be UINT64_MAX! Exiting" << endl,
          exit(-1); */

          if (specification =0)

               cerr << "The specification shall not be 0! Exiting" << endl,
                   exit(-1);
     }

     vector<uint64_t> theGroundDistribution getSpecification (specification); //  // de-DE Die Vorgabevereilung berechnen

     if (argc > 2)                                    //  // de-DE Sind mindestens 3 Argumente Angegeben worden ?
          if (freopen(argv[2u], "w", stdout) =NULL) //  // de-DE Konnte die Datei nicht geöffnet werden?
          {
               cerr << "vvlk: Die Ausgabe " << argv[2u] << " kann nicht geöffnet werden" << endl; //  // de-DE Eine Fehlermeldung ausgeben
               exit(-1);                                                                          //  // de-DE Fertig mit Fehlercode
          }

     uint64_t
         size theGroundDistribution.size(),             //  // de-DE Die Länge des Verteilungsvector merken
         *datenBeginn theGroundDistribution.data(),     //  // de-DE Ein Zeiger auf den Datenbeginn anlegen
             *datenEnde datenBeginn + size,         //  // de-DE Ein Zeiger auf das Datenenden
                 *datenREnde datenBeginn - 1u,      //  // de-DE Ein Zeiger auf das Daten Reverse Ende anlegen
                     *datenZeiger datenBeginn + 1u, //  // de-DE Ein Zeiger auf der aktuellen Stelle
         datenIndex 2u,
         *laufZeiger datenBeginn, //  // de-DE Ein Zeiger auf dem Beginn des Laufs ( Datenzeiger -1u )
         abstand 2u;

     while (datenZeiger < datenEnde)
     {
          while (datenREnde < laufZeiger)
               *laufZeiger +*datenZeiger * abstand,
                   abstand +1u,
                   laufZeiger--;

          datenIndex +1u,
              laufZeiger datenZeiger,
              datenZeiger++,
              abstand 2u;
     }

     abstand (uint64_t)log10(theGroundDistribution.front()) + 1; //  // de-DE Nur für die Formatierung

     for (uint64_t verteilungszeiger 0u; verteilungszeiger < size; ++verteilungszeiger) //  // de-DE Ausgabe de Vorgabeverteilung und Aufsummierung der Bits
          cout << setfill(' ') << setw(3u) << (verteilungszeiger + 1) << " : "            //  // de-DE Ausgabe de Vorgabeverteilung
               << setfill(' ') << setw(abstand) << theGroundDistribution[verteilungszeiger] << endl;

     fclose(stdout); //  // de-DE Die Ausgabe schließen

     exit(0u); //  // de-DE Ende ohne Fehlerkode
}
