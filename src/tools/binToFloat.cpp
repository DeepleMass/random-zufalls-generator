/*
c++ -std=c++11 -c src/werkzeuge/binToFloat.cpp -o /tmp/a
*/

#include <cmath>
#include <vector>
#include <cfloat>
#include <cstdint>
#include <ostream>
#include <istream>

using namespace std;

bool binToFloat(
  istream &eineEingabe, // eingabe Strom für die Bits
  uint64_t vorgabeWert, // in bits (wie viele Bits auf ein mal)
  ostream &eineAusgabe) // wohin mit der Ausgabe
{
	const int vorgabeBytes = vorgabeWert<8? 1: vorgabeWert / 8;

	uint64_t
	 dieBitSequence = 0.0,
	 aktuellerWert = 0.0;
	
	int geleseneBytes = 0;
//	int verarbeiteteBits = 0;

  double factor = pow(2.0,(double)vorgabeWert) ;

	// uint8_t dieBitMaske = 0b11111111 ;
	
	// switch (vorgabeWert){
	// 	case 1:
	// 		dieBitMaske = 0b10000000;
	// 		break;
	// 	case 2:
	// 		dieBitMaske = 0b11000000;
	// 		break;
	// 	case 4:						
	// 		dieBitMaske = 0b11110000;
	// 		break;
	// }

	while(true){

		if (geleseneBytes!=0){
/*
			if (vorgabeWert < 8){ // wenn der Vorgabewert kleiner ist als ein Byte

				dieBitSequence >>= 56;

				while(verarbeiteteBits!=8){
				 aktuellerWert = (dieBitSequence && dieBitMaske) >> (7-verarbeiteteBits);
				 float ausgabeWert = aktuellerWert ;
				 eineAusgabe << aktuellerWert;
				 dieBitMaske >>= vorgabeWert;
				 verarbeiteteBits +=vorgabeWert;
				}
				
			}

			else */{ // wenn mehr als 8 Bits vorgegeben sind
				aktuellerWert = dieBitSequence >> (8-vorgabeBytes);
				eineAusgabe << ((double) aktuellerWert) / factor << endl;	
			}
			
	 }
/*
		if (geleseneBytes == vorgabeBytes){
			
			continue // !!!! endlos schleife
		}
*/
	 if(!eineEingabe.read(	(char*)&dieBitSequence, vorgabeBytes) )
		break; // so lange gelesen werden kann

	 geleseneBytes = eineEingabe.gcount();

	 if (geleseneBytes != vorgabeBytes)
		return false;

//	 verarbeiteteBits = 0;
	}

	return true;
}

	/*
	const size_t SequenceLaenge = istream.tellg(); // ermittle die Länge der Datei

  char *dieBitSequence = new char [SequenceLaenge]; // reserviere speicherplatz

   // lese die Datei


	for (
		int relativerZeiger = 0; 
		relativerZeiger < SequenceLaenge; 
		relativerZeiger+= vorgabeWert<8? 1: vorgabeWert / 8){

		double einWert = (Double)
	}
  //while(eineEingabe.read((char*)derWert, zuLesendeOktete)){
 
  //eineEingabe.read((char*)derWert, zuLesendeOktete);

  //if (eineEingabe){
		gcount = eineEingabe.gcount();

		switch (vorgabeWert){
		 case  64 : 
		  eineAusgabe << *((double*) derWert) << endl;
			break;
		 case 32 :
			eineAusgabe << *((float*) derWert) << endl;
			break;
		 default :
		  eineAusgabe << *((float*) derWert) << endl;
 		}
 	}
	delete[] derWert
 //}while(eineEingabe);

 return  (gcount != vorgabeWert/8);
*/
//}
 //#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
 //#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-

