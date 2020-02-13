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
 * this script convert a binary innput into a series of decimal number
 */
int main (int argc, char** argv)
{

// if there is no argument
 if (argc!=2) return 0; 
 
 // print the ouptut on the display
 cout << strtoull(argv[1],NULL,2) << endl; 
 
 // exit safely
 exit(0); 

}
