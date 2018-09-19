#include "BinningStructure.h"

template<> void BinningStructure<int>::checkValidity()
{
  if ( size > ( max - min) ) {
    std::cerr << "Error un Utility::BinningStructure::checkValidity" << std::endl;
    std::cerr << "For integer binning structures, the size must be less than max - min" << std::endl;
    std::cerr << "max: " << max << std::endl << "min: " << min << std::endl << " size: " << size << std::endl;
    throw std::runtime_error("TypeError");
  }
  else if ( (max - min)  % size != 0) {
    std::cerr << "Error un Utility::BinningStructure::checkValidity" << std::endl;
    std::cerr << "For integer binning structures, the size must be divisable by max - min" << std::endl;
    std::cerr << "max: " << max << std::endl << "min: " << min << std::endl << "size: " << size << std::endl;
    throw std::runtime_error("TypeError");
  }
};
