#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <iostream>
#include <vector>

#include "../Utility.h"
#include "../Catch.h"
#include "../Distribution.h"
#include "../Random.h"

TEST_CASE( "UniformContinuous", "[distribution]" ) {

  std::vector<double> uniNums = { 1. }; 
  activateTesting(uniNums);

  // test sample for this derived type
  SECTION ( " sample " ) {

    uniformContinuous uni( 0, 1 );

    REQUIRE( uni.sample() == 1);
    }
}

TEST_CASE( "ExponentialContinuous", "[distribution]" ) {

  std::vector<double> expNums = { 1., .5 }; 
  activateTesting(expNums);

  // test sample for this derived type
  SECTION ( " sample " ) {

    exponentialContinuous Exp( 0, 1 );

    REQUIRE( Utility::FloatZero(Exp.sample()));
    REQUIRE( Utility::FloatEqual(Exp.sample(),.6931471806, .001));
    }
}

TEST_CASE( "NormalContinuous", "[distribution]" ) {

  std::vector<double> normNums = { .6065306597, .25, .6065306597, .5}; 
  activateTesting(normNums);

  // test sample for this derived type
  SECTION ( " sample " ) {

    normalContinuous Norm(0);

    REQUIRE( Utility::FloatZero(Norm.sample()));
    REQUIRE( Utility::FloatEqual(Norm.sample(), -1, .01));
    }
}

TEST_CASE( "IsotropicDirection", "[distribution]") {

  // test sample for this derived type
  SECTION ( " sample " ) {
    isotropicDirection randDir;

    std::vector<double> dirNums = { .5, .25 }; 
    activateTesting(dirNums);

    point correct(0,0,1);
    REQUIRE(Utility::PointEqual(randDir.sample(), correct, .001));  
  }
}

TEST_CASE( "CuboidGeometry", "[distribution]") {

  // test sample for this derived type
  SECTION ( " sample " ) {

    cuboidGeometry cuboid( 0, 0, 0, 1, 2, 3 );
    cuboidGeometry cube( 0, 0, 0, 10 );

    std::vector<double> cubeNums = { 1.5 }; 
    activateTesting(cubeNums);

    point correctoid(1,2,3);
    point correct(10,10,10);
    
    REQUIRE(  cuboid.sample() == correctoid);
    REQUIRE(  cube.sample() == correct);
  }
}

TEST_CASE("SphereicalGeometry", "[distribution]") {
    // test sample for this derived type
    SECTION ( " sample sphere " ) {

      std::vector<double> sphereNums = { 0.5, 0.25, 0.75 };
      activateTesting(sphereNums);

      sphericalGeometry sphere( 0, 0, 0, 0, 10 );

      point correct(0,-5,5);
      REQUIRE( sphere.sample() == correct);
    }

    SECTION ( " sample shell " ) {

      std::vector<double> shellNums = { 0.5, 0.5, 0.25 };
      activateTesting(shellNums);

      sphericalGeometry shell( -1, -1, 10, 9, 10 );

      point Correct(8,8,19.5);
      REQUIRE( shell.sample() == Correct);
    }
}

TEST_CASE( "XAnnular", "[distribution]" ) {
    // test sample for this derived type
    SECTION ( " sample " ) {

      std::vector<double> xcylNums = { 1, 0.25, 0.125 };
      activateTesting(xcylNums);

      xAnnularGeometry xCyl(0, 0, 0, 1, 1, 5);

      point correct(1,0,2);
      REQUIRE(Utility::PointEqual(xCyl.sample(), correct, .001)); 
   }
}

TEST_CASE( "YAnnular", "[distribution]" ) {
    // test sample for this derived type
    SECTION ( " sample " ) {

      std::vector<double> ycylNums = { 1, 0.25, 0.125 };
      activateTesting(ycylNums);

      yAnnularGeometry yCyl(0, 0, 0, 1, 1, 5);

      point correct(0,1,2);
      REQUIRE(Utility::PointEqual(yCyl.sample(), correct, .001));  
    }
}

TEST_CASE( "ZAnnular", "[distribution]" ) {
    // test sample for this derived type
    SECTION ( " sample " ) {

      std::vector<double> zcylNums = { 1, 0.25, 0.125 };
      activateTesting(zcylNums);

      zAnnularGeometry zCyl(0, 0, 0, 1, 1, 5);

      point correct(0,2,1);
      REQUIRE(Utility::PointEqual(zCyl.sample(), correct, .001));  
    }
}

TEST_CASE("Cylinder", "[distribution]" ) {
    // test sample for this derived type
    SECTION ( " sample " ) {

      std::vector<double> cylNums = { 1, 0.55, 0.45 };
      activateTesting(cylNums);

      xAnnularGeometry cyl(0, 0, 0, 5, 0, 10);

      point correct(5,1,-1);
      REQUIRE(Utility::PointEqual(cyl.sample(), correct, .001)); 
    }
}

TEST_CASE( "Catagorical", "[distribution]" ) {
    // test sample for this derived type
    SECTION ( " sample " ) {

      std::vector<double> catNums = { .2, .57, .8, .84 };
      activateTesting(catNums);

      std::vector<char> elements = { 'a', 'b', 'c', 'd' };
      std::vector<double> probs = { 3, 4, 3, 2 };

      catagoricalWeighted<char> catDist(elements, probs);
      REQUIRE( catDist.sample() == 'a' );
      REQUIRE( catDist.sample() == 'b' );
      REQUIRE( catDist.sample() == 'c' );
      REQUIRE( catDist.sample() == 'd' );
    }
}