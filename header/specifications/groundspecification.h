/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#ifndef __SPECIFICATION__
#define __SPECIFICATION__

#include <vector>
#include <cstdint>

using namespace std;

/**
 * getSpecification this produce a bits run distribution according to the specification 
* @param specification the number of bit the driver should produce as output stream having maximal entropy
* @return a vector or integer which stands for the distribution
 * of bit runs over a spread of "1" until "specification" in order of index (+1).
*/
vector<uint64_t> getSpecification(uint64_t specification);

#endif // __SPECIFICATION__
