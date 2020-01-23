/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#ifndef __TOOL__BIN_TO_FLOAT__
#define __TOOL__BIN_TO_FLOAT__

#include <cstdint>
#include <ostream>
#include <istream>

using namespace std;

bool binToFloat(
  istream &inputStream,
  uint64_t specification,
  ostream &outputStream);

#endif // __TOOL__BIN_TO_FLOAT__
