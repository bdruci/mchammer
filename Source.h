#ifndef _SOURCE_HEADER_
#define _SOURCE_HEADER_

#include <vector>
#include <string>

#include "Particle.h"
#include "Distribution.h"

typedef std::shared_ptr<Particle> Part_ptr;
typedef std::shared_ptr< Distribution<unsigned int> > intDist_ptr;
typedef std::shared_ptr< Distribution<point> > pointDist_ptr;

//REWRITE for master and norm sources

//Parent class
class Source {
private:
  std::string sourceName;
public:
  Source( std::string label ) : sourceName(label) {};
  ~Source() {}; 

  std::string getName() { return sourceName; };
  virtual std::string groupDistType() =0;
  virtual std::string dirDistType() =0;
  virtual std::string posDistType() =0;

  virtual Part_ptr sample() = 0;
};

//First derive class - holds a single source and returns a part_ptr sampled from it
class SingleSource : public Source {
private:

  intDist_ptr groupDist;
  pointDist_ptr dirDist;
  pointDist_ptr posDist;

public:

  SingleSource(std::string label, intDist_ptr groupDist_in, pointDist_ptr dirDist_in, 
         pointDist_ptr posDist_in) : groupDist(groupDist_in), dirDist(dirDist_in), posDist(posDist_in),
         Source(label) {};
  ~SingleSource() {};

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

typedef std::shared_ptr< Source > src_ptr;
typedef std::shared_ptr< Distribution< src_ptr > > srcDist_ptr;

//Second derived class - holds multiple sources. Sampling picks a source then follows SingleSource routine
class MasterSource : public Source {
private:
  srcDist_ptr srcDist;
public:
  MasterSource( std::vector<src_ptr> srcs_in, std::vector<double> srcProbs_in ) : Source("Master") {
    srcDist = std::make_shared< catagoricalWeighted< src_ptr > > ( srcs_in, srcProbs_in );
  }
  ~MasterSource() {};

  std::string groupDistType() { assert(false); };
  std::string dirDistType() { assert(false); };
  std::string posDistType() { assert(false); };

  Part_ptr sample() {
    return srcDist->sample()->sample();
  }
};

#endif