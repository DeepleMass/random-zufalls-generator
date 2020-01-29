/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#ifndef __GROUND_DISTRIBUTION__
#define __GROUND_DISTRIBUTION__

#include <vector>
#include <cstdint>
#include <istream>

using namespace std;

/**
* groundDistribution 
* @param specification The number of bits the output stream shall provide.
* (not to be understood as number of Bytes)
*
* @param inputStream  This is the input stream. The highest ist entropy,
 * the fastest the driver is done with producing the output stream of bit
 * having maximal entropy according to paper.
*
* @return a vector of integer 
*/
vector<uint64_t> groundDistribution(uint64_t specification, istream & inputStream);

#endif // __GROUND_DISTRIBUTION__
