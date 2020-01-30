/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <iostream>
#include <vector>

using namespace std;

/**
 * This script converts a series of hexadecimal number in its boolean conterpart
 */
int main (int argc, char** argv)
{

 if (argc!=2) //  // de-DE Sind es mehr als Ein Argument?
  exit (0); //  // de-DE wenn Argumente fehlen

 uint64_t eingabe = strtoull(argv[1],NULL,16); //  // de-DE Den Vorgabewert auslesen

 if (eingabe==0ull) //  // de-DE Ist die Eingabe gleich Null?
 {
  cout << 0<< endl; //  // de-DE Eine 0 Ausgeben
  exit(0); //  // de-DE Ende ohne Fehlerkode
 }

 vector<bool> einVector; //  // de-DE Den Vector für die boolschen Werte anlegen

 while (eingabe!=0ull) //  // de-DE Solange die Eingabe nicht Null ist
 {
  einVector.push_back(eingabe & 0x1ull?true:false); //  // de-DE Den Vektor um einen Element ausweiten
  eingabe>>=1; //  // de-DE Die Eingabe um eine Stelle nach rechts verschieben
 }

 while (!einVector.empty()) //  // de-DE Solange der Vector nich leer ist
 {
  cout << einVector.back (); //  // de-DE Formatierte Ausgabe der Eingabe
  einVector.pop_back (); //  // de-DE Letztes Element im Vector ausgeben.
 }

 cout << endl; //  // de-DE Zeilenvorschub

 exit(0); //  // de-DE Ende ohne Fehlerkode

}
