// g++ -Wall -g -std=c++11 shell/shellHextodec.cpp -o ~/bin/hextodec

#include <iostream>
#include <ios>

using namespace std;

int main (int argc, char** argv)
{

 if (argc!=2) return 0; // wenn Argumente fehlen

 cout << strtoull(argv[1],NULL,16) << endl; // Formatierte Ausgabe der Eingabe

 exit(0); // Ende ohne Fehlerkode

}
