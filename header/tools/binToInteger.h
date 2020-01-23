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

bool binToInteger(
  istream &inputStream,
  uint64_t specification,
  ostream &outputStream);

#endif // __TOOL__BIN_TO_INTEGER__
