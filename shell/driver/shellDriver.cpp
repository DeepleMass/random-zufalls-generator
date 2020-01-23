/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

// g++ -std=c++11 -g -Wall src/treibercpp src/vorgabeverteilenLinearKombiniert.cpp shell/shellTeiber.cpp -Iheader -o ~/bin/etef

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

#include "../../header/driver/driver.h"

using namespace std;

int main (int argc, char** argv)
{

 if (argc==1) // Gibt es nur einen Argument?
  cerr << "etef <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl // Kurze Hilfe ausgeben
       << "Wenn der Vorgabewert (in bits)0 beträgt, bricht die Verarbeitung ab" << endl
       << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
       << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl,
  exit(0); // Fertig

 uint64_t vorgabeWert=-1ull; // Initialisiere den Vorgabewert auf -1ull

 if (argc>1) // Sind es mindestens 2 Argumente
  vorgabeWert=strtoull(argv[1],NULL,10); // Vorgabewert aus der Argumentenliste auslesen
 
 if (vorgabeWert==0) // Ist der Vorgabewert gleich Null
  exit (-1); // Ende mit Fehlerkode

 if (argc>2) // Sind mindestens 3 Argumente Angegeben worden ?
  if (strcmp(argv[2u],"-")!=0) // Ist das Argument nicht "-" (Also nicht aus der Konsoleneingabe lesen) ?
   if (freopen(argv[2u],"r",stdin)==NULL) // Konnte die Datei nicht geöffnet werden?
    cerr << "sglk: Die Eingabe " << argv[2u] << " kann nicht geöffnet werden" << endl, // Eine Fehlermeldung ausgeben
    exit(-1); // Fertig mit Fehlercode

 if (argc>3) // Sind mindestens 3 Argumente Angegeben worden ?
  if (freopen(argv[3u],"w",stdout)==NULL) // Konnte die Datei nicht geöffnet werden?
   cerr << "sglk: Die Ausgabe " << argv[3u] << " kann nicht geöffnet werden" << endl, // Eine Fehlermeldung ausgeben
   exit(-1); // Fertig mit Fehlercode

 bool ergebnis =
  treiber( cin, vorgabeWert, cout); // den sequenziellen Generator aufrufen

 fclose (stdin), // Eingabe schließen

 fclose (stdout), // Ausgabe schließen

 exit(ergebnis?0u:-1); // Ausgabe entsprechend der Ausgabe des Generators

}
