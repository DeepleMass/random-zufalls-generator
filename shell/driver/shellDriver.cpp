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
#include <cstdint>

#include "../../header/driver/driver.h"

using namespace std;
/**
 * this shell script implement the use of the core driver
 * It supplies an ending bit stream out of an input stream and a specification
 * The lowest the entropy of input stream, the longest it has to be to achieve
 * the random bit generation see the help for a description of arguments 
 */
int main(int argc, char **argv)
{
     // if there is only one argument 
     if (argc ==1){
          // put a help message on the display
          cerr << "etef <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl
               << "Wenn der Vorgabewert (in bits)0 beträgt, bricht die Verarbeitung ab" << endl
               << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
               << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl;

              exit(0); // exit safely
     }
     uint64_t specification = 0ULL; // instantiate the specification

     // if there is more than one argument
     if (argc > 1)
     {   
          // get the specification out of the argument list 
          specification = strtoull(argv[1], NULL, 10); 

          if (specification ==0)// check if it gone well or if 0 has been passed as argument

               // if something went wrong                                                 
               cerr << "The specification shall not be 0! Exiting" << endl;
                    // exit with status -1
     }

     // if there is more than 2 arguments
     if (argc > 2)

     //  if the second argument is "-" so we get the values from the std input
          if (strcmp(argv[2u], "-") !=0)

               // if the std in could not be reopened 
               if (freopen(argv[2u], "r", stdin) ==NULL)

                    // give an error on the display
                    cerr << "sglk: Die Eingabe " << argv[2u] << " kann nicht geöffnet werden" << endl,

                         // exit with error
                        exit(-1);                                                                      

     // if there is more than 3 arguments
     if (argc > 3)   

          // if the std in could not be reopened
          if (freopen(argv[3u], "w", stdout) ==NULL)
          
               // put an error message on the display 
               cerr << "sglk: Die Ausgabe " << argv[3u] << " kann nicht geöffnet werden" << endl,

               //  exit properly
               exit(-1);                                                                      

     bool result =
          // call the driver
         driver(cin, specification, cout); 

     // close the input
     fclose(stdin),

          // close the output
         fclose(stdout),

          // exit with appropriate code
         exit(result ? 0u : -1);
}
