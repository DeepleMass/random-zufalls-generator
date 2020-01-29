/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <vector>
#include <cstdint>
#include <ostream>
#include <istream>

using namespace std;
/**
 * this tool converts a binary stream into an integer stream
 * @param inputStream : the input stream
 * @param specification : the number of integer to be read
 * @param outputstream : the output stream
*/
bool binToInteger(
	istream &inputStream,
	uint64_t specification,
	ostream &outputStream)
{

	void *theValue = malloc(specification / 8);

	// bool dataAvailable = false;
	ulong gcount = 0ul;

	do
	{

		inputStream.read((char *)theValue, specification);

		if (specification)
		{

			gcount = inputStream.gcount();

			switch (specification)
			{

			case 64:
				outputStream << *((uint32_t *)theValue) << endl;
				break;

			default:
				outputStream << *((uint64_t *)theValue) << endl;
			}
		}
	} while (inputStream);

	return gcount != specification / 8;
}
