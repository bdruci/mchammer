#ifndef _SOURCE_HEADER_
#define _SOURCE_HEADER_

#include <vector>
#include <string>

#include "Particle.h"
#include "Distribution.h"

typedef std::shared_ptr<Particle> Part_ptr;

class Source {
private:

  Distribution<unsigned int> * groupDist;
  Distribution<point> * dirDist;
  Distribution<point> * posDist;
  std::string sourceName;

public:

  Source(std::string label, Distribution<unsigned int> * groupDist_in, Distribution<point> * dirDist_in, 
         Distribution<point> * posDist_in) : groupDist(groupDist_in), dirDist(dirDist_in), posDist(posDist_in),
         sourceName(label) {};
  ~Source() {};

  std::string getName() { return sourceName; };

  Part_ptr sample() {
    unsigned int group = groupDist->sample();
    point dir = dirDist->sample();
    point pos = posDist->sample();
    Part_ptr p = std::make_shared<Particle>(pos, dir, group );
    return p;
  }
};

#endif