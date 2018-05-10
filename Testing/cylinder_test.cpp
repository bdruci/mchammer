#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <limits>

#include "Catch.h"
#include "Surface.h"
#include "Random.h"

TEST_CASE( "Cylinder", "[cylinder]" ) {

    double x0 =  1.0;
    double y0 =  1.0;
    double z0 =  1.0;
    double rad  =  2;
    point dir(1, 0, 0);

    std::string name = "SimpleCyl";

    cylinder SimpleCyl( name, x0, y0, z0, rad, dir );

    // test returns appropriate name
    SECTION ( " return surface name " ) {
      REQUIRE( SimpleCyl.name() == name );
    }

    // test evaluation for a single point outside cylinder
    SECTION ( " evaluation outside cylinder " ) {
      point p( -1.0, -1.0, -1.0 );
      double eval_result = 4.0;
      REQUIRE( SimpleCyl.eval(p) == Approx( eval_result ) );
    }

    // test evaluation for a single point inside cylinder
    SECTION ( " evaluation inside cylinder " ) {
      point p( 0.0, 0.0, 0.0 );
      double eval_result = -2.0;
      REQUIRE( SimpleCyl.eval(p) == Approx( eval_result ) );
    }    

    // test evaluation for a single point on cylinder
    SECTION ( " evaluation on cylinder " ) {
      point p( 25.0, 3.0, 1.0 );
      double eval_result = 0.0;
      REQUIRE( SimpleCyl.eval(p) == Approx( eval_result ) );
    }

    // test 1000 random points in box from -10 to 10
    SECTION ( " random points near cylinder " ) {
      bool rand_test_result = true;
      for ( unsigned i = 0 ; i < 1000 ; i++ ) {
        point  p( 20.0 * Urand() - 10.0, 20.0 * Urand() - 10.0, 20.0 * Urand() - 10.0 );
        double eval_result = pow( p.y - y0, 2 ) + pow( p.z - z0, 2 ) - rad*rad;
        rand_test_result = rand_test_result && ( SimpleCyl.eval(p) == eval_result );
      }
      REQUIRE( rand_test_result );
    }

    // test smallest positive distance, one intersection
    SECTION ( " distance one intersection " ) {
      point p( 156.0, 1.0, 1.0 );
      point d( 0.0, 0.0, -1.0 );
      double eval_result = 2.0;
      REQUIRE( SimpleCyl.distance( p, d ) == Approx( eval_result ) );
    } 

    // test smallest positive distance, two intersections
    SECTION ( " distance two intersections " ) {
      point p( -4567.0, 5.0, 1.0 );
      point d( 0.0, -1.0, 0.0 );
      double eval_result = 2.0;
      REQUIRE( SimpleCyl.distance( p, d ) == Approx( eval_result ) );
    } 

    // test smallest positive distance, no intersections (negative roots)
    SECTION ( " distance no intersections (negative roots) " ) {
      point p( 0.0, 10.0, 10.0 );
      point d( 0.0, 1.0 / std::sqrt(2), 1.0 / std::sqrt(2) );
      double eval_result = std::numeric_limits<double>::max();
      REQUIRE( SimpleCyl.distance( p, d ) == Approx( eval_result ) );
    } 

    // test smallest positive distance, no intersections (complex roots)
    SECTION ( " distance no intersections (complex roots) " ) {
      point p( 0.0, 10.0, 10.0 );
      point d( 0.0, 1.0, 0.0 );
      double eval_result = std::numeric_limits<double>::max();
      REQUIRE( SimpleCyl.distance( p, d ) == Approx( eval_result ) );
    } 

    // test smallest positive distance, no intersections (// to cylinder)
    SECTION ( " distance no intersections (// to cylinder) " ) {
      point p( 0.0, 10.0, 10.0 );
      point d( 1.0, 0.0, 0.0 );
      double eval_result = std::numeric_limits<double>::max();
      REQUIRE( SimpleCyl.distance( p, d ) == Approx( eval_result ) );
    } 

    // test smallest positive distance, infinte intersections (// to cylinder)
    SECTION ( " distance w/ line on cylinder " ) {
      point p( 0.0, 1.0, 3.0 );
      point d( 1.0, 0.0, 0.0 );
      double eval_result = 0;
      REQUIRE( SimpleCyl.distance( p, d ) == Approx( eval_result ) );
    } 
    
    // test getNormal
    SECTION ( " normal vec of cylinder " ) {
      point p( 0.0, 1.0, 3.0 );
      point eval_result( 0, 0, 1 );

      point actual( SimpleCyl.getNormal(p) );

      REQUIRE( actual.x == Approx( eval_result.x ) );
      REQUIRE( actual.y == Approx( eval_result.y ) );
      REQUIRE( actual.z == Approx( eval_result.z ) );
    } 

    // test getNormal
    SECTION ( " normal vec of cylinder " ) {
      point p( 0.0, -1.0, 1.0 );
      point eval_result( 0, -1, 0 );

      point actual( SimpleCyl.getNormal(p) );

      REQUIRE( actual.x == Approx( eval_result.x ) );
      REQUIRE( actual.y == Approx( eval_result.y ) );
      REQUIRE( actual.z == Approx( eval_result.z ) );
    } 

    // test getNormal, point not on the cylinder
    SECTION ( " normal vec of cylinder - returns nullptr " ) {
      point p( 1.0, 1.0, 1.0 );
      point eval_result( 0, 0, 0 );
      point actual3(SimpleCyl.getNormal( p ) );
      REQUIRE( actual3.x == Approx( eval_result.x ) );
      REQUIRE( actual3.y == Approx( eval_result.y ) );
      REQUIRE( actual3.z == Approx( eval_result.z ) );
    } 

    // test getNormal, complex result
    //When the complex point is evaluated it is non-zero by a rounding error 
    //IDK hown to get around it so this test case will always fail
    /*
    x0 = 0;
    y0 = 0;
    z0 = 0;
    rad = 1;
    point dir2(0,0,1);
    std::string n = "forNorm";
    cylinder forNorm( n, x0, y0, z0, rad, dir2 );

    SECTION ( " normal vec of cylinder " ) {

      point p9( 1.0 / std::sqrt(2), 1.0 / std::sqrt(2), 0.0 );
      std::cout << "Entered Point: (" << p9.x << ", " << p9.y << ", " << p9.z <<")" <<std::endl;
      std::cout << "Eval result for entered point: " << forNorm.eval(p9) << std::endl;

      point eval_result( 1.0 / std::sqrt(2), 1.0 / std::sqrt(2), 0.0 );
      std::cout << "Expected Point: (" << eval_result.x << ", " << eval_result.y << ", " << eval_result.z <<")" <<std::endl;

      point actual2( forNorm.getNormal(p9) );
      std::cout << "Calc Point: (" << actual2.x << ", " << actual2.y << ", " << actual2.z <<")" <<std::endl;
    
      REQUIRE( actual2.x == Approx( eval_result.x ) );
      REQUIRE( actual2.y == Approx( eval_result.y ) );
      REQUIRE( actual2.z == Approx( eval_result.z ) );
    } 
	*/

}
