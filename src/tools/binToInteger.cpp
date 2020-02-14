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

#include <iostream>

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
	
	// sanity check of specification
	if(specification != 32ULL and specification != 64ULL)

		// print an error message 
		cerr << "bintoint: the specification shall be 32 or 64. It is set to "<< specification<< ". Exiting" << endl,
		
		// exit with error
		exit (-1) ;

	// this is the current value according to specification
	void *theValue = malloc(specification / 8);

	// this just counts the number of read bytes
	uint64_t gcount = 0ul;

	// if there is something to read
	while(inputStream.read((char *)theValue, specification /8)) 
	{
		// read the number of specified bytes
		
		// if the specification is not zero
		if (specification)
		{
			// acquire the number of read bytes
			gcount = inputStream.gcount();

			// make a differenciated use of specification
			switch (specification)
			{

			// if we process 64 bits
			case 64:
				outputStream << *((uint64_t *)theValue) << endl;
				break;

			// in other cases
			default:
				outputStream << *((uint32_t *)theValue) << endl;
			}
		}
	} 
	free(theValue); // we are done. Let's free the memory

	return gcount != specification / 8; // if all is good return true, else false.
}