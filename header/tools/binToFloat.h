#ifndef __WERKZEUG__BINTOFLOAT__
#define __WERKZEUG__BINTOFLOAT__

#include <cstdint>
#include <ostream>
#include <istream>

using namespace std;

bool binToFloat(
  istream &eineEingabe,
  uint64_t vorgabeWert,
  ostream &eineAusgabe);

#endif // __WERKZEUG__BINTOFLOAT__
