#include "Distribution.h"

#include <cmath>
#include <vector>
#include <iostream>

static const double pi = acos(-1.0);

double uniformContinuous::sample() {

	double width = end - start;

	double inbetween = (Urand() * width) + start;

	return inbetween;
}

double exponentialContinuous::sample() {

	double inbetween = (-log(Urand())/lambda) + start;

	return inbetween;
}

double normalContinuous::sample() {

	//Box-Muller method
	double norm = sqrt(-2*log(Urand())) * cos(2*pi*Urand()) + start;

	return norm;
}

point isotropicDirection::sample() {

	double mu = 2 * Urand() - 1;

	double phi = 2 * pi*Urand();
	double omegaX=mu; 
	double omegaY=sqrt(1-mu*mu)*cos(phi); //sin(acos(mu)) 
	double omegaZ=sqrt(1-mu*mu)*sin(phi);
	point dir = point(omegaX,omegaY,omegaZ);

	return dir;
}

point cuboidGeometry::sample() {

	double x, y, z;
	x = (Urand()-0.5)*xaxis;
	y = (Urand()-0.5)*yaxis;
	z = (Urand()-0.5)*zaxis;

	point pos(x + x0, y + y0, z + z0);

	return pos;
}

point sphericalGeometry::sample() {

	//Changed from original code
	double x;
	double y;
	double z;
	if( radInner !=0 )
	{
		double thickness = (radOuter - radInner) * Urand();
		point origin(x0,y0,z0);
		isotropicDirection unitRand;

		point unitPoint = unitRand.sample();
		//Scale the random point to the thickness of the shell
		unitPoint = unitPoint * thickness;
		//Add the inner rad so point is inside of shell
		unitPoint = unitPoint + radInner;
		//Add the origin to place the point in the correct 3-d  space
		unitPoint = unitPoint + origin;

		return unitPoint;
	}
	else
	{
		bool reject = true;
		while(reject)
		{
			x = (2*Urand()-1)*radOuter;
			y = (2*Urand()-1)*radOuter;
			z = (2*Urand()-1)*radOuter;
			double dist = (x*x+y*y+z*z);
			if(dist < radOuter*radOuter && dist > radInner*radInner)
			{
				reject = false;
			}
		}
	}

	point pos(x + x0, y + y0, z + z0);

	return pos;
}

point xAnnularGeometry::sample() {
	//I dont like rejection sampling for this becuase the inner and outer radii may be 
	//very similar in some systems - if the radii are close and large it may take a very long
	//time to actually guess a point in the box on the annulus
	//However - if a cylinder is passed I do switch to rejection

	double x, y, z;

	x = height*Urand();

	if(radInner != 0) {
		double phi = 2 * pi*Urand();
		double dist = std::sqrt(radInner*radInner + (radOuter*radOuter - radInner*radInner)*Urand());	
		y = dist*cos(phi);
		z = dist*sin(phi);
	}
	else{
		bool reject = true;
		while(reject)
		{
			y = (2*Urand()-1)*radOuter;
			z = (2*Urand()-1)*radOuter;
			double dist = sqrt(y*y+z*z);
			if(dist < radOuter)
				reject = false;
		}
	}
	//switched to adding the origin
	point pos = point(x + x0, y + y0, z + z0);

	return pos;
}

point yAnnularGeometry::sample() {

	double x, y, z;
	y = height*Urand();
	if(radInner != 0){
		double phi = 2 * pi*Urand();
		double dist = std::sqrt(radInner*radInner + (radOuter*radOuter - radInner*radInner)*Urand());
		x = dist*cos(phi);
		z = dist*sin(phi);
	}
	else{
		bool reject = true;
		while(reject)
		{
			x = (2*Urand()-1)*radOuter;
			z = (2*Urand()-1)*radOuter;
			double dist = sqrt(x*x+z*z);
			if(dist < radOuter)
				reject = false;
		}
	}
	//switched to adding the origin
	point pos = point(x + x0, y + y0, z + z0);

	return pos;
}

point zAnnularGeometry::sample() {

	double x,y,z;
	z = height*Urand();
	if(radInner != 0){
		double phi = 2 * pi*Urand();
		double dist = std::sqrt(radInner*radInner + (radOuter*radOuter - radInner*radInner)*Urand());
		x = dist*cos(phi);
		y = dist*sin(phi);
	}
	else{
		bool reject = true;
		while(reject)
		{
			x = (2*Urand()-1)*radOuter; 
			y = (2*Urand()-1)*radOuter;
			double dist = sqrt(x*x+y*y);
			if(dist < radOuter)
				reject = false;
		}
	}
	//switched to adding the origin
	point pos = point(x + x0, y + y0, z + z0);

	return pos;
}


