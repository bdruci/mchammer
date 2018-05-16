#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <tgmath.h> 
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
  std::vector< double > xsga {1.0 , 2.0 };
  XSec_ptr xsa = std::make_shared< CaptureXS > (xsga); 
  std::vector< std::vector < double > > xsgs;
  XSec_ptr xss = std::make_shared< ScatterXS > (xsgs); 

  // construct Nuclide
  Nuclide_ptr nuclide = std::make_shared<Nuclide> ("Nuc");
  nuclide->addXSection(xsa);
  nuclide->addXSection(xss);

  // construct Material
  Mat_ptr mat = std::make_shared< Material >("mat" , 1.0);
  mat->addNuclide(nuclide , 1.0);

  // construct surface
  Surf_ptr s = std::make_shared< sphere > ( "sphere" , 0 , 0 , 0 , 1);

  // construct cell 
  Cell_ptr cell = std::make_shared< Cell > ("cell1");
  cell->setMaterial( mat );
  cell->addSurfacePair( std::pair<Surf_ptr , bool> (s , true ) );

  // construct a tet inside the cell
  point pt(0,0,0);
  Tet tet("tet" , pt);


  // ---------------------------------
  // construct EstimatorCollections
  // ----------------------------------=

  // create a group binning structure
  Bin_ptr gbin = std::make_shared<GroupBinningStructure>(2); // 2 groups -> 2 bins
  Bin_ptr nbin = std::make_shared<CollisionOrderBinningStructure>(1,1); // bin 1st collided flux in 1 bin

  // create an attribute map with the correct binning structure
  std::map< ParticleAttribute , Bin_ptr > attributeMap;
  attributeMap[ ParticleAttribute::group          ] = gbin;
  attributeMap[ ParticleAttribute::collisionOrder ] = nbin;

  // create an estimator collection of the collison type
  EstimatorCollection e( attributeMap , EstimatorCollection::EstimatorType::Collision , 10);
  e.setGeometricDivisor(1.0);
  
  // and the track length type
  EstimatorCollection e2( attributeMap , EstimatorCollection::EstimatorType::TrackLength , 10);

  // give the tet a collision estimator collection
  
  // give the cell a collision estimator collection and a track length estimator collection 

  // create a collision handler
  CollisionHandler  colHandler;

  // create a particle at the center of the cell
  point dir(1,0,0);
  Particle particle(pt , dir , 2);
  std::shared_ptr<Particle> p = std::make_shared<Particle>(particle);

  colHandler.setCurrentCell(cell);


  //SECTION(" set/get currentCell ") {
  //  REQUIRE();
  //}


}
