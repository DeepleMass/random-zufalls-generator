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


/**
 * This is a shell wrapper for calling the "ground distribution" generator function.
 * It get an inputstream (default is cin) an provides an output stream (default is cout)
 * the specification is mandatory. It runs in a single thread without parallelizing.
 */
int main(int argc, char **argv)
{

  // Is there only one argument there
  if (argc ==1u)   {

   // Show an help and return 
    cerr << "gvlk <Vorgabewert> [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl
         << "Wenn der Vorgabewert 0 beträgt, dann wird er nicht weiter berücksichtigt" << endl
         << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
         << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl;

        exit(0u); // everything is ok. Bye 
  }

    // instantiate the specification variable
    uint64_t specification = 0ull;

    // if we have more than one argument 
    if (argc > 1)
    {
      // get the specification from the command line  
      specification = strtoull(argv[1], NULL, 10);

      // if the above line has failed or we had zero
      if (specification ==0ull)

        // send an erro message to console
        cerr << "The specification shall not be 0! Exiting" << endl,

            exit(-1); // exit angry
    }

    // if we have at least 3 Arguments 
    if (argc > 2)

      // if the 3rd argument is not "-", so we won't read from the console
      if (strcmp(argv[2u], "-") !=0u)

        // if we could not open the file 
        if (freopen(argv[2u], "r", stdin) ==NULL)

          // send an error to the console 
          cerr << "gvlk: Die Eingabe " << argv[2u] << " kann nicht geöffnet werden" << endl,

              // exit angry 
              exit(-1);

    // this is the ground distribution  
    vector<uint64_t> theGroundDistribution =

        // we get it from cin and store it 
        getGroundDistribution(specification, cin);

    // we close the std input 
    fclose(stdin);

    // this is the cumulation index. Set to zero
    uint64_t cumulationIndex = 0ULL;


    // we get the cumulated ditstribution as a vector of integet according to 
    // the ground distribution
    vector<uint64_t> theCumulatedDistribution(theGroundDistribution.size(), 0ULL);

    // we set the begin of the cumulated distribution
    uint64_t
        *datenBeginn = theCumulatedDistribution.data();

    // we process the whole ground distribution
    while (!theGroundDistribution.empty())
    {

      // while the last entry of ground distribution is not null
      while (0ULL < theGroundDistribution.back())
      {
        //we accumulate the elements from the Ground distribution
        for (cumulationIndex = 0u; cumulationIndex < theGroundDistribution.size(); ++cumulationIndex)
          *(datenBeginn + cumulationIndex) +=theGroundDistribution.size() - cumulationIndex;
        --theGroundDistribution.back();
      }
      // now we remove the las element from the ground distibution
      // and go into the next loop
      theGroundDistribution.pop_back();
    }

    //
    uint64_t
        //  we get the size  to the cumulated distribution size 
        size =theCumulatedDistribution.size(), 
        // get the distance (just for the right format) 
        coutPadding = (uint64_t)log10(0 < size ? theCumulatedDistribution.front() : 0ULL) + 1; 

    // if there are at least 3 Arguments in the command line 
    if (argc > 3) 

      // we reopen the std output
      if (freopen(argv[3u], "w", stdout) ==NULL){

        // if this failed we send an error to the console and exit  
        cerr << "mvvlk: Die Ausgabe " << argv[3u] << " konnte nicht geöffnet werden" << endl; 
        
        // exit with error code  
        exit(-1);
}

    // print each element of the cumulated distribution
    for (size_t distributionIndex = 0u; distributionIndex < size; ++distributionIndex)
      // print the formatted value 
      cout << setfill(' ') << setw(3u) << (distributionIndex + 1u) << " : "
           << setfill(' ') << setw(coutPadding) << theCumulatedDistribution[distributionIndex] << endl;

    // close the std ouput 
    fclose(stdout), 

        // Exit safely 
        exit(0u);
  }
