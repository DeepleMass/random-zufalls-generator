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

 // if there is not exactly 2 arguments
 if (argc!=2) return 0; 

 // print the argument on the display in hex format
 cout << "0x"<< uppercase << hex << strtoull(argv[1],NULL,2) << endl; 
 
 // exit safely 
 exit(0); 
}
