#ifndef _DISTRIBUTION_H__
#define __DISTRIBUTION_H__

#include <string>
#include <cassert>
#include <iostream>

#include "Utility.h"
#include "Random.h"
#include "Point.h"

using namespace std;

//Base class with templated return type for sample
template <class T> 
class Distribution {

private:

  string name;

public:

  Distribution( string name_in ) : name(name_in) {};
  ~Distribution() {};

  virtual string getName() final { return name; };

  virtual T sample() = 0;

};


//Returns a random double between the start and the end 
class uniformContinuous : public Distribution<double> {

private:

  double start, end;

public:

  uniformContinuous( double s_in, double e_in ) : start(s_in), end(e_in), 
                     Distribution<double>("Uniform") { 

    assert( !Utility::FloatEqual(start, end, .0001) ); 
    if(start > end)
    {
      start = end;
      end = s_in;
    }

  };
  ~uniformContinuous() {};

  double sample();

};


//Returns a exponentially distributed point from start point with passed rate
class exponentialContinuous : public Distribution<double> {

private:

    double start, lambda;

public:
  
  exponentialContinuous( double s_in, double l_in ) : start(s_in), lambda(l_in), 
                         Distribution<double>("Exponential") {};
  ~exponentialContinuous() {};

  double sample();

};


//Returns a normally distributed point with the mean and standard deviation passed 
class gaussian : public Distribution<double> {

private: 

  double mu, stdev;

public:
  
  gaussian( double mu_in, double stdev_in ) : mu(mu_in), stdev(stdev_in),
            Distribution<double>("Normal") {};
  //Delegating constructor
  gaussian() : gaussian(0,1) {};
  ~gaussian() {};

  double sample();

};


//Returns a random direction unit vector 
class isotropicDirection : public Distribution<point> {

public:

  isotropicDirection() : Distribution<point>("Isotropic") {};
  ~isotropicDirection() {};

  //Returns random direction unit vector
  point sample();

};


//returns a random point inside a box
class cuboidGeometry : public Distribution<point> {

private:

  double x0, y0, z0, xaxis, yaxis, zaxis;

public:

  cuboidGeometry( double x_in, double y_in, double z_in, double h_in, double w_in, double l_in) 
                : x0(x_in), y0(y_in), z0(z_in), xaxis(h_in), yaxis(w_in), zaxis(l_in), Distribution<point>("Cuboid") {};
  //delegating constructors 
  cuboidGeometry( double x_in, double y_in, double z_in, double side_in) 
                : cuboidGeometry( x_in, y_in, z_in, side_in, side_in, side_in) {};
  cuboidGeometry( point o, double h_in, double w_in, double l_in ) 
                : cuboidGeometry( o.x, o.y, o.z, h_in, w_in, l_in) {};
  cuboidGeometry( point o, double side_in) 
                : cuboidGeometry( o.x, o.y, o.z, side_in, side_in, side_in) {};
  ~cuboidGeometry() {};

  //Returns random point inside sphere using rejection sampling
  point sample();

};


//returns a random point inside a sphere
class  sphericalGeometry : public Distribution<point> {

private:

  double x0, y0, z0, radInner, radOuter;

public:

  sphericalGeometry( double x_in, double y_in, double z_in, double radInner_in, double radOuter_in) 
                   : Distribution<point>("Sphere"), x0(x_in), y0(y_in), z0(z_in), radInner(radInner_in), 
                     radOuter(radOuter_in) {};
  //Delegating constructors
  sphericalGeometry( double x_in, double y_in, double z_in, double rad_in ) 
                    : sphericalGeometry( x_in, y_in, z_in, 0, rad_in) {};
  sphericalGeometry( point o, double radInner_in, double radOuter_in ) 
                    : sphericalGeometry( o.x, o.y, o.z, radInner_in, radOuter_in) {};   
  sphericalGeometry( point o, double rad_in ) 
                    : sphericalGeometry( o.x, o.y, o.z, 0, rad_in) {};
  ~sphericalGeometry() {};

  //Returns random point inside sphere using rejection sampling
  point sample();

};


//returns a random point inside a cylinder centered on the x-axis
class  xAnnularGeometry : public Distribution<point> {

private:

  double x0,y0,z0, height, radInner, radOuter;

public:

  xAnnularGeometry( double x_in, double y_in, double z_in, double height_in, double radInner_in,
                    double radOuter_in) : Distribution<point>("XAnnulus"), x0(x_in), y0(y_in), z0(z_in), 
                    height(height_in), radInner(radInner_in), radOuter(radOuter_in) {};
  //Delegating constructors
  xAnnularGeometry( double x_in, double y_in, double z_in, double height_in, double rad_in ) 
                  : xAnnularGeometry( x_in, y_in, z_in, height_in, 0, rad_in) {};
  xAnnularGeometry( point o, double height_in, double radInner_in, double radOuter_in ) 
                  : xAnnularGeometry( o.x, o.y, o.z, height_in, radInner_in, radOuter_in) {};   
  xAnnularGeometry( point o, double height_in, double rad_in ) 
                  : xAnnularGeometry( o.x, o.y, o.z, height_in, 0, rad_in) {};
  ~xAnnularGeometry() {};

  point sample();

};


//returns a random point inside a cylinder centered on the y-axis
class  yAnnularGeometry : public Distribution<point> {

private:

  double x0,y0,z0, height, radInner, radOuter;

public:

  yAnnularGeometry( double x_in, double y_in, double z_in, double height_in, double radInner_in,
                    double radOuter_in) : Distribution<point>("YAnnulus"), x0(x_in), y0(y_in), z0(z_in), 
                    height(height_in), radInner(radInner_in), radOuter(radOuter_in) {};
  //Delegating constructors
  yAnnularGeometry( double x_in, double y_in, double z_in, double height_in, double rad_in ) 
                  : yAnnularGeometry( x_in, y_in, z_in, height_in, 0, rad_in) {};
  yAnnularGeometry( point o, double height_in, double radInner_in, double radOuter_in ) 
                  : yAnnularGeometry( o.x, o.y, o.z, height_in, radInner_in, radOuter_in) {};   
  yAnnularGeometry( point o, double height_in, double rad_in ) 
                  : yAnnularGeometry( o.x, o.y, o.z, height_in, 0, rad_in) {};
  ~yAnnularGeometry() {};

  point sample();

};


//returns a random point inside a cylinder centered on the z-axis
class  zAnnularGeometry : public Distribution<point> {

private:

  double x0,y0,z0, height, radInner, radOuter;

public:

  zAnnularGeometry( double x_in, double y_in, double z_in, double height_in, double radInner_in,
                    double radOuter_in) : Distribution<point>("ZAnnulus"), x0(x_in), y0(y_in), z0(z_in), 
                    height(height_in), radInner(radInner_in), radOuter(radOuter_in) {};
  zAnnularGeometry( double x_in, double y_in, double z_in, double height_in, double rad_in ) 
                  : zAnnularGeometry( x_in, y_in, z_in, height_in, 0, rad_in) {};
  zAnnularGeometry( point o, double height_in, double radInner_in, double radOuter_in ) 
                  : zAnnularGeometry( o.x, o.y, o.z, height_in, radInner_in, radOuter_in) {};   
  zAnnularGeometry( point o, double height_in, double rad_in ) 
                  : zAnnularGeometry( o.x, o.y, o.z, height_in, 0, rad_in) {};
  ~zAnnularGeometry() {};

  point sample();
};


//Returns a random element on the list of elements passed
template <typename X>
class catagoricalWeighted : public Distribution<X> {

private:

  vector<X> elements;
  vector<double> probabilities;

public:

  catagoricalWeighted( vector<X> elements_in, vector<double> probs_in ) 
                     : Distribution<X>("catagorical"), elements(elements_in) {

    double totalProb = 0;
    for(int j = 0; j < probs_in.size(); ++j)
    {
      totalProb += probs_in.at(j);
    }
    double probCounter = 0;
    for(int i = 0; i < probs_in.size(); ++i) 
    {
      probCounter += probs_in.at(i)/totalProb;
      probabilities.push_back(probCounter);
    }

    if(!(probabilities.size() == elements.size())) 
    {
      std::cout << "Improper usage - element and probability vectors must be the same size" << std::endl;
      assert(false);
    }
    if(!Utility::FloatEqual(probabilities.back(), 1.0, .001))
    {
      std::cout << "Improper probability vector passed - cumulative probability < 1 (tolerance .001)" << std::endl;
      assert(false);
    }

    probabilities.back() = 1; //Assuming properly passed probabilities this will save rounding errors
    
  };
  ~catagoricalWeighted() {};

  //Returns an elements with weighted probability
  X sample() {

    double stopPoint = Urand();
    for(int i = 0; i < elements.size(); ++i)
    {
      if(probabilities.at(i) > stopPoint)
      {
        return elements.at(i);
      }
    }
    assert(false); //should never get this far
    return elements.at(0);
  }

};


//Echos back passed element
template <typename X>
class delta : public Distribution<X> {
private:

  X echo;

public:

  delta( X echo_in ) : Distribution<X>("delta"), echo(echo_in) {};
  ~delta() {};

  X sample() { return echo; }

};

#endif