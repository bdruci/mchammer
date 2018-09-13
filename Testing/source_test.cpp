#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "../Catch.h"
#include "../Source.h"
#include "../Utility.h"

#include <iostream>

typedef std::shared_ptr<Particle> Part_ptr;
typedef std::shared_ptr<Source> Source_ptr;
typedef std::shared_ptr<SingleSource> SSource_ptr;
typedef std::shared_ptr< Distribution<unsigned int> > intDist_ptr;
typedef std::shared_ptr< Distribution<point> > pointDist_ptr;

TEST_CASE( "Source", "[source]" ) {


  vector<unsigned int> groups = { 1, 2, 3 };
  vector<double> groupProbs = { 1, 1, 1 };
  intDist_ptr groupsDist = std::make_shared< catagoricalWeighted<unsigned int> > (groups, groupProbs);
  pointDist_ptr dirDist = std::make_shared< isotropicDirection> ( );
  point pos(0,0,0);
  pointDist_ptr posDist = std::make_shared< delta<point> > ( pos );

  Source_ptr src = std::make_shared< SingleSource >( "Tester", groupsDist, dirDist, posDist );

  SECTION("Sample test") {
    unsigned int correctGroup = 1;
    point correctDir( 0, 0, 1 );
    point correctPos (0, 0, 0 );
    vector<double> testNums = { 0.2, 0.5, 0.25 };
    activateTesting( testNums );

    Part_ptr returned = src->sample();
    REQUIRE(returned->getGroup() == correctGroup);
    REQUIRE(Utility::PointEqual(returned->getDir(), correctDir, .001));
    REQUIRE(returned->getPos() == correctPos);
  }

  SECTION("MasterSource Compile Test") {
    Source_ptr ssrc = std::make_shared< SingleSource > ("VecTesters", groupsDist, dirDist, posDist);
    std::vector< Source_ptr > srcs = { ssrc, ssrc, ssrc };
    std::vector<double> probs = { .5, .25, .25 };
    Source_ptr mSrc = std::make_shared< MasterSource > ( srcs, probs );
    Part_ptr t = mSrc->sample();
  }

}