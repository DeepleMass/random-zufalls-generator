/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#ifndef __DRIVER__LOGARITHM__
#define __DRIVER__LOGARITHM__

#include <cstdint>
#include <ostream>
#include <istream>

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
  ostream &outputStream);

#endif // __DRIVER__LOGARITHM__
