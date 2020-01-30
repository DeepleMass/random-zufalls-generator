/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

//  // de-DE g++ -Wall -g -std=c++11 shell/shellDectohex.cpp -o ~/bin/dectohex

#include <iostream>
#include <ios>

using namespace std;
/**
 * this script converts a series of decimal number into its hexadecimal conterpart
 */ 
int main (int argc, char** argv)
{

 if (argc!=2) return 0; //  // de-DE wenn Argumente fehlen

 cout <<"0x"<< uppercase << hex <<strtoull(argv[1],NULL,10) << endl; //  // de-DE Formatierte Ausgabe der Eingabe

 exit(0); //  // de-DE Ende ohne Fehlerkode

}
