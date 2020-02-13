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
 * this script convert a serie of decimal number in a series of boolean
 */
int main (int argc, char** argv)
{

 // if there are not exactly 2 arguments on the command line 
 if (argc!=2) 
 
  // exit with error
  exit (-1); 

 uint64_t input = strtoull(argv[1],NULL,10); 

 // if the input is zero
 if (input==0ull)
 
  // print a 0 on the display
  cout << 0<< endl,

  // exit safely
  exit(0); 
 

// instantiate a boolean vector
 vector<bool> aVector; 

 // process the input until its end
 while (input!=0ull) 
 {

  // push the last element of the input into the vector
  aVector.push_back(input & 0x1ull?true:false); 
  
  // shift the input one to the right
  input>>=1; 
 }

 // while the vector is not empty
 while (!aVector.empty())
 {
  
  // put the last element of the vector on the display
  cout << aVector.back (); 

  // pop the last element of the vector
  aVector.pop_back ();
 }

 // line feed on ouput 
 cout << endl; 
 
 // exit safely
 exit(0);

}
