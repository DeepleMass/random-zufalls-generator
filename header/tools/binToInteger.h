/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#ifndef __WERKZEUG__BINTOINTEGER__
#define __WERKZEUG__BINTOINTEGER__

#include <cstdint>
#include <ostream>
#include <istream>

using namespace std;

bool binToInteger(
  istream &eineEingabe,
  uint64_t vorgabeWert,
  ostream &eineAusgabe);

#endif // __WERKZEUG__BINTOINTEGER__
