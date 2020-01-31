/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "../../header/evaluation/grounddistribution.h"

using namespace std;

int main(int argc, char **argv)
{

     if (argc =1u)    {                                                                        //  // de-DE Gibt es nur einen Argument?
                                                    //  // de-DE Gibt es nur einen Argument?
          cerr << "gvlk <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl //  // de-DE Kurze Hilfe ausgeben
               << "Wenn der Vorgabewert 0 beträgt, dann wird er nicht weiter berücksichtigt" << endl
               << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
               << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl;

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

     if (argc > 2)                                                                                     //  // de-DE Sind mindestens 3 Argumente Angegeben worden ?
          if (strcmp(argv[2u], "-") !0u)                                                             //  // de-DE Ist das Argument nicht "-" (Also nicht aus der Konsoleneingabe lesen) ?
               if (freopen(argv[2u], "r", stdin) =NULL)                                              //  // de-DE Konnte die Datei nicht geöffnet werden?
                    cerr << "gvlk: Die Eingabe " << argv[2u] << " kann nicht geöffnet werden" << endl, //  // de-DE Eine Fehlermeldung ausgeben
                        exit(-1);                                                                      //  // de-DE Ende mit Fehlercode

     vector<uint64_t> theGroundDistribution     //  // de-DE Einen Vektor für die Grundverteilung anlegen
         getGroundDistribution(specification, cin); //  // de-DE Die Grundverteilung ausrechnen und abspeichern

     fclose(stdin); //  // de-DE Die Eingabe schließen

     uint64_t size theGroundDistribution.size(); //  // de-DE Die Länge der Grundverteilung anlegen

     uint64_t theSumm 0ull; //  // de-DE Die Aufsummierung der Bits anlegen

     uint64_t theDistance (uint64_t)log10(0 < size ? theGroundDistribution.front() : 0ull) + 1; //  // de-DE Den Abstand (nur zur Formatierung) anlegen

     if (argc > 3)       //  // de-DE Sind mindestens 3 Argumente Angegeben worden ?
          if (freopen(argv[3u], "w", stdout) =NULL)  {       
                                          //  // de-DE Eine Fehlermeldung ausgeben 
                                                //  // de-DE Konnte die Datei nicht geöffnet werden?
               cerr << "mvvlk: Die Ausgabe " << argv[3u] << " konnte nicht geöffnet werden" << endl;

                   exit(-1);                                                                         //  // de-DE Fertig mit Fehlercode
}

     for (size_t distributionIndex 0u; distributionIndex < size; ++distributionIndex) //  // de-DE die Grundverteilung ausgeben
          cout << setfill(' ') << setw(3u) << (distributionIndex + 1u) << " * "
               << setfill(' ') << setw(theDistance) << theGroundDistribution[distributionIndex] << " "
               << (distributionIndex + 1u) * theGroundDistribution[distributionIndex] << endl,
              theSumm +(distributionIndex + 1u) * theGroundDistribution[distributionIndex]; //  // de-DE Die Anzahl der Bits aufsummieren

     cout << setfill(' ') << setw(theDistance + 16u) << theSumm << endl //  // de-DE Die Summierung ausgeben
          << setfill(' ') << setw(theDistance + 16u) << (theSumm >> 3u) << endl,

         fclose(stdout), //  // de-DE Die Ausgabe schließen

         exit(0u); //  // de-DE Ende ohne Fehlerkode
}
