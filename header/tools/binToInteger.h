/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#ifndef __TOOL__BIN_TO_INTEGER__
#define __TOOL__BIN_TO_INTEGER__

#include <cstdint>
#include <ostream>
#include <istream>

using namespace std;

/**
 * this function converts a binary stream into
 * it integer number counterpart up to the number
 * of bits given as specification
 * @param inputStream the input binary stream. may be padded by the operating system if the stream length
 * is not a multiple of 8 
 * @param specification the number of bits to be processed. This information is important if the number
 * of bits is not a multiple of 8. (see input stream)
 * @param outputStream streams the corresponding integer numbers
 */
bool binToInteger(
  istream &inputStream,
  uint64_t specification,
  ostream &outputStream);

#endif // __TOOL__BIN_TO_INTEGER__
