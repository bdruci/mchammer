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
		rng = activateTesting(testVec);
		REQUIRE( rng->getMode() == "Testing" );
		for(int i = 1; i < 6; ++i){
			REQUIRE(rng->Urand() == i);
		}
		for(int i = 1; i < 6; ++i){
			REQUIRE(rng->Urand() == i);
		}
		for(int i = 1; i < 6; ++i){
			REQUIRE(rng->Urand() == i);
		}
	}

	SECTION ( " Activate twice and loop through ") {

		std::vector<double> testVec1 = {5,4,3,2,1};
		std::vector<double> testVec2 = {1,2,3,4,5};

		rng = activateTesting(testVec1);
		REQUIRE( rng->getMode() == "Testing" );

		for(int i = 5; i > 0; --i){
			REQUIRE(rng->Urand() == i);
		}

		rng = activateTesting(testVec2);
		REQUIRE( rng->getMode() == "Testing" );

		for(int i = 1; i < 6; ++i){
			REQUIRE(rng->Urand() == i);
		}
	}

	SECTION ( " test deactivateTesting using modified \"old test\" " ) {
		rng = deactivateTesting();
      	REQUIRE(rng->RN_test_basic());
    }
}
