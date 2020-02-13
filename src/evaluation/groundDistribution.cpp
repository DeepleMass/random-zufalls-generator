/*
#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#
*/

#include <vector>
#include <cstdint>
#include <istream>

using namespace std;

/**
* groundDistribution 
* @param specification The number of bits the output stream shall provide.
* (not to be understood as number of Bytes)
*
* @param inputStream  This is the input stream. The highest ist entropy,
 * the fastest the driver is done with producing the output stream of bit
 * having maximal entropy according to paper.
*
* @return a vector of integer 
*/
vector<uint64_t> getGroundDistribution(uint64_t specification, istream &inputStream)
{

       // if the specifi ation is zero
       if (specification == 0ull)          
              // return an empty vector
              return vector<uint64_t>(0u);

       uint64_t

           // set the octet to the first value of input stream
           octet = (uint64_t)inputStream.get(),

           // set the input stream counter 
           inputStreamCounter = inputStream.gcount() << 3u;

       // if the input stream counter is 0
       if (inputStreamCounter == 0u)  

              // return an empty vector
              return vector<uint64_t>(0u);
       
       // if the specification is smaller than the input stream counter
       if (specification <= inputStreamCounter)    
       
              // reset the input stream counter to the specification
              inputStreamCounter = specification;  
       else                                      
              // update the specification
              specification -= inputStreamCounter;

       // instantiate the ground distribution vector
       vector<uint64_t> theGroundDistribution;

       // allocate some memory for the distribution (40 may be a good orientation)
       theGroundDistribution.reserve(40u); 

       uint64_t

           // instantiate the size to be zero
           size = 0u;

       uint64_t
       
           *dataBegin = theGroundDistribution.data(), 
           // set the data end to data begin minus one
       
           *dataEnd = dataBegin - 1u,                 
               // set the current amout to data begin minus one
       
               *currentAmount = dataBegin - 1u,       
                     // set the data begin backup to data begin
                   *dataBeginBackup = dataBegin;     
                   
       // reset the difference to begin after reallocation to 0

       int64_t diffBeginAfterRealloc = 0ull; 

       bool

           // set the input status to the octet's msb
           inputStatus = (bool)(octet & 0x80u),

           // set the early stop flag to false
           earlyStop = false,

           // set the specification fulfillement flag to false
           specificationFulfilled = false;  

       // process until ...
       while (true)
       {

              // if the current amount is no above data end plus the size
              if (currentAmount > dataEnd + size)
              {

                     // append 0 to the gournd distribution array
                     theGroundDistribution.push_back(0ull); 

                     // if the data begin backup is not set to the ground distribution data array 
                     if (theGroundDistribution.data() - dataBeginBackup != 0)

                            // set the data begin to the ground distribution data array
                            dataBegin = theGroundDistribution.data(),       

                                // compute the difference to data begin after realloc 
                                diffBeginAfterRealloc = dataBegin - dataBeginBackup, 

                                // make a backup of data begin
                                dataBeginBackup = dataBegin,     

                                // set the data end
                                dataEnd = dataBegin - 1u,    

                                // update the current amount
                                currentAmount += diffBeginAfterRealloc;  

                     // increment the size
                     ++size;
              }
              
              // if the specification is fulfilled and 
              // the input stream counter is zero
              if (specificationFulfilled and inputStreamCounter == 0u)
              {
                     // update the current amount
                     *currentAmount += 1;
                     
                     // break the computation
                     break;
              }

              // compare the octet's msb with the input status
              if ((bool)(octet & 0x80u) != inputStatus)
              {

                     // if the current amount ist too big
                     if (!(*currentAmount < 0xFFFFFFFFFFFFFFFFull)) 
                            // break the computation
                            break;                  

                     // update the current amount
                     *currentAmount += 1; 
                     
                     // if we have to stop early
                     if (earlyStop)  

                            // break the computation
                            break; 

                     // toggle the input status
                     inputStatus = !inputStatus,    

                            // set the current amount to the data begin
                         currentAmount = dataBegin; 
              }

              else
                     // increment the current amount
                     ++currentAmount;

              // shift the octet by one to the left
              octet <<= 1u,               
                     
                     // decrement the input stream counter
                  inputStreamCounter -= 1u; 
              
              // if the input stream counter is not zero and the specification is not fulfilled
              if (inputStreamCounter == 0u and !specificationFulfilled) 
              {
                     // set the octet to the next occurence in
                     // the input stream
                     octet = inputStream.get(), 
                         // shift the input counter 3 bits to the left (x8)
                         inputStreamCounter = inputStream.gcount() << 3u;

                     // if the input stream counter is 0
                     if (inputStreamCounter == 0u)

                            // set the octet to 0
                            octet = 0u,

                            // set the input statue to true

                                inputStatus = true,

                                // set the early stop to true
                                earlyStop = true;

                     if (specification < inputStreamCounter)

                            // reset the input stream counter
                            inputStreamCounter = specification,

                            // the specification is fulfilled
                                specificationFulfilled = true;
                     else
                            // update the specification
                            specification -= inputStreamCounter;
              }
       }

       // return the ground distribution
       return theGroundDistribution;
}
