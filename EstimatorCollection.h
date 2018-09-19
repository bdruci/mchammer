/*
 * Author: Kyle Beyer
 * Last Updated: 11/30/17
 *
 * EstimatorCollection acts as an n dimensional histogram
 * with the space of indepenent variables being a 
 * linear combination of particle attributes, 
 * with the frequency of a point within that space
 * being the score contributed by particles with that
 * set or combination of attributes
 *
 * This class is for grouping tallies to get an answer as a function of 
 * particle attributes
 *   e.g. energy group, collision order, angle, type, etc.
 *
 * An EstimatorCollection is created at input, and belongs to one or more 
 * geometric objects 
 * (e.g. cell, surface, tet). 
 * At this time, all the particle attributes to be binned over
 * and their respective structures are required, 
 * as well as the type of Estimator being created.
 *
 * At this time, a geometric divisor can be set to normalize the results 
 * of the estimator, based on it's type and the geometric objects it belongs to.
 * 
 * Scoring is handled by event handler, which determines the correct 
 * multiplier based on the type of Estimator, and feeds the particle 
 * and multipler to the EstimatorCollection for scoring
 *
 *
 */

#ifndef _ESTIMATORCOLLECTION_HEADER_
#define _ESTIMATORCOLLECTION_HEADER_

#include <map>
#include <memory>
#include <stdexcept>
#include <fstream>

#include "Utility.h"
#include "Particle.h"
#include "Estimator.h"
#include "ParticleAttributeBinningStructure.h"

using std::vector;

typedef std::shared_ptr<Estimator>                         Estimator_ptr;
typedef std::shared_ptr<Particle>                          Part_ptr;
typedef std::shared_ptr<ParticleAttributeBinningStructure> Bin_ptr;

class EstimatorCollection {
  public:
  /* ----------------------------------------------------------------------------
   *  EstimatorType, accessors, and helper functions
   *  
   * ---------------------------------------------------------------------------- */
   
    // Types of Estimators
    // Allows EventHandler to pass in the correct multiplier
    // corresponding to the estimator type
    enum class EstimatorType {
      SurfaceCurrent ,
      SurfaceFluence , 
      TrackLength    ,
      Collision
    };

    // check if a type is valid
    bool validType(EstimatorType t);
    
    EstimatorType getType();
    std::string   getStrType();
    
  private:
    // unique name identifying estimator
    std::string name;

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
    
    // number of source particles
    const unsigned long long numHist;
    
    // a multiplier to normalize an estimate by e.g. volume of a cell/cells or area
    // of a surface/surfaces
    double geometricMultiplier = 1;
    
    // number of Estimators in the collection
    // size = sum_i N_i , where i denotes an attributed the estimator collection is 
    // binned over
    // and N_i is the number of bins in assigned to that attribute
    // e.g. 2 groups, and 0 through 3 collisions would be size = 2 * 4 = 8
    int size;

    // the number of bins over each attribute
    vector <unsigned int>  binSizes;
    
    // the binning structure for each attribute
    std::map< ParticleAttribute , Bin_ptr >  attributes;
  
    // an indexer for iterating over the dimensions of the histogram 
    // at output time
    std::map< unsigned int , ParticleAttribute > indexer;

    // all the estimators in the collection
    // these are what's actually being scored
    vector< Estimator_ptr >  estimators;
    
    // find index of estimator to score
    // the second boolean value in the pair is true of the particle has all it's 
    // attributes within the range of binning structures for this tally
    int getLinearIndex(Part_ptr p);

  /* ----------------------------------------------------------------------------
   *  Output - private
   *  
   * ---------------------------------------------------------------------------- */
    
    // prints the lower bin edge for the bins indexed by index 
    // in the 1st N-1 dimensions in the histogram, to a given ofstream
    void printTableHeader( const vector< unsigned int >& , std::ofstream& );
    
    // prints the table of estimator scores for all elements in the 
    // last dimension of the histogram, given the index of the 
    // 1st N-1 dimensions, to a given ofstream
    void printTable( const vector< unsigned int >& , std::ofstream& );

    // increments an n dimensional index one step further through the histogram
    // space of independent variablesa
    //    e.g. in the direction from (0,0,...) --> (binSize1,binSize2,...)
    //    incrementing the nth dimension 1st, then n-1st, etc.
    void advanceIndex( vector< unsigned int >&);

  public:

  /* ----------------------------------------------------------------------------
   *  Constructor/destructor
   *  
   * ---------------------------------------------------------------------------- */

    EstimatorCollection( std::string namein                                   , 
                         std::map< ParticleAttribute , Bin_ptr > attributesin , 
                         EstimatorType t                                      ,
                         unsigned long long numHistin                           );

   ~EstimatorCollection() {};

  /* ----------------------------------------------------------------------------
   *  Scoring functions - public
   *  
   * ---------------------------------------------------------------------------- */

    // score a tally in the correct bin, with a given multiplier
    void score(Part_ptr p, double multiplier);
    
    // end the history for all the Estimators
    void endHist();

    // set the geometric divisor to normalize estimators
    // implementation depends on type
    // will divide all scores by div at output time
    void setGeometricDivisor(double div);
  
  /* ----------------------------------------------------------------------------
   *  Accessors
   *  
   * ---------------------------------------------------------------------------- */

    int getSize()         { return( size ); };
    
    std::string getName() { return(name);    };
    
  /* ----------------------------------------------------------------------------
   *  Output - public 
   *  
   * ---------------------------------------------------------------------------- */

    // format and append independent variables vs. estimators 
    // to an output filestream
    void printEstimators();
   
  /* ----------------------------------------------------------------------------
   *  Testing and error checking
   *  
   * ---------------------------------------------------------------------------- */

    // for checking values in a specific bin in EstimatorCollection
    // only used for testing
    double checkEstimator(   const vector< unsigned int > &indices );
    double checkUncertainty( const vector< unsigned int > &indices );
    
    // error handling - thrown when binningStructure returns an out of range index
    void throwOutOfRange( std::string throwLocation );
};

#endif
