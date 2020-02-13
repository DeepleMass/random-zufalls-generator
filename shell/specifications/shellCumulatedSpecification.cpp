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
     // if there is only one agrument 
     if (argc ==1) 
     
          // print an error message on the screen
          cerr << "vvlk <specification> [/Pfad/zur/Ausgabe]" << endl
               << "Berechnet die kumulierte Vorgabe des angegebenen specificationes." << endl
               << "Wenn kein Pfad zur Ausgabe angegeben wird, wird es auf stdout geschrieben" << endl,

          // exit safely
          exit(0u);
     

     // instantiate the ground specification
     uint64_t specification = 0Ull;

     // if there is more than one argument
     if (argc > 1)
     {
         
         // get the specification from the argument line
          specification = strtoull(argv[1], NULL, 10); 

          // if the specification is still zero
          if (specification == 0Ull)

               // put an error message on the screen and 
               cerr << "The specification shall not be 0! Exiting" << endl,

               // exit with error code
                   exit(-1);
     }

     // get the ground distribution out of the specification
     vector<uint64_t> theGroundDistribution = getSpecification (specification);

     // if there are at least 3 arguments
     if (argc > 2)

          // if the input file could not be opened 
          if (freopen(argv[2u], "w", stdout) == NULL)

               // put an error message on the display
               cerr << "vvlk: Die Ausgabe " << argv[2u] << " kann nicht geöffnet werden" << endl,

               // exit with error code
               exit(-1);
          

     uint64_t
         // get the ground distribution size
         size = theGroundDistribution.size(),

         // set the data begin as a pointer
         *datenBeginn = theGroundDistribution.data(),

         // set the data end as a pointer
             *datenEnde = datenBeginn + size,      

             // set a reverse pointer as the end of the data set
                 *datenREnde = datenBeginn - 1u,     

                  // set a pointer next to the data begin
                     *dataPointer = datenBeginn + 1u,

          // set the data index to 2
         dataIndex = 2u,

         // set the current pointer to the data begin
         *currentPointer = datenBeginn,

         // this is just for output formating
         padding = 2u;

     // we process all data in the set
     while (dataPointer < datenEnde)
     {
          while (datenREnde < currentPointer)
               *currentPointer += *dataPointer * padding,
                   padding += 1u,
                   currentPointer--;

          dataIndex +=1u,
              currentPointer = dataPointer,
              dataPointer++,
              padding = 2u;
     }

     // set the format padding
     padding = (uint64_t)log10(theGroundDistribution.front()) + 1;

     // print the ground distribution and the bit sum
     for (uint64_t distributionPointer = 0u; distributionPointer < size; ++distributionPointer)
          cout << setfill(' ') << setw(3u) << (distributionPointer + 1) << " : "
               << setfill(' ') << setw(padding) << theGroundDistribution[distributionPointer] << endl;

     // close the output file
     fclose(stdout); 

     // exit safely
     exit(0u); 
}
