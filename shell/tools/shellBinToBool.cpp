/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <bitset>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>

using namespace std;
/**
 * this shell convert binary data in 
 * readable 0/1 format of 64 boolean prer line
 */

int main(int argc, char **argv)
{

  // if there is only one argument
  if (argc ==1) 
  
    // print a statement  
    cerr << "bintobool [/Pfad/zur/Eingabe] [/Pfad/zur/Ausgabe]" << endl
         << "Einfacher Bindump für eine belibiege Datei. Schreibt 64 Bit pro Zeile." << endl
         << "</Pfad/zur/Eingabe> kann auch \"-\" sein oder ausgelassen werden. Dann wird es aus stdin gelesen" << endl
         << "Wenn </Pfad/zur/Ausgabe> ausgelassen wird, so wird es auf stdout geschrieben" << endl,

    // exit safely
    exit(0);
  
  // if there is at least 2 arguments
  if (argc > 1) 

    // if the input file not is set to "-" 
    if (strcmp(argv[1u], "-") != 0)

      // reopen the stdin
      if (freopen(argv[1u], "r", stdin) == NULL)

       // if stdin cannot be reopened we print a statement
        cerr << "sglk: Die Eingabe " << argv[1u] << " kann nicht geöffnet werden" << endl, 

        // we exit with error code
        exit(-1);
      
  // if there is at least 3 arguments
  if (argc > 2)

    // we reopen the std output stream
    if (freopen(argv[2u], "w", stdout) == NULL)
    
      // if it failed we print a statement
      cerr << "sglk: Die Ausgabe " << argv[2u] << " kann nicht geöffnet werden" << endl,
      
      // exit with error
      exit(-1);
    

  uint8_t
      // this is the input byte
      input = 0, 
      // the line feed counter
      ensemble = 0;   
  // while reading goes right
  while (cin.read((char *)&input, 1u)) 
  {
    // print the input as bitset on the display
    cout << bitset<8>(input);
    
    // update the line feed counter
    ensemble += 8;                 
    
    // if we have written 64 bits o the ouput
    if (ensemble == 64)            
    
      // print an end of line
      cout << endl, 

      // reset the line feed counter
      ensemble = 0;
  }

  // if the line is not zero
  if (ensemble > 0)

    // put a line feed on the output
    cout << endl;

  // close the ouptut
  fclose(stdout); 
  
  // close the input
  fclose(stdin); 

  // eit safely
  exit(0u);
}
