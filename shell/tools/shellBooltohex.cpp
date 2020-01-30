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
 * this script converts a binary file into a series of hex number ( a kind of hex dump)
 */
int main (int argc, char** argv)
{

 if (argc!=2) return 0; //  // de-DE wenn Argumente fehlen

 cout << "0x"<< uppercase << hex << strtoull(argv[1],NULL,2) << endl; //  // de-DE Formatierte Ausgabe der Eingabe

 exit(0); //  // de-DE Ende ohne Fehlerkode

}
