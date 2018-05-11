#include "BinningStructure.h"

template<> void BinningStructure<int>::checkValidity()
{
  if ( size > (max - min + 1) ) {
    std::cerr << "Error un Utility::BinningStructure::checkValidity" << std::endl;
    std::cerr << "For integer binning structures, the size must be less than 1 + max - min" << std::endl;
    throw;
  }
  else if ( (1 + max - min)  % size != 0) {
    std::cerr << "Error un Utility::BinningStructure::checkValidity" << std::endl;
    std::cerr << "For integer binning structures, the size must be divisable by 1 + max - min" << std::endl;
    throw;
  }
};

