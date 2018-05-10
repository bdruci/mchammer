
/*
 * Author: Kyle Beyer
 *
 */

#include "EventHandler.h"

// Varaidic template function to score all the estimator collections of an arbitrary 
// number of objects who have a getEstimators methods that retunrs a vector of EstimatorCollections
// into single vector of estimator collections, in linear time (see vector::inster)


double CollisionHandler::getMultiplier(EstimatorCollection::EstimatorType type , Point_ptr p0 , Part_ptr particle) {
  double multiplier;
  switch(type) {
    case EstimatorCollection::EstimatorType::Collision:
      // multiplier for a collision flux tally is 1 over the macroscopic cross section of the material
      // the collision occured in
      // will be divided by tet, cell, or multicell volume at the end of the calculation
      multiplier = 1 / currentCell->getMat()->getMacroXS(particle->getGroup());
      break;
    case EstimatorCollection::EstimatorType::TrackLength:
      // multiplier for a track length estimator is the distance the particle traveled in a volume 
      // will be divided by cell, mesh element, or multicell volume at the end of the calculation
      // NOTE: track length estimators can belong to a cell or structured mesh element, but not to an unstructured mesh (e.g. tet)
      multiplier = Utility::pointL2( *p0 , particle->getPos() );
      break;
    default:
      std::cerr << "Error in CollisonHandler::getMultiplier" << std::endl;
      std::cerr << "Collision handler can only get multipliers for TrackLength and Collison estimator types" << std::endl;
      std::cerr << "This is because only TrackLength and Collision tallies are scored on Collisions" << std::endl;
      throw;
  }

  return(multiplier);

}

void CollisionHandler::score(Point_ptr p0 , Part_ptr particle) {
  double multiplier;
  for( auto est : currentCell->getEstimators() ) {
    // score all estimators that belong to the cell the particle has collided in
    // these can be collision or track length estimators
    multiplier = getMultiplier( est->getType() , p0 , particle);
    est->score(particle , multiplier);
  }
  for( auto est : currentTet->getEstimators() ) {
    // score all estimators that belong to the unstructured mesh element that the particle has collided in
    // this can only be a collision estimator
    multiplier = getMultiplier( est->getType() , p0 , particle);
    est->score(particle , multiplier);
  }

}

double SurfaceCrossingHandler::getMultiplier(EstimatorCollection::EstimatorType type , Point_ptr p0 , Part_ptr particle) {
  double multiplier;
  switch(type) {
    case EstimatorCollection::EstimatorType::SurfaceCurrent:
      // there is no multiplier for surface current - it is literally a count of the number of particles 
      // that cross a surface 
      multiplier = 1;
      break;
    case EstimatorCollection::EstimatorType::SurfaceFluence:
      // multiplier for a surface flux tally is 1 over the cosine of the angle between the particle direction and
      // the normal vector at the surface at the point it crossed
      // will be divided by the surface area at the end of the calculation
      multiplier = 1 / ( particle->getDir() * surfaceCrossed->getNormal( particle->getPos() ) );
      break;
    case EstimatorCollection::EstimatorType::TrackLength:
      // multiplier for a track length estimator is the distance the particle traveled in a volume 
      // will be divided by cell volume at the end of the calculation
      // NOTE: track length estimators can belong to a cell or structured mesh element, but not to an unstructured mesh (e.g. tet)
      multiplier = Utility::pointL2( *p0 , particle->getPos() );
      break;
    default: 
      std::cerr << "Error in SurfaceCrossingHandler::getMultiplier" << std::endl;
      std::cerr << "SurfaceCrossing handler can only get multipliers for TrackLength, SurfaceCurrent and SurfaceFluence estimator types" << std::endl;
      std::cerr << "This is because only TrackLength and Collision tallies are scored on Collisions" << std::endl;
      throw;
  }

  return(multiplier);
}

void SurfaceCrossingHandler::score(Point_ptr p0 , Part_ptr particle) {
  double multiplier;
  for( auto est : surfaceCrossed->getEstimators() ) {
    // score all estimators that belong the surface the particle has crossed
    // these can only be surface current of surface fluence estimators
    multiplier = getMultiplier( est->getType() , p0 , particle);
    est->score(particle , multiplier);
  }
  for( auto est : cellLeft->getEstimators() ) {
    // score all estimators that belong to the cell the particle has just left
    // these can only be track length estimators
    multiplier = getMultiplier( est->getType() , p0 , particle);
    est->score(particle , multiplier);
  }
}
