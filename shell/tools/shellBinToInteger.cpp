/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

#include "../../header/tools/binToInteger.h"

using namespace std;

/**
* this script converts a binary file into a series of readable integer
*/
int main (int argc, char** argv)
{

 // if there is only one argument on the command line
 if (argc==1) 
 
 // print an helper statement on the disply
  cerr << "binToInteger <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl
       << "Wenn der Vorgabewert nicht 64 bit beträgt, wird er auf 32 bit gesetzt" << endl
       << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
       << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl,
  
  // exit safely
  exit(0); 
  
  // instantiate the specification 
 uint64_t specification=0ULL; 
 
 // if there is at least 2 Arguments on the command line 
 if (argc>1){

  // get the specification 
  specification=strtoull(argv[1],NULL,10); 

  // if the specification remained 0
  if (specification == 0ULL)

        // put an error statement on the screen
        cerr << "The specification shall not be 0! Exiting" << endl,

            // exit with code -1 
            exit(-1);
}

// set the specification to 32 or 64 (depending on system architecture?)
specification = specification!=64 ? 32 : 64 ;
 
 // if there is more than 2 argument
 if (argc>2) 

 // if the input stream name is not set to " -"
  if (strcmp(argv[2u],"-")!=0) 

  // check if the std in can be reopened
   if (freopen(argv[2u],"r",stdin)==NULL) 

    // give an error statement on the display
    cerr << "sglk: Die Eingabe " << argv[2u] << " kann nicht geöffnet werden" << endl, 

    // exit safely
    exit(-1); 
    
 // if there is more than 3 Arguments
 if (argc>3) 

  // check if the output can be reopened safely
  if (freopen(argv[3u],"w",stdout)==NULL) 
  
  // print an error statement on the display
   cerr << "sglk: Die Ausgabe " << argv[3u] << " kann nicht geöffnet werden" << endl, 

   // exit with error code
   exit(-1); 

   // perform the converion from binary to integer
 bool result =
  binToInteger( cin, specification, cout); 
  
  // close the standart input
 fclose (stdin), 

 // close the standart output
 fclose (stdout), 

 // exit according the result of computation
 exit(result?0u:-1);
}
