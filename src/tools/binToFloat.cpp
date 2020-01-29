
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
 * 
 */ 
bool binToFloat(
	istream &inputStream,   // input stream 
	uint64_t specification, // in bits
	ostream &outputStream)  // output stream
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
		{ // if more than 8 Bits ar specified
			currentValue = theBitSequence >> (8 - bytesSpecification);
			outputStream << ((double)currentValue) / factor << endl;
		}

		if (!inputStream.read((char *)&theBitSequence, bytesSpecification))
			break; // while the input is not done

		readBytesNumber = inputStream.gcount();

		if (readBytesNumber != bytesSpecification)
			return false;
	}

	return true;
}
