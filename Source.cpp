#include <cmath>
#include <vector>
#include <tuple>
#include "Random.h"
#include "Source.h"
#include "Particle.h"

unsigned int Source::groupSample(std::vector<double> groupProbability)
{
	if(groupProbability.size() > 1) {
	    double rand = rng->Urand();
	    double c = 0;
	    for(unsigned int i = 0; i < groupProbability.size(); i++)
	    {
		    c += groupProbability[i];
		    if(c > rand)
		    {
			    return (i+1);
		    }
	    }
    }

    return(1);
}

Part_ptr setSourcePoint::sample(){
	double pi = acos(-1.);
	
	auto group = groupSample(groupProbability);
	
	point pos = point(x0,y0,z0);
	
	double mu = 2 * rng->Urand() - 1;
	double phi = 2 * pi*rng->Urand();
	double omegaX=mu;
	double omegaY=sin(acos(mu))*cos(phi);
	double omegaZ=sin(acos(mu))*sin(phi);
	point dir = point(omegaX,omegaY,omegaZ);
	
    Part_ptr p = std::make_shared<Particle>(pos, dir, group );

	return p;
}

Part_ptr setSourceSphere::sample(){
	double pi = acos(-1.);
	//Radius of the new particle
	//double radius = pow((pow(radInner,3.0) + rng->Urand()*(pow(radOuter,3.0)-pow(radInner,3.0))),(1. / 3.));
	//double mu = 2.0 * rng->Urand() - 1.0;
	//double phi = 2.0 * pi*rng->Urand();

	//double x=radius*sqrt(1-pow(mu,2.))*cos(phi)+x0;
	//double y=radius*sqrt(1-pow(mu,2.))*sin(phi)+y0;
	//double z=radius*mu+z0;
	//std::cout << "mu: " << mu << " x: " << x << " y: " << y << " z: " << z << std::endl;
	//std::cout << std::endl;
	double x;
	double y;
	double z;
	bool reject = true;
	while(reject)
	{
		x = 2*rng->Urand()*radOuter;
		y = 2*rng->Urand()*radOuter;
		z = 2*rng->Urand()*radOuter;
		double dist = sqrt(x*x+y*y+z*z);
		if(dist < radOuter)
			reject = false;
	}
	auto group = groupSample(groupProbability);
	
	point pos = point(x,y,z);

        // direction sampling	
	double mu = 2 * rng->Urand() - 1;
	double phi = 2 * pi*rng->Urand();
	double omegaX=mu;
	double omegaY=sin(acos(mu))*cos(phi);
	double omegaZ=sin(acos(mu))*sin(phi);
	point dir = point(omegaX,omegaY,omegaZ);
	
     Part_ptr p = std::make_shared<Particle>(pos, dir, group );

	return p;

}

Part_ptr setSourceXAnnulus::sample(){
	//I dont like rejection sampling for this becuase the inner and outer radii may be 
	//very similar in some systems - if the radii are close and large it may take a very long
	//time to actually guess a point in the box on the annulus
	//However - if a cylinder is passed I do switch to rejection

	double pi = acos(-1.);

	auto group = groupSample(groupProbability);

    //direction sampling	
    //QUESTION?? Is this any random angle?
	double mu = 2 * rng->Urand() - 1;
	double phi = 2 * pi*rng->Urand();
	double omegaX=mu;
	double omegaY=sin(acos(mu))*cos(phi);
	double omegaZ=sin(acos(mu))*sin(phi);
	point dir = point(omegaX,omegaY,omegaZ);

	double x, y, z;
	x = height*rng->Urand();
	if(radInner != 0) {
		phi = 2 * pi*rng->Urand();
		double dist = std::sqrt(radInner*radInner + 
			          (radOuter*radOuter - radInner*radInner)*rng->Urand());	
		y = dist*cos(phi);
		z = dist*sin(phi);
	}
	else{
		bool reject = true;
		while(reject)
		{
			y = 2*rng->Urand()*radOuter;
			z = 2*rng->Urand()*radOuter;
			double dist = sqrt(y*y+z*z);
			if(dist < radOuter)
				reject = false;
		}
	}
	point pos = point(x,y,z);

    Part_ptr p = std::make_shared<Particle>(pos, dir, group );

	return p;

}

Part_ptr setSourceYAnnulus::sample(){
	//I dont like rejection sampling for this becuase the inner and outer radii may be 
	//very similar in some systems - if the radii are close and large it may take a very long
	//time to actually guess a point in the box on the annulus
	//However - if a cylinder is passed I do switch to rejection

	double pi = acos(-1.);

	auto group = groupSample(groupProbability);

    //direction sampling	
	double mu = 2 * rng->Urand() - 1;
	double phi = 2 * pi*rng->Urand();
	double omegaX=mu;
	double omegaY=sin(acos(mu))*cos(phi);
	double omegaZ=sin(acos(mu))*sin(phi);
	point dir = point(omegaX,omegaY,omegaZ);

	double x, y, z;
	y = height*rng->Urand();
	if(radInner != 0){
		phi = 2 * pi*rng->Urand();
		double dist = std::sqrt(radInner*radInner + 
			          (radOuter*radOuter - radInner*radInner)*rng->Urand());
		x = dist*cos(phi);
		z = dist*sin(phi);
	}
	else{
		bool reject = true;
		while(reject)
		{
			x = 2*rng->Urand()*radOuter;
			z = 2*rng->Urand()*radOuter;
			double dist = sqrt(x*x+z*z);
			if(dist < radOuter)
				reject = false;
		}
	}
	point pos = point(x,y,z);

    Part_ptr p = std::make_shared<Particle>(pos, dir, group );

	return p;

}


Part_ptr setSourceZAnnulus::sample(){
	//I dont like rejection sampling for this becuase the inner and outer radii may be 
	//very similar in some systems - if the radii are close and large it may take a very long
	//time to actually guess a point in the box on the annulus
	//However - if a cylinder is passed I do switch to rejection

	double pi = acos(-1.);

	auto group = groupSample(groupProbability);

    //direction sampling	
	double mu = 2 * rng->Urand() - 1;
	double phi = 2 * pi*rng->Urand();
	double omegaX=mu;
	double omegaY=sin(acos(mu))*cos(phi);
	double omegaZ=sin(acos(mu))*sin(phi);
	point dir = point(omegaX,omegaY,omegaZ);

	double x,y,z;
	z = height*rng->Urand();
	if(radInner != 0){
		phi = 2 * pi*rng->Urand();
		double dist = std::sqrt(radInner*radInner + 
			          (radOuter*radOuter - radInner*radInner)*rng->Urand());
		x = dist*cos(phi);
		y = dist*sin(phi);
	}
	else{
		bool reject = true;
		while(reject)
		{
			x = 2*rng->Urand()*radOuter;
			y = 2*rng->Urand()*radOuter;
			double dist = sqrt(x*x+y*y);
			if(dist < radOuter)
				reject = false;
		}
	}
	point pos = point(x,y,z);


    Part_ptr p = std::make_shared<Particle>(pos, dir, group );

	return p;

}


