/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <bitset>
#include <iostream>
#include <cstdint>
#include <cstring>

using namespace std;
/**
 * this shell convert binary data in 
 * readable 0/1 format of 64 boolean prer line
 */

int main(int argc, char **argv)
{

  if (argc == 1) //  // de-DE Gibt es nur einen Argument?
  {
   
    cerr << "bintobool [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl //  // de-DE Kurze Hilfe ausgeben
         << "Einfacher Bindump für eine belibiege Datei. Schreibt 64 Bit pro Zeile." << endl
         << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
         << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl;



    exit(0); //  // de-DE Fertig
  }

  if (argc > 1)                                  //  // de-DE Sind mindestens 3 Argumente Angegeben worden ?
    if (strcmp(argv[1u], "-") != 0)              //  // de-DE Ist das Argument nicht "-" (Also nicht aus der Konsoleneingabe lesen) ?
      if (freopen(argv[1u], "r", stdin) == NULL) //  // de-DE Konnte die Datei nicht geöffnet werden?
      { 
        cerr << "sglk: Die Eingabe " << argv[1u] << " kann nicht geöffnet werden" << endl; //  // de-DE Eine Fehlermeldung ausgeben
        exit(-1);                                                                          //  // de-DE Fertig mit Fehlercode
      }

  if (argc > 2)                                 //  // de-DE Sind mindestens 3 Argumente Angegeben worden ?
    if (freopen(argv[2u], "w", stdout) == NULL) //  // de-DE Konnte die Datei nicht geöffnet werden?
    {

      cerr << "sglk: Die Ausgabe " << argv[2u] << " kann nicht geöffnet werden" << endl; //  // de-DE Eine Fehlermeldung ausgeben
      exit(-1);                                                                          //  // de-DE Fertig mit Fehlercode
    }

  uint8_t
      input = 0, //  // de-DE Byte für die Eingabe
      ensemble = 0;   //  // de-DE Zähler für den Zeilenumbruch

  while (cin.read((char *)&input, 1u)) //  // de-DE Solange gelesen wird
  {
    cout << bitset<8>(input); //  // de-DE Den Bitset als Folge von 0 und 1 Ausgeben
    ensemble += 8;                 //  // de-DE Die Menge der gelesenen Bits aktualisieren
    if (ensemble == 64)            //  // de-DE sind 64 Bits auf einem Stück ?
    {
      cout << endl; //  // de-DE Zeilenumbruch veranlassen
      ensemble = 0;    //  // de-DE Zähler zurücksetzen
    }
  }

  if (ensemble > 0)  //  // de-DE sind keine 64 Bits auf einem Stück ?
    cout << endl; //  // de-DE Ein Zeilenrücklauf am Ende der Ausgabe

  fclose(stdout); //  // de-DE Ausgabe schließen

  fclose(stdin); //  // de-DE Eingabe schließen

  exit(0u); //  // de-DE Fertig ohne Fehlerkode
}
