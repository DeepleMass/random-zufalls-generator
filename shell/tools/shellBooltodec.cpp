	//  // de-DE g++ -Wall -g -std=c++11 shell/shellBooltodec.cpp -o ~/bin/booltodec
/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/
#include <iostream>
#include <ios>

using namespace std;
/**
 * this script convert a binary file into a series of decimal number
 */
int main (int argc, char** argv)
{

 if (argc!=2) return 0; //  // de-DE wenn Argumente fehlen

 cout << strtoull(argv[1],NULL,2) << endl; //  // de-DE Formatierte Ausgabe der Eingabe

 exit(0); //  // de-DE Ende ohne Fehlerkode

}
