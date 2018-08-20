#ifndef _SOURCE_HEADER_
#define _SOURCE_HEADER_

#include <vector>
#include <string>

#include "Particle.h"
#include "Distribution.h"

typedef std::shared_ptr<Particle> Part_ptr;
typedef std::shared_ptr< Distribution<unsigned int> > intDist_ptr;
typedef std::shared_ptr< Distribution<point> > pointDist_ptr;

class Source {
private:

  intDist_ptr groupDist;
  pointDist_ptr dirDist;
  pointDist_ptr posDist;
  std::string sourceName;

public:

  Source(std::string label, intDist_ptr groupDist_in, pointDist_ptr dirDist_in, 
         pointDist_ptr posDist_in) : groupDist(groupDist_in), dirDist(dirDist_in), posDist(posDist_in),
         sourceName(label) {};
  ~Source() {};

  std::string getName() { return sourceName; };
  std::string groupDistType() { return groupDist->getName(); };
  std::string dirDistType() { return dirDist->getName(); };
  std::string posDistType() { return posDist->getName(); };

  Part_ptr sample() {
    unsigned int group = groupDist->sample();
    point dir = dirDist->sample();
    point pos = posDist->sample();
    Part_ptr p = std::make_shared<Particle>(pos, dir, group );
    return p;
  }
};

#endif