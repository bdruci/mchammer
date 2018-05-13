
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <tgmath.h> 
#include <memory>

#include "../Catch.h"
#include "../EstimatorCollection.h"

TEST_CASE( "group binning structure, collision estimator", "[EstimatorCollection]" ) {
  // create a group binning structure
  Bin_ptr bin = std::make_shared<GroupBinningStructure>(10);
  // create an attribute map with the correct binning structure
  std::map< string , Bin_ptr > attributeMap;
  attributeMap["Group"] = bin;

  // create an estimator collection of the collison type
  EstimatorCollection e( attributeMap , EstimatorCollection::EstimatorType::Collision);

  SECTION( "Type test ") {
    REQUIRE( e.getType() == EstimatorCollection::EstimatorType::Collision );
  }


}
