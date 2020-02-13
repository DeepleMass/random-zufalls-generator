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
#include <vector>
#include <cmath>
#include <iomanip>
#include "../../header/specifications/groundspecification.h"

using namespace std;

int main(int argc, char **argv)
{
      // if there is only one argument
     if (argc ==1)
     {
          // put a help message on the display
          cerr << "gvet <Vorgabewert> [/Pfad/zur/Ausgabe]" << endl
               << "Berechnet Grundvorgabe des angegebenen Vorgabewertes." << endl
               << "Wenn kein Pfad zur Ausgabe angegeben wird, wird es auf stdout geschrieben" << endl;

          // exit safely
          exit(0u);
     }

     // instantiate the specification
     uint64_t specification = 0ULL; 

     // if there is more than one argument
     if (argc > 1)
     {
          // read the specification from the input line
          specification = strtoull(argv[1], NULL, 10);

          // if the specification remained 0
          if (specification ==0)

               // put an error statement on the screen
               cerr << "The specification shall not be 0! Exiting" << endl,
                   // exit with code -1 
                   exit(-1);
     }

     // instantiate the ground distribution
     vector<uint64_t> groundDistribution = 
          getSpecification(specification); 
          
     // if there is more than one argument
     if (argc > 2)

          // if the file cannot be opened
          if (freopen(argv[2u], "w", stdout) ==NULL)
          {
               // put an error message on the display
               cerr << "vvlk: Die Ausgabe " << argv[2u] << " kann nicht geöffnet werden" << endl;
               
               // exit with error
               exit(-1);
          }

     // get the distribution size
     uint64_t size = groundDistribution.size(),

     // the bit sum is zero 
     summe = 0ull, 

     // just for formating the output
     abstand = (uint64_t)  log10(groundDistribution.front()) + 1;

     //  we print the ground distribution
     for (uint64_t distributionIndex = 0u; distributionIndex < size; ++distributionIndex) 
     {
          cout << setfill(' ') << setw(3u) << (distributionIndex + 1) << " * " 
               << setfill(' ') << setw(abstand) << groundDistribution[distributionIndex] << " "
               << (distributionIndex + 1) * groundDistribution[distributionIndex] << endl;
          summe +=(distributionIndex + 1) * groundDistribution[distributionIndex]; // sum up the number of bits
     }

     // print the bit and bytes sum
     cout << setfill(' ') << setw(abstand + 16) << summe << endl
          << setfill(' ') << setw(abstand + 16) << (summe >> 3u) << endl;
     
     // close the output
     fclose(stdout); 
     
     // exit safely
     exit(0u);
}
