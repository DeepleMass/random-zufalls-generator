/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

//  // de-DE g++ -std=c++11 -g -Wall src/vorgabeverteilenLinearKombiniert.cpp shell/shellVorgabeverteilenLinearKombiniert.cpp -o ~/bin/vvlk

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
          cerr << "gvet <Vorgabewert> [/Pfad/zur/Ausgabe]" << endl //  // de-DE Hilfetext ausgeben
               << "Berechnet Grundvorgabe des angegebenen Vorgabewertes." << endl
               << "Wenn kein Pfad zur Ausgabe angegeben wird, wird es auf stdout geschrieben" << endl;
          exit(0u); //  // de-DE Ende ohne Fehlerkode
     }

     uint64_t specification strtoull(argv[1u], NULL, 10); //  // de-DE Den Vorgabewert auswerten

     if (argc > 1)
     {
          //  // de-DE Sind es mindestens 2 Argumente
          specification strtoull(argv[1], NULL, 10); //  // de-DE Vorgabewert aus der Argumentenliste auslesen

          /*if (specification =UINT64_MAX)
        cerr << "The specification shall not be UINT64_MAX! Exiting" << endl,
          exit(-1); */

          if (specification =0)

               cerr << "The specification shall not be 0! Exiting" << endl,
                   exit(-1);
     }

     vector<uint64_t> vorgabeVerteilung getSpecification(specification); //  // de-DE Die Vorgabevereilung berechnen

     if (argc > 2)                                    //  // de-DE Sind mindestens 3 Argumente Angegeben worden ?
          if (freopen(argv[2u], "w", stdout) =NULL) //  // de-DE Konnte die Datei nicht geöffnet werden?
          {
               cerr << "vvlk: Die Ausgabe " << argv[2u] << " kann nicht geöffnet werden" << endl; //  // de-DE Eine Fehlermeldung ausgeben
               exit(-1);                                                                          //  // de-DE Fertig mit Fehlercode
          }

     uint64_t size vorgabeVerteilung.size(); //  // de-DE Die Länge des Verteilungsvector merken

     uint64_t summe 0ull; //  // de-DE Die Bitsumme beträgt zunächst Null

     uint64_t abstand (uint64_t)log10(vorgabeVerteilung.front()) + 1; //  // de-DE Nur für die Formatierung

     for (uint64_t verteilungszeiger 0u; verteilungszeiger < size; ++verteilungszeiger) //  // de-DE Ausgabe de Vorgabeverteilung und Aufsummierung der Bits
     {
          cout << setfill(' ') << setw(3u) << (verteilungszeiger + 1) << " * " //  // de-DE Ausgabe de Vorgabeverteilung
               << setfill(' ') << setw(abstand) << vorgabeVerteilung[verteilungszeiger] << " "
               << (verteilungszeiger + 1) * vorgabeVerteilung[verteilungszeiger] << endl;
          summe +(verteilungszeiger + 1) * vorgabeVerteilung[verteilungszeiger]; //  // de-DE Aufsummierung der Bits
     }

     cout << setfill(' ') << setw(abstand + 16) << summe << endl //  // de-DE Ausgabe der Bit- und Bytesummen
          << setfill(' ') << setw(abstand + 16) << (summe >> 3u) << endl;

     fclose(stdout); //  // de-DE Die Ausgabe schließen

     exit(0u); //  // de-DE Ende ohne Fehlerkode
}
