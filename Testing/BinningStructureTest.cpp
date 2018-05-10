#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <tgmath.h> 
#include "../Catch.h"
#include "../Estimator.h"
#include "../BinningStructure.h"

TEST_CASE( "BinningStructure with doubles, strict indexing", "[Utility]" ) {
    
    BinningStructure< double > bin(11.232 , 11.238 , 10 , false);

    SECTION ( " test getSize" ) {
      REQUIRE(  bin.getSize() == 10 );
    }
  
    SECTION( "test getIndex with in range value" ) {
      std::pair<int , bool> testOut ( 1, true);
      REQUIRE( bin.getIndex(11.23789) == 9 );
    }
    
    SECTION( "test getIndex with out of range value" ) {
      std::pair<int , bool> testOut ( 1, true);
      REQUIRE( bin.getIndex(11.2380001) == -1 );
    }
  
}


