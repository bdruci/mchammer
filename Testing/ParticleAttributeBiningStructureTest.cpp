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


TEST_CASE( "angle binning structure", "[ParticleAttributeBinningStructure.h]" ) {

  point dir(0,1,0);
  point dir2(0,-1,0);
  AngleBinningStructure bin(0 , 1 , 10 , dir);

  point pt(0,0,0);
  Particle particle(pt , dir2 , 4); 
  
  SECTION ( "test angle binning structure, out of range" ) {
    REQUIRE( bin.getIndex(std::make_shared<Particle>(particle)) == -1 );
  }
  
  point dir3(0 , 0.97 , 0.2425); // dir3 . dir = 0.967... <- last element of bin
  particle.setDir( dir3 );
  
  SECTION ( "test angle binning structure, out of range" ) {
    REQUIRE( bin.getIndex(std::make_shared<Particle>(particle)) == 9 );
  }

}

TEST_CASE( "angle binning structure, negative angles", "[ParticleAttributeBinningStructure.h]" ) {

  point dir(0,1,0);
  point dir2(0 , -1 , 0);
  AngleBinningStructure bin(-0.9 , -0.1 , 2 , dir);

  point pt(0,0,0);
  Particle particle(pt , dir2 , 4); 
  
  SECTION ( "test angle binning structure, out of range" ) {
    REQUIRE( bin.getIndex(std::make_shared<Particle>(particle)) == -1 );
  }

  
  point dir3(0.588348, -0.784465, 0.196116); // dir3 . dir = -0.79.. <- 1st element of bin
  particle.setDir( dir3 );
  
  SECTION ( "test angle binning structure, out of range" ) {
    REQUIRE( bin.getIndex(std::make_shared<Particle>(particle)) == 0 );
  }


  point dir4(0 , 0.9 , 0.45589);
  particle.setDir(dir4);

  SECTION ( "test angle binning structure, out of range positivr" ) {
    REQUIRE( bin.getIndex(std::make_shared<Particle>(particle)) == -1 );
  }

}
