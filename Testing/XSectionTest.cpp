#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <tgmath.h> 
#include "../Catch.h"
#include "../XSection.h"
#include "../Reaction.h"

TEST_CASE( "CaptureXS" , "[XSection]" ) {
  std::vector< double > xsin { 0.1 , 0.2 , 0.6 , 0.8 };
  int ngroups = xsin.size();

  CaptureXS cxs( xsin );

  SECTION(" Test getters ") {
    for( int i = 1; i <= ngroups; ++i) {
      REQUIRE( xsin[i-1] == cxs.getXS(i) );
    }
  
    REQUIRE( xsin == cxs.getCaptureXS() );
  }


}
