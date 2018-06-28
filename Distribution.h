#ifndef _DISTRIBUTION_H__
#define __DISTRIBUTION_H__

#include <string>
#include "Random.h"
#include "Point.h"

using namespace std;

class Distribution {

private:

	string type;

public:

	Distribution( string type_in ) : type(type_in) {};
	~Distribution() {};

	string getType() { return type; };

	virtual point sample() = 0;

};

class randomDirection : public Distribution {

public:

	randomDirection() : Distribution("Direction") {};
	~randomDirection() {};

	//Returns random direction unit vector
	point sample();

};

class  sphericalGeometry : public Distribution {

private:

	double x0, y0, z0, radInner, radOuter;

public:

	sphericalGeometry( double x_in, double y_in, double z_in, double radInner_in, double radOuter_in) 
			 : Distribution("Sphere"), x0(x_in), y0(y_in), z0(z_in), radInner(radInner_in), 
			 radOuter(radOuter_in) {};
	~sphericalGeometry() {};

	//Returns random point inside sphere using rejection sampling
	point sample();

};

class  xAnnularGeometry : public Distribution {

private:

	double x0,y0,z0, height, radInner, radOuter;

public:

	xAnnularGeometry( double x_in, double y_in, double z_in, double height_in, double radInner_in,
	           double radOuter_in) : Distribution("XAnnulus"), x0(x_in), y0(y_in), z0(z_in), 
			   height(height_in), radInner(radInner_in), radOuter(radOuter_in) {};
	~xAnnularGeometry() {};

	point sample();

};

class  yAnnularGeometry : public Distribution {

private:

	double x0,y0,z0, height, radInner, radOuter;

public:

	yAnnularGeometry( double x_in, double y_in, double z_in, double height_in, double radInner_in,
	           double radOuter_in) : Distribution("YAnnulus"), x0(x_in), y0(y_in), z0(z_in), 
			   height(height_in), radInner(radInner_in), radOuter(radOuter_in) {};
	~yAnnularGeometry() {};

	point sample();

};

class  zAnnularGeometry : public Distribution {

private:

	double x0,y0,z0, height, radInner, radOuter;

public:

	zAnnularGeometry( double x_in, double y_in, double z_in, double height_in, double radInner_in,
	           double radOuter_in) : Distribution("ZAnnulus"), x0(x_in), y0(y_in), z0(z_in), 
			   height(height_in), radInner(radInner_in), radOuter(radOuter_in) {};
	~zAnnularGeometry() {};

	point sample();

};

#endif