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

#include "../../header/tools/binToInteger.h"

using namespace std;

/**
* this script converts a binary file into a series of readable integer
*/
int main (int argc, char** argv)
{

 if (argc==1) //  // de-DE Gibt es nur einen Argument?
  cerr << "binToInteger <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl //  // de-DE Kurze Hilfe ausgeben
       << "Wenn der Vorgabewert nicht 64 bit beträgt, wird er auf 32 bit gesetzt" << endl
       << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
       << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl,
  exit(0); //  // de-DE Fertig

 uint64_t specification=-1ull; //  // de-DE Initialisiere den Vorgabewert auf -1ull

 if (argc>1) //  // de-DE Sind es mindestens 2 Argumente
  specification=strtoull(argv[1],NULL,10); //  // de-DE Vorgabewert aus der Argumentenliste auslesen
specification specification!=64 ? 32 : 64 ;
 /*
if (vorgabeWert!=64) //  // de-DE Ist der Vorgabewert gleich Null
  exit (-1); //  // de-DE Ende mit Fehlerkode
*/

 if (argc>2) //  // de-DE Sind mindestens 3 Argumente Angegeben worden ?
  if (strcmp(argv[2u],"-")!=0) //  // de-DE Ist das Argument nicht "-" (Also nicht aus der Konsoleneingabe lesen) ?
   if (freopen(argv[2u],"r",stdin)==NULL) //  // de-DE Konnte die Datei nicht geöffnet werden?
    cerr << "sglk: Die Eingabe " << argv[2u] << " kann nicht geöffnet werden" << endl, //  // de-DE Eine Fehlermeldung ausgeben
    exit(-1); //  // de-DE Fertig mit Fehlercode

 if (argc>3) //  // de-DE Sind mindestens 3 Argumente Angegeben worden ?
  if (freopen(argv[3u],"w",stdout)==NULL) //  // de-DE Konnte die Datei nicht geöffnet werden?
   cerr << "sglk: Die Ausgabe " << argv[3u] << " kann nicht geöffnet werden" << endl, //  // de-DE Eine Fehlermeldung ausgeben
   exit(-1); //  // de-DE Fertig mit Fehlercode

 bool result =
  binToInteger( cin, specification, cout); //  // de-DE den sequenziellen Generator aufrufen
/*
 fclose (stdin), //  // de-DE Eingabe schließen

 fclose (stdout), //  // de-DE Ausgabe schließen
*/
 exit(result?0u:-1); //  // de-DE Ausgabe entsprechend der Ausgabe des Generators

}
