/*
 * Author: Kyle Beyer
 * Last Updated: 11/30/17
 *
 * This class is for grouping tallies to get an answer as a function of particle attributes
 *  e.g. energy group, collision order, angle, type, etc.
 */

#include "EstimatorCollection.h"


EstimatorCollection::EstimatorCollection(std::map< ParticleAttribute , Bin_ptr > attributesin , EstimatorType t, unsigned long long numHistin): 
                                          attributes(attributesin) , type(t) , numHist(numHistin)
{
// set default geometric divisor to 1
geometricDivisor = 1;

// default constructor calculates number of estimators required
  size = 1;
  for (const auto &vals : attributes) {
    // determine the total number of estimators
    // and the number needed for each attribute
    size *= vals.second->getSize();
    binSizes.push_back( vals.second->getSize() );
    for(int i = 0; i < vals.second->getSize(); ++i) {
      // push back an estimator pointer for each bin in this attribute
      Estimator tally;
      Estimator_ptr estimator = std::make_shared<Estimator>(tally);
      estimators.push_back(estimator);
    }
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


// checks if an estimator type is in 
// the possible choices
bool EstimatorCollection::validType( EstimatorCollection::EstimatorType t ) {
  bool valid = false;
  for (auto t : possibleTypes ) {
    if ( t == type) {
      valid = true;
    }
  }
  return(valid);
}

EstimatorCollection::EstimatorType EstimatorCollection::getType() {
  return(type);
};

void EstimatorCollection::endHist() {
  for(auto estimator : estimators) {
    estimator->endHist();
  }
};

int EstimatorCollection::getLinearIndex(Part_ptr p ) {
  vector<int> indices;
  for(auto const& attribute : attributes) {
    int index = attribute.second->getIndex(p);
    if(index >= 0) {
      // if the particle attribute is within the binning range, append the index to the n-dimensional indices
      indices.push_back( index );
    }
    else { 
      // if one of the particle attributes is outisde the binning range, return -1 from the function 
      return(-1);
    }
  }
  return( Utility::linearizeIndices( indices ,  binSizes ) );
};

void EstimatorCollection::score(Part_ptr p  , double multiplier) {
  int i =  getLinearIndex(p);
  if( i >= 0  ) { 
    // if the particle attributes are within the binning range
    // score the particle at the correct index
    estimators.at(i)->score( multiplier * p->getWeight() );
  }
}

void EstimatorCollection::setGeometricDivisor(double div) {
    if ( type == EstimatorType::SurfaceCurrent) {
      // there is no geometric divisor for surface current - it is literally a count of the number of particles 
      // that cross a surface. This should throw a runtime error.
      std::cerr << "Error in EstimatorCollection::setGeometricDivisor"  << std::endl;
      std::cerr << "SurfaceCurrent estimators don't have geometric multipliers" << std::endl;
      throw std::runtime_error("TypeError");
    }
    else {
      geometricDivisor = div;
    }
}

void EstimatorCollection::throwOutOfRange(std::string str) {
    std::cerr << "Error in EstimatorCollection::" << str         << std::endl
              << "Attempted to access an out of range Estimator" << std::endl;
    throw std::runtime_error("OutOfRangeError");
}

// check estimator value at indices
double EstimatorCollection::checkEstimator( vector<int> indices ) {
  int l = Utility::linearizeIndices(indices , binSizes);
  if ( l >= 0 and l < size ) {
    return( estimators.at(l)->getEstimate( numHist )  );
  }
  else {
    throwOutOfRange("checkEstimator");
  }
}

// check estimator uncertainty at indices
double EstimatorCollection::checkUncertainty( vector<int> indices ) {
  int l = Utility::linearizeIndices(indices , binSizes);
  if ( l >= 0 and l < size ) {
    return( estimators.at(l)->getUncertainty( numHist)  );
  }
  else {
    throwOutOfRange("checkUncertainty");
  }
}

