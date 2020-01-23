/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

// g++ -Wall -std=c++11 -g src/grundverteilung.cpp shell/shellGrundverteilung.cpp -o ~/bin/tgvlk

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "../../header/evaluation/grounddistribution.h"

using namespace std;

int main (int argc, char** argv)
{

 if (argc==1u) // Gibt es nur einen Argument?
  cerr << "gvlk <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl // Kurze Hilfe ausgeben
       << "Wenn der Vorgabewert 0 beträgt, dann wird er nicht weiter berücksichtigt" << endl
       << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
       << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl,
  exit(0u); // Ende ohne Fehlerkode

 uint64_t vorgabeWert = strtoull(argv[1u],NULL,10); // Den Vorgabewert auswerten

 vorgabeWert = vorgabeWert==0ull?-1ull:vorgabeWert; // Ist er gleich null, dann ist er gleich maximal

 if (argc > 2) // Sind mindestens 3 Argumente Angegeben worden ?
  if (strcmp(argv[2u],"-") != 0u) // Ist das Argument nicht "-" (Also nicht aus der Konsoleneingabe lesen) ?
   if (freopen(argv[2u],"r",stdin) == NULL) // Konnte die Datei nicht geöffnet werden?
    cerr << "gvlk: Die Eingabe " << argv[2u] << " kann nicht geöffnet werden" << endl, // Eine Fehlermeldung ausgeben
    exit(-1); // Ende mit Fehlercode

 vector<uint64_t> dieGrundverteilung = // Einen Vektor für die Grundverteilung anlegen
  grundverteilung(vorgabeWert,cin); // Die Grundverteilung ausrechnen und abspeichern

 fclose(stdin); // Die Eingabe schließen

 uint64_t kumulationsIndex = 0u;

 vector<uint64_t> dieKumulierteVerteilung (dieGrundverteilung.size(),0ull);

 uint64_t 
  *datenBeginn = dieKumulierteVerteilung.data();
 while (! dieGrundverteilung.empty())
 {
  while (0ull < dieGrundverteilung.back())
  {
   for (kumulationsIndex = 0u; kumulationsIndex < dieGrundverteilung.size(); ++ kumulationsIndex)
     *(datenBeginn + kumulationsIndex)+= dieGrundverteilung.size()-kumulationsIndex;
   -- dieGrundverteilung.back();
  }
  dieGrundverteilung.pop_back();
 }

 uint64_t 
  size = dieKumulierteVerteilung.size(), // Die Länge der Grundverteilung anlegen
  Abstand = (uint64_t) log10(0 < size ? dieKumulierteVerteilung.front():0ull)+1; // Den Abstand (nur zur Formatierung) anlegen

 if (argc>3) // Sind mindestens 3 Argumente Angegeben worden ?
  if (freopen(argv[3u],"w",stdout)==NULL) // Konnte die Datei nicht geöffnet werden?
   cerr << "mvvlk: Die Ausgabe " << argv[3u] << " konnte nicht geöffnet werden" << endl, // Eine Fehlermeldung ausgeben
   exit(-1); // Fertig mit Fehlercode

 for (size_t VerteilungsIndex=0u; VerteilungsIndex < size; ++VerteilungsIndex) // die Grundverteilung ausgeben
  cout << setfill(' ') << setw(3u) << (VerteilungsIndex+1u) << " : " 
       << setfill(' ') << setw(Abstand) << dieKumulierteVerteilung[VerteilungsIndex] << endl;

 fclose (stdout), // Die Ausgabe schließen

 exit (0u); // Ende ohne Fehlerkode

}
