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
 * this script converts a series of hexadecimal into its decimal conterpart
 */
int main (int argc, char** argv)
{

 if (argc!=2) return 0; //  // de-DE wenn Argumente fehlen

 cout << strtoull(argv[1],NULL,16) << endl; //  // de-DE Formatierte Ausgabe der Eingabe

 exit(0); //  // de-DE Ende ohne Fehlerkode

}
