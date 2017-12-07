#include <tuple>
#include <vector>
#include <memory>
#include <string>
#ifndef _SOURCE_HEADER_
#define _SOURCE_HEADER_

class Source {
public:
	virtual std::tuple <double, double, double, double, double, double > sample(double radInner, double radOuter, std::vector<double> energyProbability, std::vector<double> energyList);
};

class Point : public Source {
private: 
   double x,y,z;
   std::vector <double> energyProbability,energyList;
public:
   double radInner=0;
   double radOuter=0;
   void setSourcePoint(double xSource, double ySource, double zSource, std::vector<double> energyProbSet, std::vector<double> energyListSet);
};

class Sphere : public Source {
private: 
   double x,y,z, radInner, radOuter;
   std::vector <double> energyProbability,energyList;
public:
   void setSourceSphere(double xSource, double ySource, double zSource, double radInner, double radOuter, std::vector<double> energyProbSet, std::vector<double> energyListSet);
};

#endif
