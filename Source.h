#ifndef _SOURCE_HEADER_
#define _SOURCE_HEADER_

#include <vector>
#include <string>

#include "Particle.h"
#include "Distribution.h"

typedef std::shared_ptr<Particle> Part_ptr;
typedef std::shared_ptr< Distribution<unsigned int> > intDist_ptr;
typedef std::shared_ptr< Distribution<point> > pointDist_ptr;

//Parent class
class Source {
private:
  std::string sourceName;
public:
  Source( std::string label ) : sourceName(label) {};
  ~Source() {}; 

  std::string getName() { return sourceName; };
  virtual std::string groupDistType() { assert(false); };
  virtual std::string dirDistType() { assert(false); };
  virtual std::string posDistType() { assert(false); };
  virtual std::vector< std::string > getSources() { assert(false); };

  virtual Part_ptr sample() = 0;

};

//Normal case - holds a single source and returns a part_ptr sampled from it
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

  std::string groupDistType() override { return groupDist->getName(); };
  std::string dirDistType() override { return dirDist->getName(); };
  std::string posDistType() override { return posDist->getName(); };

  Part_ptr sample() override {
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
  std::vector< std::string > srcNames;
public:
  MasterSource( std::vector<src_ptr> srcs_in, std::vector<double> srcProbs_in ) : Source("Master") {
    srcDist = std::make_shared< catagoricalWeighted< src_ptr > > ( srcs_in, srcProbs_in );
    for (int i = 0; i < srcs_in.size(); ++i)
    {
      srcNames.push_back(srcs_in.at(i)->getName());
    }
  }
  ~MasterSource() {};

  Part_ptr sample() override {
    return srcDist->sample()->sample();
  }
  std::vector< std::string > getSources() override { return srcNames;};
};

#endif