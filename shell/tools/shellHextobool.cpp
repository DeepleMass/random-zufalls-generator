// g++ -Wall -g -std=c++11 shell/shellHextobool.cpp -o ~/bin/hextobool

#include <iostream>
#include <vector>


using namespace std;

int main (int argc, char** argv)
{

 if (argc!=2) // Sind es mehr als Ein Argument?
  exit (0); // wenn Argumente fehlen

 uint64_t eingabe = strtoull(argv[1],NULL,16); // Den Vorgabewert auslesen

 if (eingabe==0ull) // Ist die Eingabe gleich Null?
 {
  cout << 0<< endl; // Eine 0 Ausgeben
  exit(0); // Ende ohne Fehlerkode
 }

 vector<bool> einVector; // Den Vector für die boolschen Werte anlegen

 while (eingabe!=0ull) // Solange die Eingabe nicht Null ist
 {
  einVector.push_back(eingabe & 0x1ull?true:false); // Den Vektor um einen Element ausweiten
  eingabe>>=1; // Die Eingabe um eine Stelle nach rechts verschieben
 }

 while (!einVector.empty()) // Solange der Vector nich leer ist
 {
  cout << einVector.back (); // Formatierte Ausgabe der Eingabe
  einVector.pop_back (); // Letztes Element im Vector ausgeben.
 }

 cout << endl; // Zeilenvorschub

 exit(0); // Ende ohne Fehlerkode

}
