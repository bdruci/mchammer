/*
 * Author: Kyle Beyer
 * Last Updated: 11/30/17
 *
 * This class is for grouping tallies to get an answer as a function of particle attributes
 *  e.g. energy group, collision order, angle, type, etc.
 */

#include <cassert>
#include "EstimatorCollection.h"


EstimatorCollection::EstimatorCollection(std::map< string , Bin_ptr > attributesin , EstimatorType t): 
                                          attributes(attributesin) , type(t) 
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
};

void EstimatorCollection::endHist() {
  for(auto estimator : estimators) {
    estimator->endHist();
  }
};

int EstimatorCollection::getLinearIndex(Part_ptr p ) {
  vector<int> indices;
  for(auto const& attribute : attributes) {
    std::pair<int , bool> index = attribute.second->getIndex(p);
    if(index.second == true) {
      // if the particle attribute is within the binning range, append the index to the n-dimensional indices
      indices.push_back( index.first );
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
  switch(type) {
    case EstimatorType::SurfaceCurrent:
      // there is no geometric divisor for surface current - it is literally a count of the number of particles 
      // that cross a surface. This should throw a runtime error.
      std::cerr << "Error in EstimatorCollection::setGeometricDivisor"  << std::endl;
      std::cerr << "SurfaceCurrent estimators don't have geometric multipliers" << std::endl;
      throw;
      break;
    case EstimatorType::SurfaceFluence:
      // the geometric divisor for a surface fluence estimator is the area being tallied over
      geometricDivisor = div;
      break;
    case EstimatorType::TrackLength:
      // the geometric divisor for a track length estimator is the volume being tallied over
      geometricDivisor = div;
      break;
    case EstimatorType::Collision:
      // the geometric divisor for a collision estimator is the volume being tallied over
      geometricDivisor = div;
      break;
    default: 
      std::cerr << "Error in EstimatorCollection::setGeometricDivisor"  << std::endl;
      std::cerr << "Unknown EstimatorType" << std::endl;
      throw;
  }
}

