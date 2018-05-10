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

#ifndef _BINNINGSTRUCTURE_H_
#define _BINNINGSTRUCTURE_H_


template <typename attributeType> class BinningStructure {
  private:
    int  size;
    bool strict;
    attributeType min , max , binWidth;
  public:
    // constructor for structure with specified number of bins
    BinningStructure(attributeType minin , attributeType maxin , int sizein , bool strictin): 
      min(minin) , max(maxin) , size(sizein) , binWidth( (max - min) / (static_cast<attributeType>( size )) ) , strict(strictin)  {};
    // constructor for structure with implicit bins edges at integer values between min and max
    BinningStructure(attributeType minin , attributeType maxin ,  bool strictin): 
      min(minin) , max(maxin) , size(max - min) , binWidth(1) , strict(strictin)  {};
   ~BinningStructure() {};

    int getSize() { return(size); };
    
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
        // throw an error
        std::cerr << "Error un Utility::BinningStructure::getIndex" << std::endl;
        std::cerr << "When BinningStructure::strict is true, a value outisde the binning structure results in a runtime error" << std::endl;
        throw;
      }
    };

};

#endif
