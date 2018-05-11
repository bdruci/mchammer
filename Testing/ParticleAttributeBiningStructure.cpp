#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <tgmath.h> 
#include "../Catch.h"
#include "../ParticleAttributeBinningStructure.h"

// we really only care about int and double structures right now
TEST_CASE( "group binning structure class tests", "[ParticleAttributeBinningStructure.h]" ) {

  GroupBinningStructure bin(10);

  SECTION ( "test getsize" ) {
    REQUIRE( bin.getSize() == 10  );
  }
  
  SECTION ( "test getsize" ) {
    REQUIRE( bin.getSize() == 10  );
  }
  
  SECTION ( "test getsize" ) {
    REQUIRE( bin.getSize() == 10  );
  }


}
