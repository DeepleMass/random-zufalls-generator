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
#include <set>
#include "../../header/tools/binToFloat.h"

using namespace std;
/**
 *  this shell script converts binary data into readable integer stream
 */

int main (int argc, char** argv)
{

 if (argc==1) //  // de-DE Gibt es nur einen Argument?
  cerr << "binToInterger <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl //  // de-DE Kurze Hilfe ausgeben
       << "Wenn der Vorgabewert nicht 64 bit beträgt, wird er auf 32 bit gesetzt" << endl
       << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
       << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl,
  exit(0); //  // de-DE Fertig

 uint64_t vorgabeWert=-1ull; //  // de-DE Initialisiere den Vorgabewert auf -1ull

 if (argc>1){ //  // de-DE Sind es mindestens 2 Argumente
  vorgabeWert = strtoull(argv[1],NULL,10); //  // de-DE Vorgabewert aus der Argumentenliste auslesen

  uint64_t werteVorrat[] = {1,2,4,8,16,32,64};
	set<uint64_t> gueltigeWerte(werteVorrat,werteVorrat+7);

	set<uint64_t>::iterator passender = gueltigeWerte.find(vorgabeWert);
	if (passender == gueltigeWerte.end())
		exit(-1);
	}
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

 bool ergebnis = binToFloat( cin, vorgabeWert, cout); //  // de-DE den sequenziellen Generator aufrufen

/*
 fclose (stdin), //  // de-DE Eingabe schließen

 fclose (stdout), //  // de-DE Ausgabe schließen
*/
 exit(ergebnis?0u:-1); //  // de-DE Ausgabe entsprechend der Ausgabe des Generators

}
