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

    std::string name = "SimpleXCyl";

    xCylinder SimpleXCyl( name, y0, z0, rad);
    yCylinder SimpleYCyl( name, x0, z0, rad);
    zCylinder SimpleZCyl( name, x0, y0, rad);

    // test returns appropriate name
    SECTION ( " return surface name " ) {
      REQUIRE( SimpleXCyl.name() == name );
      REQUIRE( SimpleYCyl.name() == name );
      REQUIRE( SimpleZCyl.name() == name );
    }

    // test evaluation for a single point outside cylinder
    SECTION ( " evaluation outside cylinder " ) {
      point p( -1.0, -1.0, -1.0 );
      double eval_result = 4.0;
      REQUIRE( SimpleXCyl.eval(p) == Approx( eval_result ) );
    }

    // test evaluation for a single point inside cylinder
    SECTION ( " evaluation inside cylinder " ) {
      point p( 0.0, 0.0, 0.0 );
      double eval_result = -2.0;
      REQUIRE( SimpleXCyl.eval(p) == Approx( eval_result ) );
    }    

    // test evaluation for a single point on cylinder
    SECTION ( " evaluation on cylinder " ) {
      point p( 25.0, 3.0, 1.0 );
      double eval_result = 0.0;
      REQUIRE( SimpleXCyl.eval(p) == Approx( eval_result ) );
    }

    // test 1000 random points in box from -10 to 10
    SECTION ( " random points near cylinder " ) {
      bool rand_test_result = true;
      for ( unsigned i = 0 ; i < 1000 ; i++ ) {
        point  p( 20.0 * Urand() - 10.0, 20.0 * Urand() - 10.0, 20.0 * Urand() - 10.0 );
        double eval_result = pow( p.y - y0, 2 ) + pow( p.z - z0, 2 ) - rad*rad;
        rand_test_result = rand_test_result && ( SimpleXCyl.eval(p) == eval_result );
      }
      REQUIRE( rand_test_result );
      for ( unsigned i = 0 ; i < 1000 ; i++ ) {
        point  p( 20.0 * Urand() - 10.0, 20.0 * Urand() - 10.0, 20.0 * Urand() - 10.0 );
        double eval_result = pow( p.x - x0, 2 ) + pow( p.z - z0, 2 ) - rad*rad;
        rand_test_result = rand_test_result && ( SimpleYCyl.eval(p) == eval_result );
      }
      REQUIRE( rand_test_result );
      for ( unsigned i = 0 ; i < 1000 ; i++ ) {
        point  p( 20.0 * Urand() - 10.0, 20.0 * Urand() - 10.0, 20.0 * Urand() - 10.0 );
        double eval_result = pow( p.x - x0, 2 ) + pow( p.y - y0, 2 ) - rad*rad;
        rand_test_result = rand_test_result && ( SimpleZCyl.eval(p) == eval_result );
      }
      REQUIRE( rand_test_result );
    }

    // test smallest positive distance, one intersection
    SECTION ( " distance one intersection " ) {
      point p( 156.0, 1.0, 1.0 );
      point d( 0.0, 0.0, -1.0 );
      double eval_result = 2.0;
      REQUIRE( SimpleXCyl.distance( p, d ) == Approx( eval_result ) );
    } 

    // test smallest positive distance, two intersections
    SECTION ( " distance two intersections " ) {
      point p( -4567.0, 5.0, 1.0 );
      point d( 0.0, -1.0, 0.0 );
      double eval_result = 2.0;
      REQUIRE( SimpleXCyl.distance( p, d ) == Approx( eval_result ) );
    } 

    // test smallest positive distance, no intersections (negative roots)
    SECTION ( " distance no intersections (negative roots) " ) {
      point p( 0.0, 10.0, 10.0 );
      point d( 0.0, 1.0 / std::sqrt(2), 1.0 / std::sqrt(2) );
      double eval_result = std::numeric_limits<double>::max();
      REQUIRE( SimpleXCyl.distance( p, d ) == Approx( eval_result ) );
    } 

    // test smallest positive distance, no intersections (complex roots)
    SECTION ( " distance no intersections (complex roots) " ) {
      point p( 0.0, 10.0, 10.0 );
      point d( 0.0, 1.0, 0.0 );
      double eval_result = std::numeric_limits<double>::max();
      REQUIRE( SimpleXCyl.distance( p, d ) == Approx( eval_result ) );
    } 

    // test smallest positive distance, no intersections (// to cylinder)
    SECTION ( " distance no intersections (// to cylinder) " ) {
      point p( 0.0, 10.0, 10.0 );
      point d( 1.0, 0.0, 0.0 );
      double eval_result = std::numeric_limits<double>::max();
      REQUIRE( SimpleXCyl.distance( p, d ) == Approx( eval_result ) );
      REQUIRE( SimpleYCyl.distance( p, d ) == Approx( eval_result ) );
      REQUIRE( SimpleZCyl.distance( p, d ) == Approx( eval_result ) );
    } 

    // test smallest positive distance, infinte intersections (// to cylinder)
    SECTION ( " distance w/ line on cylinder " ) {
      point p( 0.0, 1.0, 3.0 );
      point d( 1.0, 0.0, 0.0 );
      double eval_result = 0;
      REQUIRE( SimpleXCyl.distance( p, d ) == Approx( eval_result ) );
    } 
    
    // test getNormal
    SECTION ( " normal vec of cylinder " ) {
      point p( 0.0, 1.0, 3.0 );
      point eval_result( 0, 0, 1 );

      point actual( SimpleXCyl.getNormal(p) );

      REQUIRE( actual.x == Approx( eval_result.x ) );
      REQUIRE( actual.y == Approx( eval_result.y ) );
      REQUIRE( actual.z == Approx( eval_result.z ) );
    } 

    // test getNormal
    SECTION ( " normal vec of cylinder " ) {
      point p( 0.0, -1.0, 1.0 );
      point eval_result( 0, -1, 0 );

      point actual( SimpleXCyl.getNormal(p) );

      REQUIRE( actual.x == Approx( eval_result.x ) );
      REQUIRE( actual.y == Approx( eval_result.y ) );
      REQUIRE( actual.z == Approx( eval_result.z ) );
    } 

    // test getNormal, point not on the cylinder
    SECTION ( " normal vec of cylinder - returns nullptr " ) {
      point p( 1.0, 1.0, 1.0 );
      point eval_result( 0, 0, 0 );
      point actual3(SimpleYCyl.getNormal( p ) );
      REQUIRE( actual3.x == Approx( eval_result.x ) );
      REQUIRE( actual3.y == Approx( eval_result.y ) );
      REQUIRE( actual3.z == Approx( eval_result.z ) );
    } 

    // test getNormal, complex result
    //When the complex point is evaluated it is non-zero by a rounding error 
    //IDK hown to get around it so this test case will always fail
    x0 = 0;
    y0 = 0;
    rad = 1;
    std::string n = "forNorm";
    zCylinder forNorm( n, x0, y0, rad );

    SECTION ( " normal vec of cylinder " ) {

      point p9( 1.0 / std::sqrt(2), 1.0 / std::sqrt(2), 0.0 );

      point eval_result( 1.0 / std::sqrt(2), 1.0 / std::sqrt(2), 0.0 );

      point actual2( forNorm.getNormal(p9) );
    
      REQUIRE( actual2.x == Approx( eval_result.x ) );
      REQUIRE( actual2.y == Approx( eval_result.y ) );
      REQUIRE( actual2.z == Approx( eval_result.z ) );
    } 


}
