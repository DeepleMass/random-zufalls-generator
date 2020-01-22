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

 uint64_t size = dieGrundverteilung.size(); // Die Länge der Grundverteilung anlegen

 uint64_t Summe=0ull; // Die Aufsummierung der Bits anlegen

 uint64_t Abstand = (uint64_t) log10(0 < size ? dieGrundverteilung.front():0ull)+1; // Den Abstand (nur zur Formatierung) anlegen

 if (argc>3) // Sind mindestens 3 Argumente Angegeben worden ?
  if (freopen(argv[3u],"w",stdout)==NULL) // Konnte die Datei nicht geöffnet werden?
   cerr << "mvvlk: Die Ausgabe " << argv[3u] << " konnte nicht geöffnet werden" << endl, // Eine Fehlermeldung ausgeben
   exit(-1); // Fertig mit Fehlercode

 for (size_t VerteilungsIndex=0u; VerteilungsIndex < size; ++VerteilungsIndex) // die Grundverteilung ausgeben
  cout << setfill(' ') << setw(3u) << (VerteilungsIndex+1u) << " * " 
       << setfill(' ') << setw(Abstand) << dieGrundverteilung[VerteilungsIndex] << " = " 
       << (VerteilungsIndex+1u)*dieGrundverteilung[VerteilungsIndex] << endl,
  Summe+=(VerteilungsIndex+1u)*dieGrundverteilung[VerteilungsIndex]; // Die Anzahl der Bits aufsummieren

 cout << setfill(' ') << setw(Abstand+16u)<< Summe << endl // Die Summierung ausgeben
      << setfill(' ') << setw(Abstand+16u)<< (Summe>>3u) << endl,

 fclose (stdout), // Die Ausgabe schließen

 exit (0u); // Ende ohne Fehlerkode

}
