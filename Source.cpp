#include <cmath>
#include <vector>
#include <tuple>
#include "Random.h"
#include "Source.h"
#include "Particle.h"
#include "Distribution.h"

randomDirection dirSampleClass;
Distribution * randDir = &dirSampleClass;

unsigned int Source::groupSample(std::vector<double> groupProbability)
{
	if(groupProbability.size() > 1) {
	    double rand = Urand();
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
	
	auto group = groupSample(groupProbability);
	
	point pos = point(x0,y0,z0);

	point dir = randDir->sample();
	
    Part_ptr p = std::make_shared<Particle>(pos, dir, group );

	return p;
}

Part_ptr setSourceSphere::sample(){

	sphericalGeometry sphere(x0, y0, z0, radInner, radOuter);

	auto group = groupSample(groupProbability);
	
	point pos = sphere.sample();

	point dir = randDir->sample();
	
    Part_ptr p = std::make_shared<Particle>(pos, dir, group );

	return p;

}

Part_ptr setSourceXAnnulus::sample() {

	xAnnularGeometry xAnnulus(x0, y0, z0, height, radInner, radOuter);

	auto group = groupSample(groupProbability);

	point dir = randDir->sample();

	point pos = xAnnulus.sample();

    Part_ptr p = std::make_shared<Particle>(pos, dir, group );

	return p;

}

Part_ptr setSourceYAnnulus::sample(){

	yAnnularGeometry yAnnulus(x0, y0, z0, height, radInner, radOuter);

	auto group = groupSample(groupProbability);

	point dir = randDir->sample();

	point pos = yAnnulus.sample();

    Part_ptr p = std::make_shared<Particle>(pos, dir, group );

	return p;

}


Part_ptr setSourceZAnnulus::sample(){

	zAnnularGeometry zAnnulus(x0, y0, z0, height, radInner, radOuter);

	auto group = groupSample(groupProbability);

	point dir = randDir->sample();

	point pos = zAnnulus.sample();

    Part_ptr p = std::make_shared<Particle>(pos, dir, group );

	return p;

}


