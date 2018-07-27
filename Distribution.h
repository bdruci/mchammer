#ifndef _DISTRIBUTION_H__
#define __DISTRIBUTION_H__

#include <string>
#include <cassert>
#include "Random.h"
#include "Point.h"

using namespace std;

//TODO: Add shell functionality to sphere
//Get better sense of isotropicDir
//implement catagorical, exponential, normal
//Make better tests / fix current tests
template <typename T> 
class Distribution {

private:

	string name;

public:

	Distribution( string name_in ) : name(name_in) {};
	~Distribution() {};

	string getName() { return name; };

	virtual T sample() = 0;

};

//Returns a random point between the start and the end 
class uniformContinuous : public Distribution<double> {

private:

	double start, end;

public:

	uniformContinuous( double s_in, double e_in ) : start(s_in), end(e_in), Distribution<double>("Uniform") { assert( start < end ); };
	~uniformContinuous() {};

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
class catagoricalDistribution : public Distribution<X> {

private:

	vector<X> elements;

public:

	catagoricalDistribution( vector<X> elements_in ) : Distribution<X>("catagorical"), elements(elements_in) {};
	~catagoricalDistribution() {};

	X sample();
};

#endif