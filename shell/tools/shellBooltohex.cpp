/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

// g++ -Wall -g -std=c++11 shell/shellBooltohex.cpp -o ~/bin/booltohex

#include <iostream>
#include <ios>

using namespace std;

int main (int argc, char** argv)
{

 if (argc!=2) return 0; // wenn Argumente fehlen

 cout << "0x"<< uppercase << hex << strtoull(argv[1],NULL,2) << endl; // Formatierte Ausgabe der Eingabe

 exit(0); // Ende ohne Fehlerkode

}
