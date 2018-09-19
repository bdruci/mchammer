#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <tgmath.h> 
#include <memory>

#include "../Catch.h"
#include <cassert>
#include "../EstimatorCollection.h"

// NOTE: in Transport(), EstimatorCollection::endHist() would be called if and only if a source
// particle and all it's children die out. For the sake of simplicity, this is not reproduced in the 1st two test cases.
// The tests are simplified so the math works out anyways (e.g. only scoring 1)
//
// Each estimator could keep track of nHist (e.g. ++1 at each endHist() call), but it's a much smaller footprint on memory and time 
// to have nHist be a static const int in EstimatorCollection, which is the current implementation. 
//
// This does, however, make it more annoying to test properly.
// 
// In any case, the 1st two test cases are primarily to test the binning structure, and to make sure the correct value
// is being scored in the correct location. The calculation of standard deviation is already being tested in EstimatorTest.cpp,
// and it would be out of scope to test it again here. 

bool assertClose(double a , double b ) {
  double eps = 1E-10; 
    //std::cout << "expansion: " << a << " == " << b << std::endl;
    //std::cout << fabs(a - b) << "/" << eps <<std::endl;
  try {
    assert( fabs(a - b) < eps  );
    return(true);
  }
  catch(...) {
    return(false);
  }

} 


TEST_CASE( "group binning structure", "[EstimatorCollection]" ) {
  // create a group binning structure
  Bin_ptr bin = std::make_shared<GroupBinningStructure>(10);

  // create an attribute map with the correct binning structure
  std::map< ParticleAttribute , Bin_ptr > attributeMap;
  attributeMap[ ParticleAttribute::group ] = bin;

  // create an estimator collection of the collison type
  // the number of source particles is constant at 4, no mater how many times endHist is called
  EstimatorCollection e( "test1" , attributeMap , EstimatorCollection::EstimatorType::Collision , 4);
  e.setGeometricDivisor(1.0);

  SECTION( "Type test ") {
    REQUIRE( e.getType() == EstimatorCollection::EstimatorType::Collision );
  }
  
  point pt(0,0,0);
  point dir(0,1,0);
  Particle particle(pt , dir , 4);
  std::shared_ptr<Particle> p = std::make_shared<Particle>(particle);
  
  vector<unsigned int> ig4 {3};
  
  // no scoring
  SECTION( "flux test ") {
    REQUIRE( e.checkEstimator(ig4) == 0 );
  }
  
  SECTION( "uncertainty test ") {
    REQUIRE( e.checkUncertainty(ig4) == 0 );
  }

  // score in group 4, 4 times over 4 source particles, with mean 1 and pop std dev 1
  e.score(p , 1.0);
  e.score(p , 1.0);
  e.endHist();
  e.score(p , 1.0);
  e.score(p , 1.0);
  e.endHist();


  SECTION( "flux test ") {
    REQUIRE( e.checkEstimator(ig4) == 1 );
  }
  
  SECTION( "uncertainty test ") {
    REQUIRE( e.checkUncertainty(ig4) == 1 );
  }

  p->setGroup(5); // move particle to group 5
  // score in group 5, 3 times over 4 source particles, with mean 0.75 and pop stddev 0.82916
  e.score(p , 1.0);
  e.score(p , 1.0);
  e.endHist();
  e.score(p , 1.0);
  e.endHist();
  
  // check to see if last group is unchanged
  SECTION( "flux test after scoring in different group") {
    REQUIRE( e.checkEstimator(ig4) == 1 );
  }
  
  SECTION( "uncertainty test after scoring in different group") {
    REQUIRE( e.checkUncertainty(ig4) == 1 );
  }
  
  vector<unsigned int> ig5 {4};

  // check to make sure new group is correct
  SECTION( "flux test w/ variance") {
    REQUIRE( e.checkEstimator(ig5) == 0.75 );
  }
  
  SECTION( "uncertainty test w/ variance ") {
    REQUIRE( e.checkUncertainty(ig5) == 0.829156197588849962278733184167671670981772136397338399264 );
  }


}

TEST_CASE( "group and collision binning structure", "[EstimatorCollection]" )   {
  // create a group binning structure
  Bin_ptr gbin = std::make_shared<GroupBinningStructure>(10); // 10 groups
  Bin_ptr nbin = std::make_shared<CollisionOrderBinningStructure>(0,4); // bin particles with 0-4 collisions

  // create an attribute map with the correct binning structure
  std::map< ParticleAttribute , Bin_ptr > attributeMap;
  attributeMap[ ParticleAttribute::group          ] = gbin;
  attributeMap[ ParticleAttribute::collisionOrder ] = nbin;

  // create an estimator collection of the collison type
  // with 10 source particles
  EstimatorCollection e( "test2" , attributeMap , EstimatorCollection::EstimatorType::Collision , 10);
  e.setGeometricDivisor(1.0);

  SECTION( "test size ") {
    REQUIRE( e.getSize() == 50 );
  }

  point pt(0,0,0);
  point dir(0,1,0);
  Particle particle(pt , dir , 4);
  std::shared_ptr<Particle> p = std::make_shared<Particle>(particle);

  // score in group 4, score 1 time over 10 source particles, with mean 0.1 and pop std dev 0
  // should score in numCol = 0, as the particle has not collided yet
  e.score(p , 1.0);
  e.endHist();

  vector<unsigned int> g4_nc0 {3, 0};


  SECTION( "flux test, multibinning, g4 nc 0") {
    REQUIRE( e.checkEstimator(g4_nc0) == 0.1 );
  }
  
    SECTION( "uncertainty test, multibinning g4 nc 0 ") {
      REQUIRE(  assertClose( e.checkUncertainty(g4_nc0) , 3.0 / 10.0 ) );
    }

  // increment the particles collision counter twice so it is in ncol = 2
  p->countCollision();
  p->countCollision();

  vector<unsigned int> g4_nc2 {3, 2};
 
  // score 4x, over 2 histories, in group 4, ncol = 2
  e.score(p , 1.0);
  e.endHist();
  e.score(p , 1.0);
  e.endHist();
  

  SECTION( "flux test, multibinning, g4 nc 0 - make sure its unchanged after diff bin scored") {
    REQUIRE( e.checkEstimator(g4_nc0) == 0.1 );
  }
  
  SECTION( "uncertainty test, multibinning g4 nc 0 - make sure its unchanged after diff bin scored") {
    REQUIRE(  assertClose( e.checkUncertainty(g4_nc0) , 0.3) );
  }
  
  SECTION( "flux test, multibinning, g4 nc 2") {
    REQUIRE( e.checkEstimator(g4_nc2) == 0.2 );
  }
  
  SECTION( "uncertainty test, multibinning g4 nc 2") {
    REQUIRE( e.checkUncertainty(g4_nc2) == 0.4  );
  }

}

TEST_CASE("run like a simulated 2 speed transport" , "[EstimatorCollection]" ) {
  int numSource = 500;
  int numGroups = 2;
  int minCol = 0; 
  int maxCol = 12;

  // construct an estimator collection
  // create a group binning structure
  Bin_ptr gbin = std::make_shared<GroupBinningStructure>(numGroups); // 2 groups -> 2 bins
  Bin_ptr nbin = std::make_shared<CollisionOrderBinningStructure>(minCol,maxCol); // bin particles with 5 or 6 collisions

  // create an attribute map with the correct binning structure
  std::map< ParticleAttribute , Bin_ptr > attributeMap;
  attributeMap[ ParticleAttribute::group          ] = gbin;
  attributeMap[ ParticleAttribute::collisionOrder ] = nbin;

  // create an estimator collection of the collison type
  // with 100 source particles
  EstimatorCollection e( "test3" , attributeMap , EstimatorCollection::EstimatorType::Collision , numSource);
  e.setGeometricDivisor(1.0);

  //manually set up a counter to keep track of the estimator scoring
  std::vector< vector< std::shared_ptr< Estimator > > > manual;

  std::vector< std::shared_ptr< Estimator > > tmp;
  for (int i = 1; i <= numGroups; ++i) {
    for (int j = minCol; j <= maxCol; ++j) {
      Estimator est;
      tmp.push_back( std::make_shared<Estimator>(est) );
    }
    manual.push_back(tmp);
    tmp.clear();
  }

  SECTION("size" , "[EstimatorCollection]") {
    REQUIRE( numGroups * ( 1 + maxCol - minCol ) == e.getSize() );
  }

  double multiplier;
  int groupIndex;
  int colIndex;

  // iterate through source particles
  for (int i = 0; i < numSource; ++i) {

    // start a particle at (0,0,0) in group 2 
    point pt(0,0,0);
    point dir(0,0,1);
    Particle particle(pt , dir , 1);
    std::shared_ptr<Particle> p = std::make_shared<Particle>(particle);
    
    while( p->isAlive() ) {
     
      // chance of scatter or being absorbed
      if ( Urand() < 0.9 ) {
        // score the estimator collection
        e.score(p , multiplier);
        
        // manually score the manual collection with the same multiplier
        groupIndex = p->getGroup() - 1;
        colIndex   = p->getNumCollisions() - minCol;

        if (colIndex >= 0 and colIndex < 1 + maxCol - minCol ) {
          manual.at(groupIndex).at(colIndex)->score(multiplier);
        }
       
        // scatter with 70% chance of downscatter
        int gf = ( Urand() > 0.3 ? 1 : 2);  
        p->scatter(gf);

        // make a random multiplier, simulating particle weight
        multiplier =  Urand();
      }

      else  {
        p->kill();
      }

    } // end particle life

    // end the histories in the collection
    e.endHist();

    // manually end the histories in the manual tally
    for ( auto vec : manual ) {
      for ( auto est : vec ) {
        est->endHist();
      }
    }

  } // end for loop through source queue

  
  vector <unsigned int> indices {0,0};
  SECTION("compare manual and EstimatorCollection scoring after simulated transport") {
    for (unsigned int i = 0; i < numGroups; ++i) {
      indices.at(0) = i;
      for (unsigned int j = minCol; j <= maxCol; ++j) {
        indices.at(1) = j - minCol;
        REQUIRE( e.checkEstimator(indices)   == manual.at(i).at(j - minCol)->getEstimate( numSource )     );
        REQUIRE( e.checkUncertainty(indices) == manual.at(i).at(j - minCol)->getUncertainty( numSource ) );

      }
    }
  }


}
