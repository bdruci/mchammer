#ifndef _DISTRIBUTION_H__
#define __DISTRIBUTION_H__

#include <string>
#include <cassert>
#include "Random.h"
#include "Point.h"

using namespace std;

//TODO: Make better tests 
//Is the current return for norm okay?

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

//Returns a random point between the start and the end 
class uniformContinuous : public Distribution<double> {

private:

	double start, end;

public:

	uniformContinuous( double s_in, double e_in ) : start(s_in), end(e_in), 
					   Distribution<double>("Uniform") { assert( start < end ); };
	~uniformContinuous() {};

	double sample();
};

class exponentialContinuous : public Distribution<double> {

private:

  	double start, lambda;

public:
  
  	exponentialContinuous( double s_in, double l_in ) : start(s_in), lambda(l_in), 
					       Distribution<double>("Exponential") {};
	~exponentialContinuous() {};

	double sample();
};

//Uses the box-muller method to return an unbounded random point normally distributed 
//around the start point
class normalContinuous : public Distribution<double> {

private: 

	double start;

public:
  
  	normalContinuous( double s_in ) : start(s_in), Distribution<double>("Normal") {};
	~normalContinuous() {};

	double sample();
};

//Returns a random direction unit vector 
class isotropicDirection : public Distribution<point> {

public:

	isotropicDirection() : Distribution<point>("Direction") {};
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
	//if it's a cube
	cuboidGeometry( double x_in, double y_in, double z_in, double side_in) 
				  : x0(x_in), y0(y_in), z0(z_in), xaxis(side_in), yaxis(side_in), zaxis(side_in), 
				    Distribution<point>("Cube") {};
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
		for(int j = 0; j < probs_in.size(); ++j){
			totalProb += probs_in.at(j);
		}
		for(int i = 0; i < probs_in.size(); ++i) {
			probabilities.push_back(probs_in.at(i)/totalProb);
		}
		assert(probabilities.size() == elements.size());
	};
	~catagoricalWeighted() {};

	X sample() {

		double stopPoint = Urand();
		double place = 0;
		for(int i = 0; i < elements.size(); ++i){
			place += probabilities.at(i);
			if(place > stopPoint){
				return elements.at(i);
			}
		}
		assert(false); //should never get this far
		return elements.at(0);
	};
};

#endif