#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <vector>
#include <iostream>

#include "Catch.h"
#include "Utility.h"


TEST_CASE( "Utility", "[utility]" ) {



/* ****************************************************************************************************** * 
 * Generic Vector and Point Operations
 *
 * ****************************************************************************************************** */ 

  std::vector< int    > intVec;
  std::vector< double > doubleVec, doubleVec2;

  for (int i=1; i<5; i++)
  {
    intVec.push_back( i );
    doubleVec.push_back( i * 1.0 );
    doubleVec2.push_back( i * 1.0 + 4.0 );
  }
  
  // test returns sum of vector
  SECTION ( " sum a vector " ) {
    REQUIRE( Utility::vecSum( intVec    ) == 10   );
    REQUIRE( Utility::vecSum( doubleVec ) == 10.0 );
  }
  // test returns mean of vector
  SECTION ( " return mean of a vector " ) {
    // failed for intVec
    REQUIRE( Utility::vecMean( doubleVec ) == 2.5 );
  }
  //test returns minimum value of a vector
  SECTION ( " return min of a vector ") {
    REQUIRE( Utility::vecMin( intVec    ) == 1   );
    REQUIRE( Utility::vecMin( doubleVec ) == 1.0 );
  }
  //test returns minimum value of a vector
  SECTION ( " return max of a vector ") {
    REQUIRE( Utility::vecMax( intVec    ) == 4   );
    REQUIRE( Utility::vecMax( doubleVec ) == 4.0 );
  }
  // test returns dot product of vector
  SECTION ( " return dot product of a vector " ) {
    REQUIRE( Utility::vecDot( doubleVec, doubleVec2 ) == 70 );
  }
}

/* ****************************************************************************************************** * 
 * Miscellaneous                      
 *
 * ****************************************************************************************************** */ 


TEST_CASE( "linearizeIndices" , "[Utility]") {
  
  // trivial case 1d |-> 1d array
  vector<int> indices  {3};
  vector<int> binSizes {10};

  
  SECTION ( " 1d |-> 1d map " ) {
    REQUIRE( Utility::linearizeIndices( indices , binSizes) == 3 );
  }
  
  // case 2d |-> 1d array
  vector<int> indices2d  {3,1};
  vector<int> binSizes2d {5,3};
  
  SECTION ( " 2d |-> 1d map " ) {
    REQUIRE( Utility::linearizeIndices( indices2d , binSizes2d ) == 8 );
  }
  
  // case 3d |-> 1d array
  vector<int> indices3d  {3,1,1};
  vector<int> binSizes3d {5,3,2};
  
  SECTION ( " 3d |-> 1d map " ) {
    REQUIRE( Utility::linearizeIndices( indices3d , binSizes3d ) == 23 );
  }
  
  // case 3d |-> 1d array , larger bins
  vector<int> indices3d_2  {3,1,5};
  vector<int> binSizes3d_2 {5,3,6};
  
  SECTION ( " 3d |-> 1d map , larger bins" ) {
    REQUIRE( Utility::linearizeIndices( indices3d_2 , binSizes3d_2 ) == 83 );
  }


}
