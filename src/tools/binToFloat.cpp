
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
	istream &inputStream,   //  // de-DE input stream 
	uint64_t specification, //  // de-DE in bits
	ostream &outputStream)  //  // de-DE output stream
{
	const int bytesSpecification = specification < 8 ? 1 : specification / 8;

	uint64_t
		theBitSequence = 0.0,
		currentValue = 0.0;

	int readBytesNumber = 0;

	double factor = pow(2.0, (double)specification);

	while (true)
	{

		if (readBytesNumber != 0)
		{ //  // de-DE if more than 8 Bits ar specified
			currentValue = theBitSequence >> (8 - bytesSpecification);
			outputStream << ((double)currentValue) / factor << endl;
		}

		if (!inputStream.read((char *)&theBitSequence, bytesSpecification))
			break; //  // de-DE while the input is not done

		readBytesNumber = inputStream.gcount();

		if (readBytesNumber != bytesSpecification)
			return false;
	}

	return true;
}
