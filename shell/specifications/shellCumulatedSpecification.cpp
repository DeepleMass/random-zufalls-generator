// g++ -Wall -g -std=c++11 src/grundVorgabe.cpp shell/shellKumulierteVorgabe.cpp -o ~/bin/kvef

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <iomanip>
#include "../../header/specifications/groundspecification.h"

using namespace std;

int main (int argc, char** argv)
{

 if (argc==1) // Gibt es nur einen Argument?
 {
  cerr << "vvlk <Vorgabewert> [/Pfad/zur/Ausgabe]" << endl // Hilfetext ausgeben
       << "Berechnet die kumulierte Vorgabe des angegebenen Vorgabewertes." << endl
       << "Wenn kein Pfad zur Ausgabe angegeben wird, wird es auf stdout geschrieben" << endl;
  exit(0u); // Ende ohne Fehlerkode
 }


 uint64_t vorgabeWert =-1ull; // Den Vorgabewert initialisieren

 if (argc>1) // Sind es mindestens 2 Argumente
  vorgabeWert = strtoull(argv[1],NULL,10); // Den Vorgabewert auslesen

 if (vorgabeWert==0ull) // Ist der Vorgabwert gleich Null
  exit(0u); // Ende ohne Fehlerkode

 vector<uint64_t> vorgabeVerteilung = grundVorgabe(vorgabeWert); // Die Vorgabevereilung berechnen

 if (argc>2) // Sind mindestens 3 Argumente Angegeben worden ?
  if (freopen(argv[2u],"w",stdout)==NULL) // Konnte die Datei nicht geöffnet werden?
  {
   cerr << "vvlk: Die Ausgabe " << argv[2u] << " kann nicht geöffnet werden" << endl; // Eine Fehlermeldung ausgeben
   exit(-1); // Fertig mit Fehlercode
  }

 uint64_t
         size = vorgabeVerteilung.size(), // Die Länge des Verteilungsvector merken
 *datenBeginn = vorgabeVerteilung.data(), // Ein Zeiger auf den Datenbeginn anlegen
   *datenEnde = datenBeginn + size, // Ein Zeiger auf das Datenenden
  *datenREnde = datenBeginn -1u, // Ein Zeiger auf das Daten Reverse Ende anlegen
 *datenZeiger = datenBeginn + 1u, // Ein Zeiger auf der aktuellen Stelle
   datenIndex = 2u,
  *laufZeiger = datenBeginn , // Ein Zeiger auf dem Beginn des Laufs ( Datenzeiger -1u )
      abstand = 2u;

 while (datenZeiger<datenEnde)
 {
  while (datenREnde<laufZeiger)
   *laufZeiger += *datenZeiger * abstand,
       abstand += 1u,
      laufZeiger --;

  datenIndex += 1u,
   laufZeiger = datenZeiger,
  datenZeiger ++,
      abstand =2u;
 }

 abstand = (uint64_t) log10(vorgabeVerteilung.front())+1; // Nur für die Formatierung

 for (uint64_t verteilungszeiger=0u; verteilungszeiger<size; ++verteilungszeiger) // Ausgabe de Vorgabeverteilung und Aufsummierung der Bits
  cout << setfill(' ') << setw(3u) << (verteilungszeiger+1) << " : " // Ausgabe de Vorgabeverteilung
       << setfill(' ') << setw(abstand) << vorgabeVerteilung[verteilungszeiger] << endl;

 fclose (stdout); // Die Ausgabe schließen

 exit(0u); // Ende ohne Fehlerkode
}

