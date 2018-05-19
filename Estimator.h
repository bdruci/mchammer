/*
 * Author: Kyle Beyer
 * Last Updated: 05/03/18
 *
 * This is the most basic estimator object. 
 * It is generic to any type of estimator.
 */

#ifndef _ESTIMATOR_HEADER_
#define _ESTIMATOR_HEADER_

#include <iostream>
#include <vector>
#include <memory> 

using std::vector;
using std::string;

class Estimator {
  protected:
    double currentHistTally;
    double histTally;
    double histTallySqr;

  public:
    Estimator(): currentHistTally(0.0) , histTally(0.0) , histTallySqr(0.0) {}; 
   ~Estimator() {};
    
    // set/gets
    double getCurrentHistTally() { return( currentHistTally ); };
    double getHistTally()    	   { return( histTally        ); };
    double getHistTallySqr() 	   { return( histTallySqr     ); };
    
    // scoring methods
    void score(double val);
    void endHist();

    // output methods
    double getEstimate(   unsigned long long numHist);
    double getUncertainty( unsigned long long numHist);
};

#endif
