/*
 * Author: Kyle Beyer
 * Last Updated: 11/30/17
 *
 * This class is for grouping tallies to get an answer as a function of particle attributes
 *  e.g. energy group, collision order, angle, type, etc.
 *
 *  An EstimatorCollection is created at input, and belongs to one or more geometric objects
 *  (e.g. cell, surface, tet). At this time, all the particlr attributes to be binned over
 *  and their respective structures are required, as well as the type of Estimator being created.
 *
 *  At this time, a geometric divisor can be set to normalize the results of the estimator,
 *  based on it's type and the geometric objects it belongs to.
 *  
 *
 *  Scoring is handled by event handler, which determines the correct multiplier based on the 
 *  type of Estimator, and feeds the particle and multipler to the EstimatorCollection for 
 *  scoring
 *
 *
 */

#ifndef _ESTIMATORCOLLECTION_HEADER_
#define _ESTIMATORCOLLECTION_HEADER_

#include <map>
#include <memory>
#include <stdexcept>

#include "Utility.h"
#include "Particle.h"
#include "Estimator.h"
#include "ParticleAttributeBinningStructure.h"

using std::vector;

typedef std::shared_ptr<Estimator>                         Estimator_ptr;
typedef std::shared_ptr<Particle>                          Part_ptr;
typedef std::shared_ptr<ParticleAttributeBinningStructure> Bin_ptr;

class EstimatorCollection {
  private:
    // a multiplier to normalize an estimate by e.g. volume of a cell/cells or area
    // of a surface/surfaces
    double geometricDivisor = 1;
    
    // number of Estimators in the collection
    int    size;

    // the number of bins over each attribute
    vector <int>  binSizes;
    
    // the binning structure for each attribute
    std::map < ParticleAttribute , Bin_ptr >  attributes;

    // all the estimators in the collection
    vector   < Estimator_ptr >  estimators;
    
    // find index of estimator to score
    // the second boolean value in the pair is true of the particle has all it's 
    // attributes within the range of binning structures for this tally
    int getLinearIndex(Part_ptr p);

    // number of source particles
    const unsigned long long numHist;

  public:
    // Types of Estimators
    // Allows EventHandler to pass in the correct multiplier
    // corresponding to the estimator type
    enum class EstimatorType {
      SurfaceCurrent ,
      SurfaceFluence , 
      TrackLength    ,
      Collision
    };
    
    // syntactic sugar for iterating through valid types
    // new types added to EstimatorType must also be added
    // to possibleTypes
    const std::vector<EstimatorType> possibleTypes {
       EstimatorType::SurfaceCurrent ,
       EstimatorType::SurfaceFluence , 
       EstimatorType::TrackLength    ,
       EstimatorType::Collision
    };

    // this holds the type 
    EstimatorType type;
    
    // functions dealing with EstimatorType
    EstimatorType getType();
    bool validType(EstimatorType t);

    EstimatorCollection(std::map< ParticleAttribute , Bin_ptr > attributesin , EstimatorType t , unsigned long long numHistin);
   ~EstimatorCollection() {};

    // score a tally in the correct bin, with a given multiplier
    void score(Part_ptr p, double multiplier);

    // set the geometric divisor to normalize estimators
    // implementation depends on type
    void setGeometricDivisor(double div);

    //gets
    int getSize() { return( size ); };
    
    // end the history for all the Estimators
    void endHist();

    // print estimators
    void printEstimators( std::string filename);
    
    // for checking values in a specific bin in EstimatorCollection
    // only used for testing
    double checkEstimator(   vector< int > indices );
    double checkUncertainty( vector< int > indices );
    
    // error handling
    void throwOutOfRange( std::string throwLocation );
  
};


#endif
