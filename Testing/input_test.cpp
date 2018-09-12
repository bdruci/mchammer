#define CATCH_CONFIG_MAIN

#include "../Catch.h"
#include "../Input.h"

#include <utility>
#include <iostream>

//Geometry needs to hold a vector of sources + some mechanism to pick a source to sample 
//each history -- a class ~ catagoricalWeighted

//Does a distribution make more sense in fission::sample() rather that the floor + Urand?

//Materials werent being communicated to geometry

//FloatEqual for surface evals
//Normal vector for surfaces is negative?


typedef std::shared_ptr< Estimator >         Estimator_ptr;
typedef std::shared_ptr<EstimatorCollection> EstCol_ptr;
typedef std::shared_ptr< Reaction >          Reaction_ptr;
typedef std::shared_ptr< Nuclide  >          Nuclide_ptr;
typedef std::shared_ptr< Particle >          Part_ptr;
typedef std::shared_ptr< Cell >              Cell_ptr;
typedef std::shared_ptr< Material >          Mat_ptr;
typedef std::shared_ptr< surface >           Surf_ptr;
typedef std::shared_ptr< Source >            Source_ptr;

TEST_CASE( "Compile test", "[Input]" ) {
	SECTION( " Make sure all classes are constructed the way they should be ") {
	std::string xmlFileName = "../inputfiles/compile_test.xml";
	Input inputTest;

	inputTest.readInput( xmlFileName );

	std::shared_ptr< Geometry >   geometry  = inputTest.getGeometry();
	std::shared_ptr< Constants >  constants = inputTest.getConstants();
	std::shared_ptr< Mesh >       mesh      = inputTest.getMesh();
	std::shared_ptr< HammerTime > timer     = inputTest.getTimer();

	//Check the set up
	//Mesh file can be checked from output
	REQUIRE( constants->getNumHis() == 10 );
	REQUIRE( constants->getNumGroups() == 2 );

	std::vector< Reaction_ptr > berpReactions = geometry->getMaterials().at(0)->getAllNuclides().at(0).first->getReactions();
	std::vector< Reaction_ptr > polyReactions = geometry->getMaterials().at(1)->getAllNuclides().at(0).first->getReactions();
	std::vector< Reaction_ptr > airReactions = geometry->getMaterials().at(2)->getAllNuclides().at(0).first->getReactions();
		
	point pos(0,0,0);
	point dir(1,0,0);
	Part_ptr testerPart1 = std::make_shared< Particle > ( pos, dir, 1 );
	Part_ptr testerPart2 = std::make_shared< Particle > ( pos, dir, 1 );
	Part_ptr testerPart3 = std::make_shared< Particle > ( pos, dir, 1 );
	Part_ptr testerPart4 = std::make_shared< Particle > ( pos, dir, 1 );
	Part_ptr testerPart5 = std::make_shared< Particle > ( pos, dir, 1 );
	std::stack< Part_ptr > testerBank;
	testerBank.push(testerPart1);
	testerBank.push(testerPart2);
	testerBank.push(testerPart3);
	testerBank.push(testerPart4);
	testerBank.push(testerPart5);

	//Test nuclide data by name
	REQUIRE( berpReactions.at(0)->name() == "Capture" );
	REQUIRE( berpReactions.at(1)->name() == "Scatter" );
	REQUIRE( berpReactions.at(2)->name() == "Fission" );
	REQUIRE( berpReactions.size() == 3 );

	REQUIRE( polyReactions.size() == 2 );

	REQUIRE( airReactions.size() == 2 );
		
	//Test nuclide data by sample
	berpReactions.at(0)->sample( testerPart1, testerBank );
	REQUIRE(  !testerPart1->isAlive() );

	std::vector<double> testNums1 = { 0, 0, 1 };
	activateTesting(testNums1);
	berpReactions.at(1)->sample( testerPart2, testerBank );
	REQUIRE( testerPart2->getGroup() == 1 );

	std::vector<double> testNums2 = { 0.9 };
	activateTesting(testNums2);
	berpReactions.at(2)->sample( testerPart3, testerBank );
	REQUIRE(  testerBank.size() == 8 );
	REQUIRE( testerPart3->isAlive() );

	activateTesting(testNums1);
	polyReactions.at(0)->sample( testerPart4, testerBank );
	REQUIRE(  !testerPart4->isAlive() );

	polyReactions.at(1)->sample( testerPart5, testerBank );
	REQUIRE( testerPart5->getGroup() == 1 );

	//Test the materials 
	std::vector< Mat_ptr > materials = geometry->getMaterials();
	Mat_ptr berp = materials.at(0);
	Mat_ptr poly = materials.at(1);
	Mat_ptr air = materials.at(2);

	REQUIRE( berp->name() == "berpball" );
	REQUIRE( berp->getAtomDensity() == 1 );
	REQUIRE( Utility::FloatEqual(berp->getMacroXS( testerPart5 ), 0.4136653999, .0001 ) );

	REQUIRE( poly->name() == "polyball" );
	REQUIRE( poly->getAtomDensity() == 1 );
	REQUIRE( Utility::FloatEqual(poly->getMacroXS( testerPart5 ), 1.0550016, .0001 ) );

	REQUIRE( air->name() == "air" );
	REQUIRE( air->getAtomDensity() == 1 );
	REQUIRE( Utility::FloatEqual(air->getMacroXS( testerPart5 ), .000200795, .0001 ) );

	//Test surfaces
	std::vector< Surf_ptr > surfs = geometry->getSurfaces();

	REQUIRE( surfs.size() == 8);

	REQUIRE( surfs.at(0)->name() == "sphere1" );
	REQUIRE( surfs.at(1)->name() == "sphere2" );
	REQUIRE( surfs.at(2)->name() == "xplane1" );
	REQUIRE( surfs.at(3)->name() == "xplane2" );
	REQUIRE( surfs.at(4)->name() == "yplane1" );
	REQUIRE( surfs.at(5)->name() == "yplane2" );
	REQUIRE( surfs.at(6)->name() == "zplane1" );
	REQUIRE( surfs.at(7)->name() == "zplane2" );

	point testPoint1(203.2,0,0);
	point testPoint2(0,203.2,0);
	point testPoint3(0,0,203.2);
	point testDir1(-1,0,0);
	point testDir2(0,-1,0);
	point testDir3(0,0,-1);

	REQUIRE( Utility::FloatEqual(surfs.at(2)->distance( testPoint1, testDir1 ), 304.8, .0001) );
	REQUIRE( Utility::FloatEqual(surfs.at(3)->distance( testPoint1, testDir1 ), 101.6, .0001) );
	REQUIRE( Utility::FloatEqual(surfs.at(4)->distance( testPoint2, testDir2 ), 304.8, .0001) );
	REQUIRE( Utility::FloatEqual(surfs.at(5)->distance( testPoint2, testDir2 ), 101.6, .0001) );
	REQUIRE( Utility::FloatEqual(surfs.at(6)->distance( testPoint3, testDir3 ), 304.8, .0001) );
	REQUIRE( Utility::FloatEqual(surfs.at(7)->distance( testPoint3, testDir3 ), 101.6, .0001) );

	point testPoint4(0,0,0);
	point testDir4(1,0,0);
	point testDir5(0,1,0);

	REQUIRE( Utility::FloatEqual(surfs.at(0)->distance( testPoint4, testDir4 ), 3.79349, .0001)  );
	REQUIRE( Utility::FloatEqual(surfs.at(1)->distance( testPoint4, testDir5 ), 11.41349, .0001) );

	//test cells
	std::vector< Cell_ptr > cells = geometry->getCells();
	Cell_ptr berpCell = cells.at(0);
	Cell_ptr polyCell = cells.at(1);
	Cell_ptr airCell = cells.at(2);

	REQUIRE( berpCell->name() == "berpball");
	REQUIRE( berpCell->getMat()->name() == "berpball");

	REQUIRE( polyCell->name() == "polyball");
	REQUIRE( polyCell->getMat()->name() == "polyball");

	REQUIRE( airCell->name() == "air");
	REQUIRE( airCell->getMat()->name() == "air");

	//TODO add test for estimators - in file w/ all the esimators written
	
	//Test sources
	Source_ptr src = geometry->getSource();
	REQUIRE( src->getName() == "berpsource" );
	REQUIRE( src->groupDistType() == "delta" );
	REQUIRE( src->dirDistType() == "Isotropic" );
	REQUIRE( src->posDistType() == "Sphere" );

	std::vector<double> testNums3 = { .5, .25, .25 };
	activateTesting(testNums3);
	Part_ptr sampledPart = src->sample();
	point correctDir(0,0,1);
	point correctPos(-1.896745,0,-1.896745);

	REQUIRE( sampledPart->getGroup() == 1 );
	REQUIRE( Utility::PointEqual(sampledPart->getDir(), correctDir, .001) );
	REQUIRE( Utility::PointEqual( sampledPart->getPos(), correctPos, .001) );
	}

}