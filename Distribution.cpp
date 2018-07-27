#include "Distribution.h"

#include <cmath>
#include <vector>
#include <iostream>

double uniformContinuous::sample() {

	double width = end - start;

	double inbetween = (Urand() * width) + start;

	return inbetween;
}

point isotropicDirection::sample() {
	double pi = acos(-1.);
	double rand = Urand();
	double mu = (2 * rand) - 1;
	std::cout << " mu is: " << mu << std::endl;
	std::cout << " rand is: " << rand << std::endl;
	double phi = 2 * pi*Urand();
	double omegaX=mu;
	double omegaY=sin(acos(mu))*cos(phi);
	double omegaZ=sin(acos(mu))*sin(phi);
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
	bool reject = true;
	while(reject)
	{
		x = (2*Urand()-1)*radOuter;
		y = (2*Urand()-1)*radOuter;
		z = (2*Urand()-1)*radOuter;
		double dist = (x*x+y*y+z*z);
		if(dist < radOuter*radOuter && dist > radInner*radInner)
			reject = false;
	}

	point pos(x + x0, y + y0, z + z0);

	return pos;
}

point xAnnularGeometry::sample() {
	//I dont like rejection sampling for this becuase the inner and outer radii may be 
	//very similar in some systems - if the radii are close and large it may take a very long
	//time to actually guess a point in the box on the annulus
	//However - if a cylinder is passed I do switch to rejection

	double pi = acos(-1.);

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

	double pi = acos(-1.);

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

	double pi = acos(-1.);

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
