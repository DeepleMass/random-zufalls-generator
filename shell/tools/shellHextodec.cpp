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

// if the arguments number is not exactly 2
 if (argc!=2) return 0;

 // formated output 
 cout << strtoull(argv[1],NULL,16) << endl;

 // safe exit
 exit(0); 

}
