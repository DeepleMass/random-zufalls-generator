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


/**
 * This is a shell wrapper for calling the "ground distribution" generator function.
 * It get an inputstream (default is cin) an provides an output stream (default is cout)
 * the specification is mandatory. It runs in a single thread without parallelizing.
 */
int main(int argc, char **argv)
{

  if (argc == 1u)   {                                                             // Gibt es nur einen Argument?
#ifdef __GERMAN__ // this is the german help code. to be changed with selection of system language
    cerr << "gvlk <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl // Kurze Hilfe ausgeben
         << "Wenn der Vorgabewert 0 beträgt, dann wird er nicht weiter berücksichtigt" << endl
         << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
         << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl;
#else // default is englich
cerr << "gvlk <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl // Kurze Hilfe ausgeben
         << "Wenn der Vorgabewert 0 beträgt, dann wird er nicht weiter berücksichtigt" << endl
         << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
         << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl;
#endif
        exit(0u); // Ende ohne Fehlerkode
  }

    uint64_t specification = strtoull(argv[1u], NULL, 10); // Den Vorgabewert auswerten

    if (argc > 1)
    {
      // Sind es mindestens 2 Argumente
      specification = strtoull(argv[1], NULL, 10); // Vorgabewert aus der Argumentenliste auslesen

      /*if (specification == UINT64_MAX)
        cerr << "The specification shall not be UINT64_MAX! Exiting" << endl,
          exit(-1); */

      if (specification == 0)

        cerr << "The specification shall not be 0! Exiting" << endl,
            exit(-1);
    }

    if (argc > 2)                                                                            // Sind mindestens 3 Argumente Angegeben worden ?
      if (strcmp(argv[2u], "-") != 0u)                                                       // Ist das Argument nicht "-" (Also nicht aus der Konsoleneingabe lesen) ?
        if (freopen(argv[2u], "r", stdin) == NULL)                                           // Konnte die Datei nicht geöffnet werden?
          cerr << "gvlk: Die Eingabe " << argv[2u] << " kann nicht geöffnet werden" << endl, // Eine Fehlermeldung ausgeben
              exit(-1);                                                                      // Ende mit Fehlercode

    vector<uint64_t> theGroundDistribution =       // Einen Vektor für die Grundverteilung anlegen
        groundDistribution(specification, cin); // Die Grundverteilung ausrechnen und abspeichern

    fclose(stdin); // Die Eingabe schließen

    uint64_t cumulationIndex = 0ULL;

    vector<uint64_t> theCumulatedDistribution(theGroundDistribution.size(), 0ULL);

    uint64_t
        *datenBeginn = theCumulatedDistribution.data();

    while (!theGroundDistribution.empty())
    {

      while (0ULL < theGroundDistribution.back())
      {
        for (cumulationIndex = 0u; cumulationIndex < theGroundDistribution.size(); ++cumulationIndex)
          *(datenBeginn + cumulationIndex) += theGroundDistribution.size() - cumulationIndex;
        --theGroundDistribution.back();
      }

      theGroundDistribution.pop_back();
    }

    uint64_t
        size = theCumulatedDistribution.size(),                                            // Die Länge der Grundverteilung anlegen
        coutPadding = (uint64_t)log10(0 < size ? theCumulatedDistribution.front() : 0ULL) + 1; // Den Abstand (nur zur Formatierung) anlegen

    if (argc > 3)                                                                             // Sind mindestens 3 Argumente Angegeben worden ?
      if (freopen(argv[3u], "w", stdout) == NULL){ 
#ifdef __GERMAN__// Konnte die Datei nicht geöffnet werden?
        cerr << "mvvlk: Die Ausgabe " << argv[3u] << " konnte nicht geöffnet werden" << endl; // Eine Fehlermeldung ausgeben
#else
        cerr << "mvvlk: could not open " << argv[3u] <<  " as output stream"<< endl; // Eine Fehlermeldung ausgeben
#endif         
        exit(-1);                                                                         // Fertig mit Fehlercode
}
    for (size_t distributionIndex = 0u; distributionIndex < size; ++distributionIndex) // die Grundverteilung ausgeben
      cout << setfill(' ') << setw(3u) << (distributionIndex + 1u) << " : "
           << setfill(' ') << setw(coutPadding) << theCumulatedDistribution[distributionIndex] << endl;

    fclose(stdout), // Die Ausgabe schließen

        exit(0u); // Ende ohne Fehlerkode
  }
