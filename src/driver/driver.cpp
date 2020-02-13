/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <vector>
#include <cstdint>


#include <ostream>
#include <istream>

#include "../../header/specifications/groundspecification.h"

using namespace std;

/**
 * driver is the core function of egef.
 *  
 * @param inputStream This is the input stream. The highest ist entropy,
 * the fastest the driver is done with producing the output stream of bit
 * having maximal entropy according to paper.
 * 
 * @param specification The number of bits the output stream shall provide.
 * (not to be understood as number of Bytes)
 * 
 * @param outputStream This is the output stream. It entropy is said to be maximal.
 * Say it is a random bit series
 *
 */
bool driver(
    istream &inputStream,
    uint64_t specification,
    ostream &outputStream)
{

  if (specification <= 0ull) // if the specification is zero 
    return true;             // there is nothing to do 

  uint64_t
      currentAmount = 0ull,         // the amount of the current run length in the ground distribution 
      down = 0xFFFFFFFF00000000ull, // upper bound for the log search 
      top = 0ull,                   // lower bound for the log search 
      rang = 0ull,                  // the current rang 
      currentCumulation = 0ull,     // the current runs cumulation 
      inputPuffer = 0ull;           // the input buffer 

  uint64_t
      outputBuffer = 0u,        // the output buffer (64 bits)  
      inputCounter = 0u,        // the number of read input  bit  
      outputCounter = 8u,       // the output buffer counter and the index of specification 
      size = 0u,                // the size of ground distribution
      outputSpecification = 0u; // the length of next run 

  vector<uint64_t> simpleDistribution = // the specification vector 
      getSpecification(specification);  // initialize using the specification and ground distribution 

  vector<pair<uint64_t, uint64_t>> theSpecifiedDistribution; // the vector of pair for run length and distribution 

  while (!simpleDistribution.empty()) // we process all of it until it is empty
  {

    if (0u < simpleDistribution.back()) // we populate the vector of pairs

      theSpecifiedDistribution.push_back(                                   // a pair of run length and amount 
          make_pair(simpleDistribution.size(), simpleDistribution.back())), // to be pushed back 
          currentAmount += simpleDistribution.back();                       // increment the current amount 

    simpleDistribution.pop_back();                                          // remove the last entry from the simple distribution 
  }

  size = theSpecifiedDistribution.size(); // get the size of the specified distribution 

  pair<uint64_t, uint64_t>
      *dataBegin = theSpecifiedDistribution.data(), // a pointer to the data begin 
      *specificationPointer = dataBegin;            // a pointer to the specification distribution 

  bool
      releaseIt = false,   // at the first loop we don`t process the output 
      readOut = false,     // set when the input has been read out (stops processing) 
      inputStatus = false, // the status of input 
      outputStatus = true; // the status of output 
  
  while (0u < size) // while the specification is not satisfied 
  {

    // special case if the specification distribution has only one element 
    if (size <= 1u) 
      releaseIt = true, // set the release flag
      specificationPointer = dataBegin; // set the specification pointer to the first index of distribution 

    outputSpecification = specificationPointer->first; // set the current tmp output specification 

    // processing the output 
    if (releaseIt) // if the output shall be released 
    {

      while (0ull < outputSpecification) // until the output specification is not zero 
      {

        outputBuffer |= outputStatus ? 0x1u : 0x0u, // the lower bit of output buffer 
            outputSpecification -= 1u,              // decrement the output specification 
            outputCounter -= 1u;                    // decrement the output counter 

        if (outputCounter <= 0u)          // if the output counter is zero 

          outputStream.put(outputBuffer), // write the output buffer on the output stream 
              outputCounter = 8u,         // reset the output counter 
              outputBuffer = 0u;          // reset the output buffer 
        else

          outputBuffer <<= 1u; // shift the output buffer to the left 
      }

      currentAmount -= 1u,                    // decrement the current amount 
          outputStatus = !outputStatus,       // toggle the output status 
          specificationPointer->second -= 1u; // decrement the specified amount 

      if (specificationPointer->second <= 0u)     // is this specified amount zero 
        theSpecifiedDistribution.erase(theSpecifiedDistribution.begin() + (specificationPointer - dataBegin)), // remove this from the ground distribution 
        size = theSpecifiedDistribution.size();                                                            // decrement the ground distribution 

      if (size <= 0u) // if the specification is not satisfied  
      {

        if ((bool)(specification & 7u))        // if the specification is a multiple of eight 

          outputBuffer <<= outputCounter - 1u, // shift the output buffer to the left 
              outputStream.put(outputBuffer);  // write the output buffer into out to the output stream 
        break;                                 // the process has reached its end 
      }

      if (size <= 1u) // if the specification has only one entry left 

        continue;     // repeat from then begin or stop if the specifications are satisfied 
    }

    // process the input 

    rang = 0ull, // the rang is set to zero
    top = 0ull,               // the top is set to zero 
        down = currentAmount; // the lower bound is set to the current amout 

    while (top < down) // this is always true 
    {

      if (inputCounter <= 0u) // if the input buffer has been read 
      {

        inputStream.read((char *)&inputPuffer, 8u),      // fill the input buffer 
            inputCounter = inputStream.gcount() << 3u,   // set the number or read bits 
            readOut = inputCounter <= 0u ? true : false; // if the input stream has been read out 

        if (readOut) // if the input buffer has been read out 
          break;     // abort the logarithm search 

        inputPuffer <<= 64u - inputCounter; // shift the input buffer to the left 
      }

      inputPuffer <<= 1u,                                            // shifts the input buffer to the left 
          inputCounter -= 1u,                                        // decrement the input counter 
          inputStatus = (bool)(inputPuffer & 0x8000000000000000ull); // get the value of the input status 

      if (top + 1u < down) // the distance between bounds is at least one 
      {
        if (inputStatus)                // is the input bit true 

          down -= ((down - top) >> 1u); // decrement the upper bound 
        else                            // if the input run has not reached its end 

          top += ((down - top) >> 1u);  // increment the lower bound 
        continue;                       // have to read some new bits 

      }

      break; // stops the computation 
    }

    rang = inputStatus ? top : down; // compute the rang 

    if (readOut) // if the input counter is zero 
      break;     // abort the computation 

    // find the next run lengths 
    specificationPointer = dataBegin,          // initialize the specification pointer 
        currentCumulation = dataBegin->second; // reset the current cumulation 

    while (currentCumulation < rang and specificationPointer + 1u < dataBegin + size) // until the current cumulation is smaller than the rang 
      ++specificationPointer,                                                         // increment the specification pointer 
          currentCumulation += specificationPointer->second;                          // increment the current cumulation 
  
    // this is done only at the first loop 
    if (!releaseIt)
      outputStatus = inputStatus, // initialize the output status  
          releaseIt = true;       // set the ouput free 
  }

  return currentAmount == 0ull ? true : false; // return if the driver has end well // eigneter Rückgabewert berechnen
}