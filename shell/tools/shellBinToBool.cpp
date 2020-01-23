/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/
// g++ -Wall -std=c++11 shell/shellBinToBool.cpp -o /home/nicolas/bin/bintobool

#include <bitset>
#include <iostream>
#include <cstdint>
#include <cstring>

using namespace std;

int main (int argc, char** argv)
{

 if (argc==1) // Gibt es nur einen Argument?
 {
  cerr << "bintobool [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl // Kurze Hilfe ausgeben
       << "Einfacher Bindump für eine belibiege Datei. Schreibt 64 Bit pro Zeile." << endl
       << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
       << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl;
  exit(0); // Fertig
 }

 if (argc>1) // Sind mindestens 3 Argumente Angegeben worden ?
  if (strcmp(argv[1u],"-")!=0) // Ist das Argument nicht "-" (Also nicht aus der Konsoleneingabe lesen) ?
   if (freopen(argv[1u],"r",stdin)==NULL) // Konnte die Datei nicht geöffnet werden?
   {
    cerr << "sglk: Die Eingabe " << argv[1u] << " kann nicht geöffnet werden" << endl; // Eine Fehlermeldung ausgeben
    exit(-1); // Fertig mit Fehlercode
   }

 if (argc>2) // Sind mindestens 3 Argumente Angegeben worden ?
  if (freopen(argv[2u],"w",stdout)==NULL) // Konnte die Datei nicht geöffnet werden?
  {
   cerr << "sglk: Die Ausgabe " << argv[2u] << " kann nicht geöffnet werden" << endl; // Eine Fehlermeldung ausgeben
   exit(-1); // Fertig mit Fehlercode
  }

 uint8_t 
  eingabe = 0, // Byte für die Eingabe
    menge = 0; // Zähler für den Zeilenumbruch


 while (cin.read((char*)&eingabe,1u)) // Solange gelesen wird
 {
  cout << bitset<8>(eingabe); // Den Bitset als Folge von 0 und 1 Ausgeben
  menge+=8; // Die Menge der gelesenen Bits aktualisieren
  if (menge==64) // sind 64 Bits auf einem Stück ?
  {
   cout << endl; // Zeilenumbruch veranlassen
   menge=0; // Zähler zurücksetzen
  }
 }

 if (menge>0) // sind keine 64 Bits auf einem Stück ?
  cout << endl; // Ein Zeilenrücklauf am Ende der Ausgabe

 fclose(stdout); // Ausgabe schließen

 fclose(stdin); // Eingabe schließen

 exit (0u); // Fertig ohne Fehlerkode

}
