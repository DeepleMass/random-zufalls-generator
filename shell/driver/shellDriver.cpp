/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include "../../header/driver/driver.h"

using namespace std;

int main(int argc, char **argv)
{

     if (argc == 1)                                                                    // Gibt es nur einen Argument?
          cerr << "etef <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl // Kurze Hilfe ausgeben
               << "Wenn der Vorgabewert (in bits)0 beträgt, bricht die Verarbeitung ab" << endl
               << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
               << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl,
              exit(0); // Fertig

     uint64_t specification = UINT64_MAX; // Initialisiere den Vorgabewert auf -1ull

     if (argc > 1)
     {                                                 // Sind es mindestens 2 Argumente
          specification = strtoull(argv[1], NULL, 10); // Vorgabewert aus der Argumentenliste auslesen
           /*if (specification == UINT64_MAX)

               cerr << "The specification shall not be UINT64_MAX! Exiting" << endl,
                   exit(-1); */

          if (specification == 0)

               cerr << "The specification shall not be 0! Exiting" << endl,
                   // Ist der Vorgabewert gleich Null
                   exit(-1); // Ende mit Fehlerkode
     }

     if (argc > 2)                                                                                     // Sind mindestens 3 Argumente Angegeben worden ?
          if (strcmp(argv[2u], "-") != 0)                                                              // Ist das Argument nicht "-" (Also nicht aus der Konsoleneingabe lesen) ?
               if (freopen(argv[2u], "r", stdin) == NULL)                                              // Konnte die Datei nicht geöffnet werden?
                    cerr << "sglk: Die Eingabe " << argv[2u] << " kann nicht geöffnet werden" << endl, // Eine Fehlermeldung ausgeben
                        exit(-1);                                                                      // Fertig mit Fehlercode

     if (argc > 3)                                                                                // Sind mindestens 3 Argumente Angegeben worden ?
          if (freopen(argv[3u], "w", stdout) == NULL)                                             // Konnte die Datei nicht geöffnet werden?
               cerr << "sglk: Die Ausgabe " << argv[3u] << " kann nicht geöffnet werden" << endl, // Eine Fehlermeldung ausgeben
                   exit(-1);                                                                      // Fertig mit Fehlercode

     bool result =
         driver(cin, specification, cout); // den sequenziellen Generator aufrufen

     fclose(stdin), // Eingabe schließen

         fclose(stdout), // Ausgabe schließen

         exit(result ? 0u : -1); // Ausgabe entsprechend der Ausgabe des Generators
}
