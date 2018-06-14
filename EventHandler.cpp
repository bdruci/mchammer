/*
 * Author: Kyle Beyer
 *
 */

#include "EventHandler.h"


/* ===============================================================================================================
 *  CollisionHandler scoring function
 *
 * =============================================================================================================== */

// scores all the estimators in the currentCell and currentTet
// if they have been set before the function call
void CollisionHandler::score(Point_ptr p0 , Part_ptr particle) {
  if ( currentCell != NULL) {
    for( auto est : currentCell->getEstimators() ) {
      // score all estimators that belong to the cell the particle has collided in
      // these can be collision or track length estimators
      switch(est->getType() ) {

        case EstimatorCollection::EstimatorType::Collision:
          // multiplier for a collision flux tally is 1 over the macroscopic cross section of the material
          // the collision occured in
          // will be divided by tet, cell, or multicell volume at the end of the calculation
          est->score(particle ,  1 / currentCell->getMat()->getMacroXS(particle->getGroup())  );
          break;
        
        case EstimatorCollection::EstimatorType::TrackLength:
          // multiplier for a track length estimator is the distance the particle traveled in a volume 
          // will be divided by cell, mesh element, or multicell volume at the end of the calculation
          // NOTE: track length estimators can belong to a cell or structured mesh element, but not to an unstructured mesh (e.g. tet)
          est->score( particle ,   Utility::pointL2( *p0 , particle->getPos() ) );
          break;
    
        default:
          // if not a collision or track length estimator
          // ignore it and move on
          break;

      } // - end switch

    } // - end for
    
  } // - end cell estimator block

  if ( currentTet != NULL and currentCell != NULL) {
    for( auto est : currentTet->getEstimators() ) {
      // score all estimators that belong to the unstructured mesh element that the particle has collided in
      // this can only be a collision estimator
      switch(est->getType() ) {

        case EstimatorCollection::EstimatorType::Collision:
          // multiplier for a collision flux tally is 1 over the macroscopic cross section of the material
          // the collision occured in
          // will be divided by tet, cell, or multicell volume at the end of the calculation
          est->score(particle ,  1 / currentCell->getMat()->getMacroXS(particle->getGroup())  );
          break;
        
        default:
          // if not a collision estimator
          // ignore it and move on
          break;

      } // - end switch
    } // - end for

    // reset currentCell and currentTet to null
    // This forces the client to reset the currentCell every time score is called
    // This condition is forced to avoid accidently scoring the wrong estimators
    currentCell = NULL;
    currentTet = NULL;

  } // - end tet estimator block

}

/* ===============================================================================================================
 *  SurfaceCrossingHandler scoring function
 *
 * =============================================================================================================== */

void SurfaceCrossingHandler::score(Point_ptr p0 , Part_ptr particle) {
  double multiplier;
  if ( surfaceCrossed != NULL) {
    for( auto est : surfaceCrossed->getEstimators() ) {
      // score all estimators that belong the surface the particle has crossed
      // these can only be surface current of surface fluence estimators
      switch (est->getType() ) {

        case EstimatorCollection::EstimatorType::SurfaceFluence:
          // multiplier for a surface fluence estimator is 1/|Omega . n| ,
          // where Omega is the unit vector representing the particle direction
          // and n is the unit vector representing the surface normal at the point the particle crossed
          // will be divided by surface area at the end of the calculation
          est->score(particle , 1 / ( particle->getDir() * surfaceCrossed->getNormal( particle->getPos() ) ) );
          break;

        case EstimatorCollection::EstimatorType::SurfaceCurrent:
          // multiplier for surface current estimator is 1
          // It is literally a count of the average number of particles to cross a surface per history
          est->score(particle , 1 );
          break;

        default:
          // if not a surface estimator
          // ignore it and move on
          break;

      } // - end switch
    } // - end for

    // reset surfaceCrossed to null, so client must reset it before the next call to score
    surfaceCrossed = NULL;
  
  } // - end surface estimator block

  if ( cellLeft != NULL ) {
    for( auto est : cellLeft->getEstimators() ) {
      // score all estimators that belong to the cell the particle has just left
      // these can only be track length estimators
      switch (est->getType() ) {

        case EstimatorCollection::EstimatorType::TrackLength:
          // multiplier for a track length estimator is the distance the particle traveled in a volume 
          // will be divided by cell, mesh element, or multicell volume at the end of the calculation
          // NOTE: track length estimators can belong to a cell or structured mesh element, but not to an
          // unstructured mesh (e.g. tet)
          est->score(particle , Utility::pointL2( *p0 , particle->getPos() ) );
          break;

        default:
          // if not a track length estimator
          // ignore it and move on
          break;

      } // - end switch
    } // - end for

    // reset cellLeft to null, so client must reset it before the next call to score
    cellLeft = NULL;

  } // - end cell estimator block

}
