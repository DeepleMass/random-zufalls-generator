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

	// this is the current value according to specification
	void *theValue = malloc(specification / 8);

	// this just counts the number of read bytes
	ulong gcount = 0ul;

	do // just do it !
	{
		// read the number of specified bytes
		inputStream.read((char *)theValue, specification);

		// if the specification is not zero
		if (specification)
		{
			// aquiere the number of read bytes
			gcount = inputStream.gcount();

			// make a differenciated use of specification
			switch (specification)
			{
			// if we process 64 bits
			case 64:
				outputStream << *((uint32_t *)theValue) << endl;
				break;
			// in other cases
			default:
				outputStream << *((uint64_t *)theValue) << endl;
			}
		}
	} while (inputStream); // while there is something to read

	free(theValue); // we are done. Let's free the memory

	return gcount != specification / 8; // if all is good return true, else false.
}
