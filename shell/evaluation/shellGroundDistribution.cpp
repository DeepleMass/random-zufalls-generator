/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "../../header/evaluation/grounddistribution.h"

using namespace std;

int main(int argc, char **argv)
{
     // if there is only one argument
     if (argc ==1u)    {
          // print a help statement
          cerr << "gvlk <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl
               << "Wenn der Vorgabewert 0 beträgt, dann wird er nicht weiter berücksichtigt" << endl
               << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
               << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl;

              exit(0u); // just exit 
}
     //  instantiate the specification
     uint64_t specification =0ULL;

     // if there is at least two arguments
     if (argc > 1)
     {
          // read the specification from the command line 
          specification = strtoull(argv[1], NULL, 10);
          
          // if the specifiaction has been set to 0
          if (specification ==0ULL)

               cerr << "The specification shall not be 0! Exiting" << endl,
                   exit(-1);
     }
     // if there is at least 2 arguments
     if (argc > 2)

          // if the input stream is " -"
          if (strcmp(argv[2u], "-") !=0u)

               // we reopen the inputstream to be std input
               if (freopen(argv[2u], "r", stdin) ==NULL)

                    // if this fails
                    cerr << "gvlk: Die Eingabe " << argv[2u] << " kann nicht geöffnet werden" << endl,

                        // exit with error code
                        exit(-1);

     // instantiate the ground distribution according to the specification and the input stream
     vector<uint64_t> theGroundDistribution  = 
         getGroundDistribution(specification, cin); // compute the ground distribution for storing

     // close the input stream
     fclose(stdin); 

     uint64_t 

          // store the ground distribution size for performance purpose
          size = theGroundDistribution.size(), 

          // the sum is zero at the begin
          theSum = 0ull, 

          // the distance is just for ouptput formatting
          theDistance = (uint64_t)log10(0 < size ? theGroundDistribution.front() : 0ull) + 1; 

     // if there is at least 4 arguments
     if (argc > 3)       

          // open the output stream 
          if (freopen(argv[3u], "w", stdout) ==NULL)         
                                          
                // if the output stream could not be opened fprint an erro statement
               cerr << "mvvlk: Die Ausgabe " << argv[3u] << " konnte nicht geöffnet werden" << endl,

                    // exit with error
                   exit(-1);

      // print the ground distribution
     for (size_t distributionIndex = 0u; distributionIndex < size; ++distributionIndex)
          cout << setfill(' ') << setw(3u) << (distributionIndex + 1u) << " * "
               << setfill(' ') << setw(theDistance) << theGroundDistribution[distributionIndex] << " "
               << (distributionIndex + 1u) * theGroundDistribution[distributionIndex] << endl,
                // update the sum
              theSum +=(distributionIndex + 1u) * theGroundDistribution[distributionIndex];

      // print the sum
     cout << setfill(' ') << setw(theDistance + 16u) << theSum << endl
          << setfill(' ') << setw(theDistance + 16u) << (theSum >> 3u) << endl,

          // close the output stream
         fclose(stdout), 

          // exit normally
         exit(0u); 
}
