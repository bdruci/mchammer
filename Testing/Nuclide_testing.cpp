#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <tgmath.h> 
#include "../Catch.h"
#include "../Nuclide.h"
#include "../Reaction.h"

typedef std::shared_ptr< XSection > XSec_ptr;
typedef std::shared_ptr< Reaction > React_ptr;

TEST_CASE( " one XSection  " , "[Nuclide]" ) {

	//Construct simple CaptureXS object
	std::vector< double > capXS = { 1, 2, 3, 4, 5 };
	CaptureXS CaptureEx(capXS);
	XSec_ptr capt = std::make_shared<CaptureXS>(CaptureEx);

	Capture dumbyRxn(capt);
 	React_ptr rxn_ptr = std::make_shared<Capture>(dumbyRxn);
    CaptureEx.setRxn(rxn_ptr);


	//Construct the Dumby Nuclide
	Nuclide Dumby("Dumby");
	Dumby.addXSection(capt);

  	SECTION(" Test getters ") {

    	REQUIRE(Dumby.name() == "Dumby");

    	std::vector< XSec_ptr > CorrectxSections = { capt };
    	REQUIRE(Dumby.getXSections() == CorrectxSections);

    	REQUIRE(Dumby.getTotalXS(3) == 3);

    	REQUIRE(Dumby.getXS(3, "Capture") == 3);
 	}

 	SECTION( " Test basic sampling "){
 		//REQUIRE(Dumby.sampleReaction(1) == nullptr );

 		//Make an actual reaction and set it to captureXS' rxn so it can be sampled

     	//Double check that I made everything correctly
     	React_ptr returned = CaptureEx.getRxn();
      	REQUIRE( returned->name() == "Capture");

      	std::cout << "About to enter" << std::endl;
     	REQUIRE(Dumby.sampleReaction(1) == rxn_ptr );
 	}

}

