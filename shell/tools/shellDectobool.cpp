/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

//  // de-DE g++ -Wall -g -std=c++11 shell/shellDectobool.cpp -o ~/bin/dectobool

#include <iostream>
#include <vector>


using namespace std;
/**
 * this script convert a serie of decimal number in a series of boolean
 */
int main (int argc, char** argv)
{

 if (argc!=2) //  // de-DE Sind es mehr als Ein Argument?
  exit (0); //  // de-DE wenn Argumente fehlen

 uint64_t input = strtoull(argv[1],NULL,10); //  // de-DE Den Vorgabewert auslesen

 if (input==0ull) //  // de-DE Ist die Eingabe gleich Null?
 {
  cout << 0<< endl; //  // de-DE Eine 0 Ausgeben
  exit(0); //  // de-DE Ende ohne Fehlerkode
 }

 vector<bool> aVector; //  // de-DE Den Vector für die boolschen Werte anlegen

 while (input!=0ull) //  // de-DE Solange die Eingabe nicht Null ist
 {
  aVector.push_back(input & 0x1ull?true:false); //  // de-DE Den Vektor um einen Element ausweiten
  input>>=1; //  // de-DE Die Eingabe um eine Stelle nach rechts verschieben
 }

 while (!aVector.empty()) //  // de-DE Solange der Vector nich leer ist
 {
  cout << aVector.back (); //  // de-DE Formatierte Ausgabe der Eingabe
  aVector.pop_back (); //  // de-DE Letztes Element im Vector ausgeben.
 }

 cout << endl; //  // de-DE Zeilenvorschub

 exit(0); //  // de-DE Ende ohne Fehlerkode

}
