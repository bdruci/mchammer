#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "Catch.h"
#include "Random.h"
#include <vector>

#define    ULONG  unsigned long long

TEST_CASE( "Rand", "[Random]" ) {

  SECTION ( " test Urand using modified \"old test\" " ) {
  REQUIRE(rng->RN_test_basic());
  }
}

TEST_CASE( "Testing", "[Random]") {

  SECTION ( " Activate and loop through a few times") {
    std::vector<double> testVec = {1,2,3,4,5};
    ReturnSetNums testClass( testVec );
    rng = &testClass;
    
    REQUIRE( rng->getMode() == "Testing" );
    for(int i = 1; i < 6; ++i)
    {
      REQUIRE(rng->Urand() == i);
    }
    for(int i = 1; i < 6; ++i)
    {
      REQUIRE(rng->Urand() == i);
    }
    for(int i = 1; i < 6; ++i)
    {
      REQUIRE(rng->Urand() == i);
    }
  }

  SECTION ( " Activate twice and loop through ") {

    std::vector<double> testVec1 = {5,4,3,2,1};
    std::vector<double> testVec2 = {1,2,3,4,5};

    ReturnSetNums testClass1( testVec1 );
    rng = &testClass1;
    REQUIRE( rng->getMode() == "Testing" );

    for(int i = 5; i > 0; --i)
    {
      REQUIRE(rng->Urand() == i);
    }

    ReturnSetNums testClass2( testVec2 );
    rng = &testClass2;
    REQUIRE( rng->getMode() == "Testing" );

    for(int i = 1; i < 6; ++i)
    {
      REQUIRE(rng->Urand() == i);
    }
  }
}
