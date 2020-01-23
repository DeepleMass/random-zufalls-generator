/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

/*
Auf Branch Generatoren
Volllogarithmischer Vorschub mit Entfernung der abgearbeiteten Häufigkeiten.
*/

#include <vector>
#include <cstdint>
#include <ostream>
#include <istream>


using namespace std;

bool binToInteger(
  istream &eineEingabe,
  uint64_t vorgabeWert,
  ostream &eineAusgabe)
{

 void *derWert = malloc(vorgabeWert/8);

// bool dataAvailable = false;
 ulong gcount = 0ul;

 do{
 
  eineEingabe.read((char*)derWert, vorgabeWert);

  if (eineEingabe){

		gcount = eineEingabe.gcount();

		switch (vorgabeWert){

			case  64 : 
				eineAusgabe << *((uint32_t*) derWert) << endl;
				break;

			default :
				eineAusgabe << *((uint64_t*) derWert) << endl;
  }
}

 }while(eineEingabe);

 return  (gcount != vorgabeWert/8);

}
