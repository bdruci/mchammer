#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <tgmath.h> 
#include "../Catch.h"
#include "../BinningStructure.h"

// we really only care about int and double structures right now
TEST_CASE( "BinningStructure with doubles, non-strict indexing", "[Utility]" ) {
    
    BinningStructure< double > bin(11.232 , 11.238 , 10 , false);

    SECTION ( " test getSize" ) {
      REQUIRE(  bin.getSize() == 10 );
    }
  
    SECTION( "test getIndex with in range value" ) {
      REQUIRE( bin.getIndex(11.23789) == 9 );
    }
    
    SECTION( "test getIndex with out of range value" ) {
      REQUIRE( bin.getIndex(11.2380001) == -1 );
    }
    
    SECTION( "test getIndex with negative out of range value" ) {
      REQUIRE( bin.getIndex(-12) == -1 );
    }
    
    SECTION( "test getIndex with 0 out of range value" ) {
      REQUIRE( bin.getIndex(0) == -1 );
    }
  
}

TEST_CASE( "BinningStructure with doubles, negative values, non-strict indexing", "[Utility]" ) {
    
    BinningStructure< double > bin( -7 , 7 , 150 , false);

    SECTION ( " test getSize" ) {
      REQUIRE(  bin.getSize() == 150 );
    }
    
    SECTION( "test getIndex with in range negative value" ) {
      REQUIRE( bin.getIndex(-7) == 0 );
    }
    
    SECTION( "test getIndex with in range positive value" ) {
      REQUIRE( bin.getIndex(7) == -1 );
    }
  
    SECTION( "test getIndex with in range negative value" ) {
      REQUIRE( bin.getIndex(-6) == 10 );
    }
    
    SECTION( "test getIndex with out of range negative value" ) {
      REQUIRE( bin.getIndex(-7.01) == -1 );
    }
    
    SECTION( "test getIndex with value testing bin boundary fidelity") {
      REQUIRE( bin.getIndex(-0.001) == 74);
    }
    
    SECTION( "test getIndex with in range 0") {
      REQUIRE( bin.getIndex(0) == 75);
    }
  
}

TEST_CASE( "BinningStructure with int, non-strict indexing", "[Utility]" ) {
    
    BinningStructure< int > bin(12 , 23 , 6 , false);

    SECTION( "test getIndex with in range value" ) {
      REQUIRE( bin.getIndex(14) == 1 );
    }
    
    SECTION( "test getIndex with out of range value" ) {
      REQUIRE( bin.getIndex(-1) == -1 );
    }
  
}

TEST_CASE( "BinningStructure with int, max size, non-strict indexing", "[Utility]" ) {
    
    BinningStructure< int > bin(12 , 23 , 12 , false);

    SECTION( "test getIndex with in range value" ) {
      REQUIRE( bin.getIndex(14) == 2 );
    }
    
    SECTION( "test getIndex with out of range value" ) {
      REQUIRE( bin.getIndex(11) == -1 );
    }
  
}

TEST_CASE( "BinningStructure with int, negative values, non-strict indexing", "[Utility]" ) {
    
    BinningStructure< int > bin(-3 , 3 , 7 , false);

    SECTION( "test getIndex with in range negative value" ) {
      REQUIRE( bin.getIndex(-2) == 1 );
    }
    
    SECTION( "test getIndex with in range value 0" ) {
      REQUIRE( bin.getIndex(0) == 3 );
    }
    
    SECTION( "test getIndex with in range value pos value" ) {
      REQUIRE( bin.getIndex(2) == 5 );
    }
    
    SECTION( "test getIndex with out of range value pos value" ) {
      REQUIRE( bin.getIndex(6) == -1 );
    }
    
    SECTION( "test getIndex with out of range value neg value" ) {
      REQUIRE( bin.getIndex(-4) == -1 );
    }
  
}

