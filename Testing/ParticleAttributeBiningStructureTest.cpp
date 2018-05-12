#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <tgmath.h> 
#include "../Catch.h"
#include "../ParticleAttributeBinningStructure.h"

// we really only care about int and double structures right now
TEST_CASE( "group binning structure class tests", "[ParticleAttributeBinningStructure.h]" ) {

  GroupBinningStructure bin(10);
  
  point pt(0,0,0);
  point dir(0,1,0);
  Particle particle(pt , dir , 4); 

  SECTION ( "test getsize" ) {
    REQUIRE( bin.getSize() == 10  );
  }
  
  SECTION ( "test group binning" ) {
    REQUIRE( bin.getIndex(std::make_shared<Particle>(particle)) == 3  );
  }

  GroupBinningStructure bin2(10 , 5);
  
  SECTION ( "test group binning for 2nd constructor type - out of range above " ) {
    REQUIRE( bin2.getIndex(std::make_shared<Particle>(particle)) == -1  );
  }
  
  particle.setGroup(1);
  
  SECTION ( "test group binning for 2nd constructor type - out of range below " ) {
    REQUIRE( bin2.getIndex(std::make_shared<Particle>(particle)) == -1  );
  }

  particle.setGroup(5);
  
  SECTION ( "test group binning for 2nd constructor type in range " ) {
    REQUIRE( bin2.getIndex(std::make_shared<Particle>(particle)) == 0  );
  }
  
  GroupBinningStructure bin3(10 , 2 , 7);
  particle.setGroup(8);
  
  SECTION ( "test group binning for 3rd constructor type - out of range above " ) {
    REQUIRE( bin3.getIndex(std::make_shared<Particle>(particle)) == -1  );
  }
  
  particle.setGroup(1);
  
  SECTION ( "test group binning for 2nd constructor type - out of range below " ) {
    REQUIRE( bin3.getIndex(std::make_shared<Particle>(particle)) == -1  );
  }

  particle.setGroup(5);
  
  SECTION ( "test group binning for 2nd constructor type in range " ) {
    REQUIRE( bin3.getIndex(std::make_shared<Particle>(particle)) == 3 );
  }

}

TEST_CASE( "Collision order binning structure, single collison case", "[ParticleAttributeBinningStructure.h]" ) {

  CollisionOrderBinningStructure bin(2);
  
  point pt(0,0,0);
  point dir(0,1,0);
  Particle particle(pt , dir , 4); 
  
  particle.countCollision();
  particle.countCollision();
  
  SECTION ( "test collision order binning in range" ) {
    REQUIRE( bin.getIndex(std::make_shared<Particle>(particle)) == 0 );
  }
  
  particle.countCollision();
  
  SECTION ( "test collision order binning out of range" ) {
    REQUIRE( bin.getIndex(std::make_shared<Particle>(particle)) == -1 );
  }
  
  CollisionOrderBinningStructure bin2(0);
 
  Particle particle2(pt , dir , 4); 

  SECTION ( "test collision order binning in range, for 0th order" ) {
    REQUIRE( bin2.getIndex(std::make_shared<Particle>(particle2)) == 0 );
  }
  
  SECTION ( "test collision order binning out of range, for 0th order" ) {
    REQUIRE( bin2.getIndex(std::make_shared<Particle>(particle)) == -1 );
  }

}


TEST_CASE( "Collision order binning structure, collison range case", "[ParticleAttributeBinningStructure.h]" ) {

  CollisionOrderBinningStructure bin(0,3);
 
  point pt(0,0,0);
  point dir(0,1,0);
  Particle particle(pt , dir , 4); 
  
  SECTION ( "test collision order binning in range" ) {
    REQUIRE( bin.getIndex(std::make_shared<Particle>(particle)) == 0 );
  }
  
  particle.countCollision();
  particle.countCollision();
  
  SECTION ( "test collision order binning in range" ) {
    REQUIRE( bin.getIndex(std::make_shared<Particle>(particle)) == 2 );
  }
  
  particle.countCollision();
  particle.countCollision();
  particle.countCollision();
  
  SECTION ( "test collision order binning out of range" ) {
    REQUIRE( bin.getIndex(std::make_shared<Particle>(particle)) == -1 );
  }

}
