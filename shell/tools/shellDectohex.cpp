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
 * this script converts a series of decimal number into its hexadecimal conterpart
 */ 
int main (int argc, char** argv)
{

 // if there is not exactly two arguments
 if (argc!=2) exit(-1); 

 // print the argument as hexa number on the display
 cout <<"0x"<< uppercase << hex <<strtoull(argv[1],NULL,10) << endl; 
 
 // exit safely
 exit(0); 
}
