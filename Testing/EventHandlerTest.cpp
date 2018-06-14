#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <memory>

#include "../Catch.h"
#include <cassert>
#include "../EventHandler.h"

typedef std::shared_ptr< Cell >      Cell_ptr;
typedef std::shared_ptr< surface >   Surf_ptr;
typedef std::shared_ptr< Material >  Mat_ptr;
typedef std::shared_ptr< Nuclide  >  Nuclide_ptr;
typedef std::shared_ptr< XSection >  XSec_ptr;



TEST_CASE("CollisionHandler" , "[EventHandler]" ) {

  // ---------------------------------
  // construct geometry
  // ----------------------------------

  // construct XS
  std::vector< double > xsga {0.5 , 0.5 };
  XSec_ptr xsa = std::make_shared< CaptureXS > (xsga); 
  std::vector< std::vector < double > > xsgs {xsga , xsga };
  XSec_ptr xss = std::make_shared< ScatterXS > (xsgs); 

  // construct Nuclide
  auto nuc = std::make_shared< Nuclide >("Nuc");
  nuc->addXSection(xsa);
  nuc->addXSection(xss);

  // construct Material
  Mat_ptr mat = std::make_shared< Material >("mat" , 1.0);
  mat->addNuclide(nuc , 1.0);

  // construct surface
  Surf_ptr s = std::make_shared< sphere > ( "sphere" , 0 , 0 , 0 , 1);

  // construct cell 
  Cell_ptr cell = std::make_shared< Cell > ("cell1");
  cell->setMaterial( mat );
  cell->addSurfacePair( std::pair<Surf_ptr , bool> (s , true ) );

  // construct a tet inside the cell
  // assume tet encompasses the whole geometry
  point pt(0,0,0);
  Tet_ptr tet = std::make_shared<Tet> ("tet" , pt);

  // ---------------------------------
  // set up common binning structure
  // ----------------------------------

  // create a group binning structure
  Bin_ptr gbin = std::make_shared<GroupBinningStructure>(2); // 2 groups -> 2 bins
  Bin_ptr nbin = std::make_shared<CollisionOrderBinningStructure>(1,1); // bin 1st collided flux in 1 bin

  // create an attribute map with the correct binning structure
  std::map< ParticleAttribute , Bin_ptr > attributeMap;
  attributeMap[ ParticleAttribute::group          ] = gbin;
  attributeMap[ ParticleAttribute::collisionOrder ] = nbin;

  // ---------------------------------
  // construct EstimatorCollections
  // ----------------------------------=


  // create an estimator collection of the collison type
  EstimatorCollection cole( attributeMap , EstimatorCollection::EstimatorType::Collision , 10);
  cole.setGeometricDivisor(1.0);
  
  // and the track length type
  EstimatorCollection tracke( attributeMap , EstimatorCollection::EstimatorType::TrackLength , 10);
  tracke.setGeometricDivisor(1.0);

  // give the tet a collision estimator collection
  tet->addEstimator( std::make_shared<EstimatorCollection>(cole) );
  
  // give the cell a collision estimator collection and a track length estimator collection 
  cell->addEstimator( std::make_shared<EstimatorCollection>(cole) ); // the tet is pointing to the same collision estimator as the cell. Not realistic, just for testing
  cell->addEstimator( std::make_shared<EstimatorCollection>(tracke) );
  
  // create a collision handler
  CollisionHandler  colHandler;

  // create a particle at the center of the cell
  point dir(1,0,0);
  Particle particle(pt , dir , 2);
  std::shared_ptr<Particle> p = std::make_shared<Particle>(particle);

  colHandler.setCurrentCell(cell);
  colHandler.setCurrentTet(tet);


  SECTION(" set/get currentCell ") {
    REQUIRE( colHandler.getCurrentCell() == cell );
  }

  
  SECTION(" set/get current tet ") {
    REQUIRE( colHandler.getCurrentTet() == tet );
  }

  // move particle and score
  p->move( 0.1 ); // displace [0,0.1,0] from current location
  colHandler.score( std::make_shared< point >(pt) ,  p ); 


}
