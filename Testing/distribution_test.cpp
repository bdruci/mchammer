#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <iostream>

#include "Utility.h"
#include "Catch.h"
#include "Distribution.h"
#include "Random.h"

TEST_CASE( "Distribution", "[sample]" ) {

    // test sample for this derived type
    SECTION ( " uniformContinuous " ) {

        vector<double> uniNums{ 1. };
        activateTesting( uniNums );

        uniformContinuous uni( 0, 1 );

        REQUIRE( uni.sample() == 1);
    }

    // test sample for this derived type
    SECTION ( " isotropicDirection " ) {
        isotropicDirection randDir;

        vector<double> dirNums{ .5, .25 };
        activateTesting( dirNums );   

        point correct(0,0,1);
        point sample = randDir.sample();
        REQUIRE( sample.x == correct.x);
        REQUIRE( Utility::FloatZero(sample.y));
        REQUIRE( sample.z == correct.z);
    }


    // test sample for this derived type
    SECTION ( " cuboidGeometry " ) {

        vector<double> cubeNums{ 0.1, 0.2, 0.3 };
        activateTesting( cubeNums ); 

        cuboidGeometry cuboid( 0, 0, 0, 1, 2, 3 );
        cuboidGeometry cube( 0, 0, 0, 10 );

        point correctoid(-0.4,-0.6,-0.6);
        point correct(-4,-3,-2);
        
        point coidS = cuboid.sample();
        point cs = cube.sample();
        std::cout << "cuboid sample is: " << coidS.x << " " << coidS.y << " " << coidS.z << std::endl;
        std::cout << "cube sample is: " << cs.x << " " << cs.y << " " << cs.z << std::endl;
        REQUIRE(  coidS == correctoid);
        REQUIRE(  cs == correct);
    }


    // test sample for this derived type
    SECTION ( " sphereicalGeometry " ) {

        vector<double> sphereNums{ 0.5, 0.25, 0.2 };
        activateTesting( sphereNums );

        sphericalGeometry sphere( 0, 0, 0, 0, 10 );

        point correct(0,-5,-6);
        REQUIRE( sphere.sample() == correct);
    }

    // test sample for this derived type
    SECTION ( " xAnnular " ) {

        xAnnularGeometry xCyl(0, 0, 0, 1, 1, 5);

        point correct(1,1,1);
        REQUIRE( xCyl.sample() == correct);
    }

    // test sample for this derived type
    SECTION ( " yAnnular " ) {

        yAnnularGeometry yCyl(0, 0, 0, 1, 1, 5);

        point correct(1,1,1);
        REQUIRE( yCyl.sample() == correct);
    }

    // test sample for this derived type
    SECTION ( " zAnnular " ) {

        zAnnularGeometry zCyl(0, 0, 0, 1, 1, 5);

        point correct(1,1,1);
        REQUIRE( zCyl.sample() == correct);
    }

    // test sample for this derived type
    SECTION ( " cylinder " ) {

        xAnnularGeometry cyl(0, 0, 0, 0, 1, 10);

        point correct(1,1,1);
        REQUIRE( cyl.sample() == correct);
    }

    // test sample for this derived type
    SECTION ( " catagorical " ) {
    }

  }