/*
 * Author: Kyle Beyer
 * Last Updated: 11/30/17
 *
 */
#include <cmath>
#include "Estimator.h"

// functions
void Estimator::endHist() {
  // set the current history tally and square tally running sums
  histTally    += currentHistTally;
  histTallySqr += pow( currentHistTally , 2 );
  
  // set the current hist tally to 0
  currentHistTally = 0;
};

void Estimator::score(double val) {
  currentHistTally += val;
};

// return the mean in the score in each history of whatever is being tallied
// normalized to nHist - the number of source particles
// note: the number of times endHist() is called has no impact on nHist
// nHist is a predetermined parameter.
double Estimator::getEstimate(unsigned long long nHist) {
  return( histTally / nHist );
};

// return the std deviation in the score in each history  of whatever is being tallied
// normalized to nHist - the number of source particles
double Estimator::getUncertainty(unsigned long long nHist) {
  if (nHist > 1) {
      // calculate the standard deviation of the estimator
      return( sqrt( ( 1.0 / (nHist) ) * ( histTallySqr  - ( 1.0 / nHist ) * pow( histTally , 2 )  ) ) );
  }
  else {
    return(0);
  }
};
