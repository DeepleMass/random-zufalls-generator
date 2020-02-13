/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <vector>
#include <cstdint>

using namespace std;
/**
 * get specification produces a distribution of bits runs according
 * to a given specification, which is the length of the random bits
 * series to be computed
 * @param specification is the number of bit to be randomly generated
 * @return a distribution as vector of integer where the index+1 is the
 * run length and the value is the number of runs to be produced
 */ 
vector<uint64_t> getSpecification(uint64_t specification)
{
  // if the specification is 0
  if (specification == 0ull)     
    // return an empty vector
    return vector<uint64_t>(0);
    

#ifndef _FULL_
#define _FULL_ 0x7FFFFFFFFFFFFFFFull
  // set each bit of the specification to be one
  specification = _FULL_ < specification ? _FULL_ : specification;
#endif

  uint64_t
  
      // set the number of indices
      numberOfIndices = 0ull,                 
      
      // set the temproray specification
      tmpSpecification = specification, 

      // set the temporary cumulation
      tmpCumulation = 1ull;

  // compute the number of indices which are neede for the ground distribution
  while (0 < (specification >> numberOfIndices)) 
    numberOfIndices++;                       

  // declare the distribution
  vector<uint64_t> theDistribution = 

    // instantiate the distribution with zeros
    vector<uint64_t>(numberOfIndices, 0ull); 

  uint64_t

      // this is a pointer to the data begin
      *dataBegin = theDistribution.data(), 
      
      // set the data end to data begin value minus 1
      *dataEnd = dataBegin - 1u,

      // set the pointer index to data begin
      *pointerIndex = dataBegin,     

      // set the current index to data begin
          *currentIndex = dataBegin; 

  // set the temporary specifications
  tmpSpecification = specification; 

  // process the longest bit run within the ground distribution
  do
  {

    // if the lsb of temporary specification is set
    if (tmpSpecification & 1ull)  

    // set the pointer index to 1
      *pointerIndex = 1ull;  

      // shift the temporary specification one tho the right
    tmpSpecification >>= 1u;

    // increment the pointer index
    ++pointerIndex;  

    // stop if the specification has become 0
  } while (0ull < tmpSpecification);

  // set the pointer index to data begin minus 2
  pointerIndex = dataBegin - 2u,

      // set the current index to data begin minus 2
      currentIndex = dataBegin - 2u;

  // while the specification has not be processed til its end
  while (0ull < specification) 
  {

    // if the lsb of specification is one
    if (specification & 1ull) 
    {

      // set the temporary cumulation to one
      tmpCumulation = 1ull;

      // set the current index to the pointer index
      currentIndex = pointerIndex; 
      
      // while the end of data is not reached
      while (dataEnd < currentIndex)

        // update the current index using the temporary cumulation
        *currentIndex += tmpCumulation, 
            // shift the temporary cumulation one to the left
            tmpCumulation <<= 1u,       
            // decrement the current index
            currentIndex--;
    }

    // shift the specification one to the right
    specification >>= 1u, 

        // increment the index pointer
        pointerIndex++; 
  }

 // return the distribution
  return theDistribution;
}
