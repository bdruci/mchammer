#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <stack>

#include "Catch.h"
#include "Reaction.h"
#include "Utility.h"
#include "Random.h"

typedef std::shared_ptr< XSection >  XSec_ptr;
typedef std::shared_ptr< Particle  > Part_ptr;

TEST_CASE( "Capture", "[Reaction]" ) {

	// Make the required objects
	//[XSection]
	std::vector< double > capXS = { 1, 2, 3, 4, 5 };
	CaptureXS CaptureEx(capXS);

	//[Particle bank]
	point dumbyPos(0,0,0);
	point dumbyDir(1,1,1);
	Particle p1(dumbyPos, dumbyDir, 1);
	Particle p2(dumbyPos, dumbyDir, 1);
	Particle p3(dumbyPos, dumbyDir, 1);
	Part_ptr p1_ptr = std::make_shared< Particle >(p1);
	Part_ptr p2_ptr = std::make_shared< Particle >(p2);
	Part_ptr p3_ptr = std::make_shared< Particle >(p3);
	std::stack< Part_ptr > dumbyBank;
	dumbyBank.push(p1_ptr);
	dumbyBank.push(p2_ptr);
	dumbyBank.push(p3_ptr);

	//[Rxn]
    XSec_ptr capt = std::make_shared<CaptureXS>(CaptureEx);
    Capture CaptureRxn(capt);

    // test returns appropriate name
    SECTION ( " return reaction name " ) {
      REQUIRE( CaptureRxn.name() == "Capture" );
    }

    // test the sampling (should just kill the pointer)
    SECTION( " test the sampling" ) {
    	CaptureRxn.sample( p1_ptr, dumbyBank);
    	REQUIRE( !p1_ptr->isAlive() );
    	
    	REQUIRE( p2_ptr->isAlive() );
    	CaptureRxn.sample( p2_ptr, dumbyBank);
    	REQUIRE( !p2_ptr->isAlive() );
    	
    	REQUIRE( p3_ptr->isAlive() );
    	CaptureRxn.sample( p3_ptr, dumbyBank);
    	REQUIRE( !p3_ptr->isAlive() );
    }
}

TEST_CASE( "Scatter", "[Reaction]" ) {

	// Make the required objects
	//[XSection]
	std::vector<double> g1 = {0.33, 0.33, 0.33};
	std::vector<double> g2 = {.5, .5, 0};
	std::vector<double> g3 = {.1, .2, .7};
	std::vector< std::vector<double> > scatXS = { g1, g2, g3 };
	ScatterXS ScatterEx(scatXS);

	//[Particle bank]
	point dumbyPos(0,0,0);
	point dumbyDir(1,1,1);
	Particle p1(dumbyPos, dumbyDir, 1);
	Particle p2(dumbyPos, dumbyDir, 3);
	Particle p3(dumbyPos, dumbyDir, 1);
	Part_ptr p1_ptr = std::make_shared< Particle >(p1);
	Part_ptr p2_ptr = std::make_shared< Particle >(p2);
	Part_ptr p3_ptr = std::make_shared< Particle >(p3);
	std::stack< Part_ptr > dumbyBank;
	dumbyBank.push(p1_ptr);
	dumbyBank.push(p2_ptr);
	dumbyBank.push(p3_ptr);

	//[Rxn]
    XSec_ptr sect = std::make_shared<ScatterXS>(ScatterEx);
    Scatter ScatterRxn(sect);

    // test returns appropriate name
    SECTION ( " return reaction name " ) {
      REQUIRE( ScatterRxn.name() == "Scatter" );
    }

    // test sampling (did the math)
    SECTION ( " scatter p1 once " ) {
    	point correctDir(-0.0648782559, -0.0648782559, 0.9957819158);
    	std::vector<double> testNums = { .5, .5, .75 };
    	activateTesting(testNums);
    	ScatterRxn.sample( p1_ptr, dumbyBank );
    	REQUIRE( p1_ptr->getGroup() ==  2);
    	REQUIRE( Utility::PointEqual(p1_ptr->getDir(), correctDir, .01) );
    	REQUIRE( p1_ptr->getPos() == dumbyPos );
    }
    // test sampling (no rotate)
    SECTION ( " scatter p2 twice " ) {
    	std::vector<double> testNums = { .9, .5, .5 };
    	activateTesting(testNums);
    	ScatterRxn.sample( p2_ptr, dumbyBank );
    	ScatterRxn.sample( p2_ptr, dumbyBank );
    	REQUIRE( p2_ptr->getGroup() ==  3);
    	REQUIRE( Utility::PointEqual(p2_ptr->getDir(), p3_ptr->getDir(), .01) );
    	REQUIRE( p2_ptr->getPos() == dumbyPos );
    }

}

TEST_CASE( "Fission", "[Reaction]" ) {

	// Make the required objects
	//[XSection]
	std::vector<double> fizXS = { 1, 2, 3, 4, 5 };
	std::vector<double> nuXS = { 1, 2, 3, 4, 5 };
	std::vector<double> chiXS = { 1, 2, 3, 4, 5 };
	FissionXS FissionEx(fizXS, nuXS, chiXS);

	//[Particle bank]
	point dumbyPos(0,0,0);
	point dumbyDir(1,1,1);
	Particle p1(dumbyPos, dumbyDir, 1);
	Particle p2(dumbyPos, dumbyDir, 1);
	Particle p3(dumbyPos, dumbyDir, 1);
	Part_ptr p1_ptr = std::make_shared< Particle >(p1);
	Part_ptr p2_ptr = std::make_shared< Particle >(p2);
	Part_ptr p3_ptr = std::make_shared< Particle >(p3);
	std::stack< Part_ptr > dumbyBank;
	dumbyBank.push(p1_ptr);
	dumbyBank.push(p2_ptr);
	dumbyBank.push(p3_ptr);

	//[Rxn]
    XSec_ptr fizPtr = std::make_shared<FissionXS>(FissionEx);
    Fission FissionRxn(fizPtr);

    // test returns appropriate name
    SECTION ( " return reaction name " ) {
      REQUIRE( FissionRxn.name() == "Fission" );
    }

    // test the sampling (should just kill the pointer the way it is written)
    SECTION( " test the sampling" ) {
    	FissionRxn.sample( p1_ptr, dumbyBank);
    	REQUIRE( !p1_ptr->isAlive() );
    	
    	REQUIRE( p2_ptr->isAlive() );
    	FissionRxn.sample( p2_ptr, dumbyBank);
    	REQUIRE( !p2_ptr->isAlive() );
    	
    	REQUIRE( p3_ptr->isAlive() );
    	FissionRxn.sample( p3_ptr, dumbyBank);
    	REQUIRE( !p3_ptr->isAlive() );
    }
}
