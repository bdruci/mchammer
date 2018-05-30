#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "Catch.h"
#include "XSection.h"
#include "Utility.h"
#include "Reaction.h"

/*
NOTES: 
	-Currently there are no protections for indexing out of bounds on getXS 
	 but this gives junk values over seg faults for me.

	 I don't know how much slower .at() is over [] but if easily replaced it might 
	 help with the seg faults
*/

class Reaction;

typedef std::shared_ptr< Reaction > React_ptr;
typedef std::shared_ptr< XSection >  XSec_ptr;

TEST_CASE( "CaptureXS", "[XSection]" ) {
	//Construct simple CaptureXS object
	std::vector< double > capXS = { 1, 2, 3, 4, 5 };
	CaptureXS CaptureEx(capXS);

    // test the parent class fxns
    SECTION ( " test captureXS parent class fxns " ) {
      REQUIRE( CaptureEx.name() == "Capture" );
      REQUIRE( CaptureEx.getRxn() == nullptr );

      //Dumby variable to test set and get rxn
      XSec_ptr capt = std::make_shared<CaptureXS>(CaptureEx);
      Capture dumbyRxn(capt);
      React_ptr rxn_ptr = std::make_shared<Capture>(dumbyRxn);

      CaptureEx.setRxn(rxn_ptr);
      React_ptr returned = CaptureEx.getRxn();
      REQUIRE( returned->name() == "Capture");
    }

    // test correct group is returned
    SECTION ( " return captureXS by group " ) {
      REQUIRE( CaptureEx.getXS(1) == 1 );
      REQUIRE( CaptureEx.getXS(2) == 2 );
      REQUIRE( CaptureEx.getXS(3) == 3 );
      REQUIRE( CaptureEx.getXS(4) == 4 );
      REQUIRE( CaptureEx.getXS(5) == 5 );
    }

    // test for seg fault on getXS
    SECTION ( " return captureXS by group (indexing out of bounds) " ) {
      double dumby = CaptureEx.getXS(100);
      CaptureEx.getXS(0);
    }
    
    //Test the get xs vector
    SECTION ( " return captureXS vector " ) {
      std::vector<double> returned = CaptureEx.getCaptureXS();
      REQUIRE( capXS.size() == returned.size() );
      for(int i = 0; i < capXS.size(); ++i){
      	REQUIRE( Utility::FloatEqual(capXS[i], returned[i], .01) );
      }
    }
}

TEST_CASE( "ScatterXS", "[XSection]" ) {
	//Construct simple ScatterXS object
	std::vector<double> g1 = {0.5, 0, 0.25};
	std::vector<double> g2 = {2, 4, 8};
	std::vector<double> g3 = {1.618, 3.142, 1.414};
	std::vector< std::vector<double> > scatXS = { g1, g2, g3 };
	ScatterXS ScatterEx(scatXS);

    // test the parent class fxns
    SECTION ( " test scatterXS parent class fxns " ) {
      REQUIRE( ScatterEx.name() == "Scatter" );
      REQUIRE( ScatterEx.getRxn() == nullptr );

      //Dumby variable to test set and get rxn
      XSec_ptr scatt = std::make_shared<ScatterXS>(ScatterEx);
      Scatter dumbyRxn(scatt);
      React_ptr rxn_ptr = std::make_shared<Scatter>(dumbyRxn);

      ScatterEx.setRxn(rxn_ptr);
      React_ptr returned = ScatterEx.getRxn();
      REQUIRE( returned->name() == "Scatter");
    }

    // test correct group is returned
    SECTION ( " return scatterXS by group " ) {
      REQUIRE( ScatterEx.getXS(1) == .75 );
      REQUIRE( ScatterEx.getXS(2) == 14 );
      REQUIRE( Utility::FloatEqual(ScatterEx.getXS(3), 6.174, .001) );
    }

     // test for seg fault on getXS
    SECTION ( " return scatterXS by group (indexing out of bounds) " ) {
      ScatterEx.getXS(100);
      ScatterEx.getXS(0);
    }

    //Test the get xs vector
    SECTION ( " return scatterXS vector " ) {
      std::vector< std::vector<double> > returned = ScatterEx.getScatterXS();
      REQUIRE( scatXS.size() == returned.size() );
      for(int i = 0; i < scatXS.size(); ++i){
      	for(int j = 0; j < scatXS[i].size(); ++j)
      	REQUIRE( Utility::FloatEqual(scatXS[i][j], returned[i][j], .001) );
      }
    }

    //Test the get xs totals 
    SECTION ( " return scatterXS totals " ) {
      std::vector<double> returned = ScatterEx.getScatterTotalXS();
      std::vector<double> correct = { .75, 14, 6.174 };
      REQUIRE( correct.size() == returned.size() );
      for(int i = 0; i < correct.size(); ++i){
      	REQUIRE( Utility::FloatEqual(correct[i], returned[i], .001) );
      }
    }
}

TEST_CASE( "FissionXS", "[XSection]" ) {
	//Construct simple FissionXS object
	std::vector<double> fizXS = { 1, 2, 3, 4, 5 };
	std::vector<double> nuXS = { 1, 2, 3, 4, 5 };
	std::vector<double> chiXS = { 1, 2, 3, 4, 5 };
	FissionXS FissionEx(fizXS, nuXS, chiXS);

    // test the parent class fxns
    SECTION ( " return fissionXS by group " ) {
      REQUIRE( FissionEx.name() == "Fission" );
      REQUIRE( FissionEx.getRxn() == nullptr );

      //Dumby variable to test set and get rxn
      XSec_ptr fizz = std::make_shared<FissionXS>(FissionEx);
      Fission dumbyRxn(fizz);
      React_ptr rxn_ptr = std::make_shared<Fission>(dumbyRxn);

      FissionEx.setRxn(rxn_ptr);
      React_ptr returned = FissionEx.getRxn();
      REQUIRE( returned->name() == "Fission");
    }

     // test correct group is returned
    SECTION ( " return fissionXS by group " ) {
      REQUIRE( FissionEx.getXS(1) == 1 );
      REQUIRE( FissionEx.getXS(2) == 2 );
      REQUIRE( FissionEx.getXS(3) == 3 );
      REQUIRE( FissionEx.getXS(4) == 4 );
      REQUIRE( FissionEx.getXS(5) == 5 );
    }

     // test for seg fault on getXS
    SECTION ( " return fissionXS by group (indexing out of bounds) " ) {
      FissionEx.getXS(10);
      FissionEx.getXS(0);
    }

    // test the nu get
    SECTION ( " test the nu get " ) {
      std::vector<double> returned = FissionEx.getNu();
      REQUIRE( nuXS.size() == returned.size() );
      for(int i = 0; i < nuXS.size(); ++i){
      	REQUIRE( nuXS[i] == returned[i] );
      }
    }

    // test the chi get
    SECTION ( " test the chi get " ) {
      std::vector<double> returned = FissionEx.getChi();
      REQUIRE( chiXS.size() == returned.size() );
      for(int i = 0; i < chiXS.size(); ++i){
      	REQUIRE( chiXS[i] == returned[i] );
      }
    }

    // test the fissionXS vector get
    SECTION ( " test the vector fissionXS get " ) {
      std::vector<double> returned = FissionEx.getFissionXS();
      REQUIRE( fizXS.size() == returned.size() );
      for(int i = 0; i < fizXS.size(); ++i){
      	REQUIRE( fizXS[i] == returned[i] );
      }
    }
}