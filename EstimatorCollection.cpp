/*
 * Author: Kyle Beyer
 * Last Updated: 11/30/17
 *
 * This class is for grouping tallies to get an answer as a function of particle
 * attributes
 *  e.g. energy group, collision order, angle, type, etc.
 */

#include "EstimatorCollection.h"

/* -----------------------------------------------------------------------------
 *  Constructor 
 *  
 * ----------------------------------------------------------------------------- */


EstimatorCollection::EstimatorCollection( std::string namein , 
                                          std::map< ParticleAttribute , 
                                          Bin_ptr > attributesin , 
                                          EstimatorType t, 
                                          unsigned long long numHistin): 
                                          attributes(attributesin) , 
                                          type(t) , 
                                          numHist(numHistin) , 
                                          name(namein)
{
  // set default geometric multiplier to 1
  geometricMultiplier = 1;

  // default constructor calculates number of estimators required
  size = 1;
    
  // create a map from integer index to the particle attributes 
  // used in this problem
  unsigned int i = 0;

  for (const auto &attr : attributes) {
    // determine the total number of estimators
    // and the number needed for each attribute
    size *= attr.second->getSize();
    binSizes.push_back( attr.second->getSize() );
    
    // set indexer 
    indexer[i] = attr.first;
    ++i;
  }

  for(int i = 0; i < size; ++i) {
    // push back an estimator pointer for each bin in this attribute
    Estimator tally;
    Estimator_ptr estimator = std::make_shared<Estimator>(tally);
    estimators.push_back(estimator);
  }

  // check type validity and set type
  bool valid = validType(t);
  if (valid) {
    type = t;
  }
  else {
    std::cerr << "Error in EstimatorCollection::EstimatorCollection"  << std::endl;
    std::cerr << "Unknown EstimatorType" << std::endl;
    throw;
  }

};


/* -----------------------------------------------------------------------------
 *  EstimatorType, accessors, and helper functions
 *  
 * ----------------------------------------------------------------------------- */

// checks if an estimator type is in 
// the possible choices
bool EstimatorCollection::validType( EstimatorCollection::EstimatorType t ) 
{
  bool valid = false;
  for (auto t : possibleTypes ) {
    if ( t == type) {
      valid = true;
    }
  }
  return(valid);
}

EstimatorCollection::EstimatorType EstimatorCollection::getType() 
{
  return(type);
};

// inline static function
std::string EstimatorCollection::getStrType() 
{
  switch (type) {
    case EstimatorType::SurfaceCurrent:
      return("Surface Current");
      break;
    case EstimatorType::SurfaceFluence:
      return("Surface Fluence");
      break;
    case EstimatorType::TrackLength:
      return("Track Length");
      break;
    case EstimatorType::Collision:
      return("Collision");
      break;
  }
}

// sets the divisor (e.g. cell or cell union volume, surface or surface union area)
// used to normalize estimators
void EstimatorCollection::setGeometricDivisor(double div) 
{
    if ( type == EstimatorType::SurfaceCurrent) {
      // there is no geometric divisor for surface current -
      // it is literally a count of the number of particles 
      // that cross a surface. This should throw a runtime error.
      std::cerr << "Error in EstimatorCollection::setGeometricDivisor"  
                << std::endl;
      std::cerr << "SurfaceCurrent estimators don't have geometric multipliers" 
                << std::endl;
      throw std::runtime_error("TypeError");
    }
    else {
      geometricMultiplier = 1.0 /  div;
    }
}

/* -----------------------------------------------------------------------------
 *  Scoring functions
 *  
 * ----------------------------------------------------------------------------- */

void EstimatorCollection::endHist()
{
  for(auto estimator : estimators) {
    estimator->endHist();
  }
};


// return unraveled linear index of n-dimensional histogram binning structure
int EstimatorCollection::getLinearIndex(Part_ptr p ) 
{
  vector<unsigned int> indices;
  for(auto const& attribute : attributes) {
    int index = attribute.second->getIndex(p);
    if(index >= 0) {
      // if the particle attribute is within the binning range, append the index to the n-dimensional indices
      indices.push_back( static_cast< unsigned int > (index) );
    }
    else { 
      // if one of the particle attributes is outisde the binning range, return -1 from the function 
      return(-1);
    }
  }

  return( Utility::linearizeIndices( indices ,  binSizes ) );
};

// score an estimator
void EstimatorCollection::score(Part_ptr p  , double multiplier) 
{
  int i =  getLinearIndex(p);
  if( i >= 0  ) { 
    // if the particle attributes are within the binning range
    // score the particle at the correct index
    estimators.at(i)->score( multiplier * p->getWeight() );
  }
}

/* -----------------------------------------------------------------------------
 *  Output
 *  
 * ----------------------------------------------------------------------------- */

// Create an output file for this estimator collection
// and call printLoop() to write all the data to it
void EstimatorCollection::printEstimators( ) 
{

  // set output file name and open stream
  std::string fname = "outfiles/" + name + ".out";
  std::cout << "Writing estimator: " << name << " to " << fname << std::endl;
  std::ofstream out;
  out.open(fname , std::ofstream::out | std::ofstream::trunc );

  if( !out.is_open() ) {
    std::cerr << "Error opening estimator output file " << fname << std::endl;
  }
  else {

    // print header identifying estimator
    out << "Estimator name: " << name         << std::endl
        << "Estimator type: " << getStrType() << std::endl << std::endl;

    // initialize a vector that indexes the 1st n-1 dimensions of the histogram
    // starts on all 0's -> 1st element of each dimension
    std::vector< unsigned int > index(binSizes.size() - 1 , 0);

    // the number of elements in the 1st N-1 dimensions of the histogram
    unsigned int nTables = size / binSizes.back();

    // iterate through the tables and print data
    // TODO: store in tree structure for later access
    for( int i = 0; i < nTables; ++i ) {
      // print table number and independent variable values for 1st n-1 
      // particle attributes
      out << "Table " << i + 1 << ": ";
      printTableHeader( index , out );
      
      // print the data table for the last dimension 
      printTable( index , out );

      advanceIndex( index );
    }
  }
}

// prints the lower bin edge for the bins indexed by index 
// in the 1st N-1 dimensions in the histogram
void EstimatorCollection::printTableHeader( const vector< unsigned int > &index , 
                                            std::ofstream &out                    )
{
  for(int i = 0; i < index.size() - 1; ++i){
    (attributes[ indexer[ index[i] ] ])->printLowerEdge(index[i] , out);
    out << " , ";
  }
  (attributes[ indexer[index.back() ] ])->printLowerEdge(index.back() , out);
  out << std::endl;
}

// Fetches the data table for the last dimension in the histogram 
// corresponding to the elements in the 1st n-1 dimensions
// described by index
//
// Then feeds the data and uncertainties to the ParticleAttributeBinningStructure 
// to print in a tabular form alongisde the bin edges
void EstimatorCollection::printTable( const vector< unsigned int > &index , 
                                      std::ofstream &out                    ) 
{ 
  // initialize data and variance containers
  std::vector< double > dat(binSizes.back() , 0);
  std::vector< double > var(binSizes.back() , 0);

  // create vector for full ND index
  vector< unsigned int > fullIndex = index;
  fullIndex.push_back(0);
  unsigned int linearIndex;

  // populate data and uncertainty with the corresponding
  // estimator values
  for( int i = 0; i < binSizes.back(); ++i) {
    fullIndex[ binSizes.back() - 1 ] = i;
    unsigned int linearIndex = Utility::linearizeIndices( fullIndex , binSizes );
    dat[i] = estimators[ linearIndex ]->getEstimate(numHist) * geometricMultiplier;
    var[i] = estimators[ linearIndex ]->getUncertainty(numHist) *
             geometricMultiplier;
  }

  // send the tabular data and output stream to the 
  // ParticleAttributeBinningStructure in the Nth dimension of the histogram
  // to print
  (attributes[ indexer[ index.size() ] ])->printTabularData(dat , var , out);
}

// Description: 
//    increments an n dimensional index one step further through the histogram
//    space of independent variablesa
//      e.g. in the direction from (0,0,...) --> (binSize1,binSize2,...)
//    incrementing the nth dimension 1st, then n-1st, etc.
// Requires: n-sized vector giving an index of an ndimensional histogram
// Needs   : binSizes - the number of elements in each dimension
// Modifies: the given vector, by incrementing it one step forward without
//           overflowing the number of elements in any dimension
//           e.g.: index=(0,2,3), binSizes=(5,5,5) --> index=(1,2,4)
//                 index=(0,2,4), binSizes=(5,5,5) --> index=(1,3,0)
void EstimatorCollection::advanceIndex( vector< unsigned int > &index ) 
{
  for( int i = index.size() - 1; i >= 0; --i) {
    // if the index is at the maximum bin for the the attribute
    if (index[i] == binSizes[i] - 1) {
      // reset it to 0
      index[i] = 0;
    }
    else {
      // otherwise increment it and exit
      index[i]++;
      break;
    }
  }
}

/* -----------------------------------------------------------------------------
 *  Testing and error checking
 *  
 * ----------------------------------------------------------------------------- */

// generic out of range throw for checkEstimator and checkUncertainty
void EstimatorCollection::throwOutOfRange(std::string str) 
{
    std::cerr << "Error in EstimatorCollection::" << str         << std::endl
              << "Attempted to access an out of range Estimator" << std::endl;
    throw std::runtime_error("OutOfRangeError");
}


// check estimator value at indices
double EstimatorCollection::checkEstimator(const vector< unsigned int > &indices ) 
{
  unsigned int l = Utility::linearizeIndices(indices , binSizes);
  if ( l < size ) {
    return( estimators.at(l)->getEstimate( numHist )  );
  }
  else {
    throwOutOfRange("checkEstimator");
  }
}

// check estimator uncertainty at indices
double EstimatorCollection::checkUncertainty(const vector< unsigned int > &indices ) 
{
  unsigned int l = Utility::linearizeIndices(indices , binSizes);
  if ( l < size ) {
    return( estimators.at(l)->getUncertainty( numHist)  );
  }
  else {
    throwOutOfRange("checkUncertainty");
  }
}

