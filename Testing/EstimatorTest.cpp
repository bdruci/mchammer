#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <tgmath.h> 
#include "../Catch.h"
#include "../Estimator.h"

bool close(double a , double b) {
    return(fabs(a - b) < 0.0001);
};


TEST_CASE( "Estimator scoring and score in ", "[Estimator]" ) {

    Estimator est;

    est.score(1.0);
    est.score(1.0);

    // score does the thing right
    SECTION ( " test score " ) {
      REQUIRE( close( est.getCurrentHistTally() ,  2.0));
    }
    
    est.endHist();
    est.score(1.0);
    est.score(1.0);
    est.score(1.0);
    
    // score does the thing right
    SECTION ( " test score again" ) {
      REQUIRE( est.getCurrentHistTally() ==  3.0);
    }

    est.endHist();
    est.score(1.0);
    est.score(1.0);
    est.endHist();
    
    double a = est.getScalarEstimator(3).first;
    double b = est.getScalarEstimator(3).second;
    
    // history tallies work
    SECTION ( " test flux " ) {
      REQUIRE( close( a , 2.3333) );
    }

    SECTION ( " test uncertainty " ) {
      REQUIRE( close( b , 0.47140) );
    }
  
}

TEST_CASE( "collision estimator std dev", "[Estimator]" ) {

    Estimator est2;
    est2.score(0.1);
    est2.score(0.1);
    est2.endHist();
    est2.score(0.3);
    est2.score(0.8);
    est2.endHist();
    est2.score(1.5);
    est2.score(2.1);
    est2.endHist();
    
    double a = est2.getScalarEstimator(3).first;
    double b = est2.getScalarEstimator(3).second;


    SECTION ( " test uncertainty" ) {
      REQUIRE( close( b , 1.43836) );
    }
  
}



