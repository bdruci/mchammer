/******************************************************************************
 * Filename:  BinningStructure.h
 * Description:
 *  A class template dealing with binning over an attribute of varying data types
 *    - Constructed with a min, max, and integer size
 *    - getIndex is constant lookup time, 
 *        - returns < n , true >, where n is the index where 'value' falls in the binning structure 
 *          if 'value' is in [min , max]
 *        - returns < 0 , false > otherwise
 *    - attributeType should typically be either an int, float or double; must be castable both to/from int
 *    - attributeType must have comparator operators >= and <= overloaded
 *
 * Version: 1.0
 * Author: Kyle Beyer
 * Organization: University of Michigan
 * e-mail: beykyle@umich.edu
 *
 ******************************************************************************/


#include <math.h>
#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
#include <limits>

#ifndef _BINNINGSTRUCTURE_H_
#define _BINNINGSTRUCTURE_H_


template <typename attributeType> class BinningStructure {
  private:
    int  size;
    bool strict;
    attributeType min , max , binWidth;

    // specialized method for checking validity of input parameters
    // throws if input parameters are invalid for attributeType
    void checkValidity() {}

  public:
    // constructor for structure with specified number of bins
    BinningStructure(attributeType minin , attributeType maxin , int sizein , bool strictin): 
      min(minin) , max(maxin) , size(sizein) , binWidth( (max - min) / (static_cast<attributeType>( size )) ) , strict(strictin)  
    { 
      // generic (not type dependent) check for binning structure validity
      if (max <= min ) {
        std::cerr << "Error in BinningStructure::BinningStructure!" << std::endl;
        std::cerr << "Max must be less than min " << std::endl;
        throw;
      }
      
      checkValidity();
    };
    
   ~BinningStructure() {};

    int getSize() { return(size); };
    
    // given a value, finds the index in the binning structure 
    int getIndex(attributeType value) 
    {
      int index;
      if (value <= max and value >= min) {
        return( (int)   floor( size * (value - min) / (max - min) ) );
      }
      else if (strict == false) {
        // if not strict, an index of -1 tells the client the value is outside the binning structure
        return(-1);
      }
      else {
        // if strict, throw an error
        std::cerr << "Error un Utility::BinningStructure::getIndex" << std::endl;
        std::cerr << "When BinningStructure::strict is true, a value outisde the binning structure results in a runtime error" << std::endl;
        throw;
      }
    };
};


// validity check for integer binning structures
template<> void BinningStructure<int>::checkValidity();



#endif
