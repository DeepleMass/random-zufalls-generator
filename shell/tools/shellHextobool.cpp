/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <iostream>
#include <vector>

using namespace std;

/**
 * This script converts a series of hexadecimal number in its boolean conterpart
 */
int main (int argc, char** argv)
{

// we need exactly two arguments
 if (argc!=2)

  // exit with error code
  exit (-1);

 // read the input from the command line
 uint64_t input = strtoull(argv[1],NULL,16);

 // if the input is zero
 if (input==0ull)

  // print it on the output
  cout << 0<< endl, 

  // exit safely
  exit(0); 
 
 // instantiate a boolean vector 
 vector<bool> avector;

 // process the input until it is zero
 while (input!=0ull)
 {

  // push the lsb of input onto the disbplay
  avector.push_back(input & 0x1ull?true:false); 
  
  // shift the input one to the right
  input>>=1;
 }

 // we process until the vector is emty
 while (!avector.empty())
 {

  // print the vector last element on the output   
  cout << avector.back ();

  // pop the last vector element 
  avector.pop_back (); 
 }

 // line feed
 cout << endl;

 // exit safely
 exit(0); 
}
