/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <set>
#include "../../header/tools/binToFloat.h"

using namespace std;
/**
 *  this shell script converts binary data into readable integer stream
 */

int main (int argc, char** argv)
{

 // if there is exactly one argument
 if (argc==1)

  // print an helper message on the display
  cerr << "binToInterger <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl 
       << "Wenn der Vorgabewert nicht 64 bit beträgt, wird er auf 32 bit gesetzt" << endl
       << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
       << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl,

  // exit safely
  exit(0); 

 // instantiate the specification
 uint64_t specification=0ULL; 

 if (argc>1){ 

    // get the specification from the argument line 
    specification = strtoull(argv[1],NULL,10); 

    // if the specification remained 0
    if (specification == 0ULL)

      // put an error statement on the screen
      cerr << "The specification shall not be 0. Aborting!" << endl,
      
          // exit with code -1 
          exit(-1);

    // instantiate an array of avialable values
    uint64_t possibleValues[] = {1,2,4,8,16,32,64};

    // instantiate a set of available values
    set<uint64_t> availableValues(possibleValues,possibleValues+7);

    // instantiate an interator
    set<uint64_t>::iterator anIterator = availableValues.find(specification);

    // chek if the iterator is not at the end of available values set
    if (anIterator == availableValues.end())

      // exit with error code
      exit(-1);
	}

 // if there is more than 2 Arguments
 if (argc>2)

  // check if the input stream name is set to "-"
  if (strcmp(argv[2u],"-")!=0)

   // check if the std input can be reopened
   if (freopen(argv[2u],"r",stdin)==NULL)

    // print an error statement on the display
    cerr << "sglk: Die Eingabe " << argv[2u] << " kann nicht geöffnet werden" << endl,

     // exit with error code
    exit(-1);

 // if there is more than 3 arguments
 if (argc>3) 

  // check if the output stream is safely opened 
  if (freopen(argv[3u],"w",stdout)==NULL)
  
    // print an error statement on the display 
   cerr << "sglk: Die Ausgabe " << argv[3u] << " kann nicht geöffnet werden" << endl,

    // exit with error code
   exit(-1); 
   
 // convert from binary to floating point number
 bool ergebnis = binToFloat( cin, specification, cout); 
 
 // close the input
 fclose (stdin),

 // close the output
 fclose (stdout), 

 // exit according to the result of computation
 exit(ergebnis?0u:-1); 
}
