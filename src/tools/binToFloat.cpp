
/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <cmath>
#include <vector>
#include <cfloat>
#include <cstdint>
#include <ostream>
#include <istream>

using namespace std;
/**
 * this is a very simple tool that convert a bit series in a series of floating
 * point number
 * @param inputStream the bit input stream - cin as default
 * @param specification the number of bit contained in the input stream
 * @param ouputStream the resulting series of floating number
 * @return true if everything went well, false in all other cases
 */ 
bool binToFloat(
	istream &inputStream,  
	uint64_t specification,
	ostream &outputStream) {

	// this is the byte specification (specification is always in bit)
	const int bytesSpecification = specification < 8 ? 1 : specification / 8;

	uint64_t

		// this is the bit sequence
		theBitSequence = 0ULL,

		// this is the current value
		currentValue = 0ULL;

	// this is the number of read bytes
	int readBytesNumber = 0;

	// this is the dividing factor for conversion from long to double
	double factor = pow(2.0, (double)specification);

	// process until ...
	while (true)
	{
		// if some bytes are available
		if (readBytesNumber != 0)
		{
			// if more than 8 Bits ar specified
			currentValue = theBitSequence >> (8 - bytesSpecification);

			// print the formated current value on the display
			outputStream << ((double)currentValue) / factor << endl;
		}

		// if the input cannot be read
		if (!inputStream.read((char *)&theBitSequence, bytesSpecification))

			// break the computation
			break; 
			
		// get the number of read bytes
		readBytesNumber = inputStream.gcount();

		// if this doesn't match the specification
		if (readBytesNumber != bytesSpecification)
		
		// return with error
			return false;
	}

	// return safely
	return true;
}
