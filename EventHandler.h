/*
 * Author: Kyle Beyer

 *
 * This class handles estimator scoring at collision and surface crossing events. 
 * The base class is an abstract interface, and the derived classes correspond to (and are explicitly called at) the mentioned events
 * The class handles determination of which tally types to populate, given a geometric object (cell, surface, unstructured mesh element)
 * as well as determines the correct multipliers to score. 
 * It acts as a high level interface between transport and EstimatorCollection
 */

#ifndef _EVENTHANDLER_HEADER_
#define _EVENTHANDLER_HEADER_

#include <iostream>
#include <vector>
#include <memory> 

#include "Utility.h"
#include "Cell.h"
#include "Mesh.h"
#include "Surface.h"
#include "Particle.h"
#include "Geometry.h"
#include "EstimatorCollection.h"

using std::vector;
using std::string;

typedef std::shared_ptr< Cell                >  Cell_ptr;
typedef std::shared_ptr< Material            >  Mat_ptr;
typedef std::shared_ptr< surface             >  Surf_ptr;
typedef std::shared_ptr< Nuclide             >  Nuclide_ptr;
typedef std::shared_ptr< Reaction            >  Reaction_ptr;
typedef std::shared_ptr< Particle            >  Part_ptr;
typedef std::shared_ptr< point               >  Point_ptr;
typedef std::shared_ptr< Tet                 >  Tet_ptr;
typedef std::shared_ptr< EstimatorCollection >  EstCol_ptr;


class EventHandler {
// abstract base class for EventHandler
  public:
    EventHandler() {};
   ~EventHandler() {};

    virtual double getMultiplier( EstimatorCollection::EstimatorType type , Point_ptr p0 , Part_ptr particle) = 0; 
    virtual void   score(Point_ptr p0 , Part_ptr particle)  = 0;
};

class CollisionHandler : public EventHandler {
// following a collision, the client must set the currentCell and currentTet as the cell and tet the collision occured in
// then the client can call score
// CollisionHandler will search Geometry, currentCell, and currentTet for any EstimatorCollections
// CollisionHandler will determine the necessary multipliers for each EstimatorCollection, and score them appropriately

  private:
    Cell_ptr currentCell;
    Tet_ptr  currentTet;
   
  public:
    CollisionHandler(): currentTet(NULL) , currentCell(NULL) {};
   ~CollisionHandler() {};

    void setCurrentCell( Cell_ptr cell ) { currentCell = cell; };
    void setCurrentTet(  Tet_ptr  tet  ) { currentTet  = tet;  };
    
    Cell_ptr getCurrentCell() { return( currentCell ); };
    Tet_ptr  getCurrentTet()  { return( currentTet  ); };

    
    double getMultiplier( EstimatorCollection::EstimatorType type , Point_ptr p0 , Part_ptr particle);
    void   score(Point_ptr p0 , Part_ptr particle);

};


class SurfaceCrossingHandler : public EventHandler {
// following a surface corssing, the client must set the surfaceCrossed and cellLeft
// then the client can call score
// SurfaceCrossingHandler will search Geometry, surfaceCrossed, and cellLeft for any EstimatorCollections
// SurfaceCrossingHandler will determine the necessary multipliers for each EstimatorCollection, and score them appropriately

  private:
    Surf_ptr surfaceCrossed;
    Cell_ptr cellLeft;

  public:
    SurfaceCrossingHandler(): surfaceCrossed(NULL) , cellLeft(NULL) {};
   ~SurfaceCrossingHandler() {};

    void setSurfaceCrossed( Surf_ptr surf )  { surfaceCrossed = surf; };
    void setCellLeft(       Cell_ptr cell )  { cellLeft       = cell; };
    
    Surf_ptr  getCurrentTet()  { return( surfaceCrossed  ); };
    Cell_ptr  getCellLeft()    { return( cellLeft        ); };

    double getMultiplier( EstimatorCollection::EstimatorType type , Point_ptr p0 , Part_ptr particle);
    void   score(Point_ptr p0 , Part_ptr particle);

};

#endif

