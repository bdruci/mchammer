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

/* ****************************************************************************************************** * 
 * Miscellaneous                      
 *
 * ****************************************************************************************************** */ 

  //Test FloatEqual for true (simple)
  SECTION ( " Test FloatEqual for true (simple) " ) {
    double justOff1 = 1 - std::numeric_limits<double>::epsilon();

    REQUIRE( Utility::FloatEqual( 1.0, justOff1, 1E-10));
    REQUIRE( Utility::FloatEqual( -1.0, -1, 1E-10));
    REQUIRE( Utility::FloatEqual( 0, 0, 1));
  }
  //Test FloatEqual for true (complex)
  SECTION ( " Test FloatEqual for true (complex) " ) {
    double justOff2 = ( (1/std::sqrt(2)) * (1/std::sqrt(2)) );

    REQUIRE( Utility::FloatEqual(.5, justOff2, 1E-10));
  }
  //Test FloatEqual for false (simple)
  SECTION ( " Test FloatEqual for false (simple) " ) {
    //REQUIRE( !Utility::FloatEqual( 1, 2, 1E-10));
    REQUIRE( !Utility::FloatEqual( 1, -1, 1E-10));
    REQUIRE( !Utility::FloatEqual( 1, 2, 1E-10));
  }
  //Test FloatEqual for false (complex)
  SECTION ( " Test FloatEqual for false (complex) " ) {
    double cornerNegHalf = -1 - std::numeric_limits<double>::epsilon();
    double tooOff = 1 - ( 600 * std::numeric_limits<double>::epsilon() );

    REQUIRE( !Utility::FloatEqual(cornerNegHalf, 1, 1E-10));
    REQUIRE( !Utility::FloatEqual(tooOff, 1, 1E-20));

  }
  //Test FloatZero overloaded for all
  SECTION ( " Test FloatEqual overloaded for all " ) {
    REQUIRE( !Utility::FloatZero(1));
    REQUIRE( !Utility::FloatZero(-1));
    REQUIRE( Utility::FloatZero(0));
    REQUIRE( !Utility::FloatZero(std::numeric_limits<double>::epsilon() *501));
    REQUIRE( Utility::FloatZero(std::numeric_limits<double>::epsilon() *499));
    REQUIRE( Utility::FloatZero(std::numeric_limits<double>::epsilon() *-499));
  }

  /* ****************************************************************************************************** * 
 * Fast Matrix Operations
 *   Functions for calculating determinants of double-typed matrices
 * ****************************************************************************************************** */ 
  //Test two by two determinant
  SECTION ( " Test 2x2 determinant " ) {
    vector<double> v1 = { 2, 3 };
    vector<double> v2 = { 4, 5 };
    REQUIRE( Utility::twoDeterminant(v1, v2) == -2);
  }

}
