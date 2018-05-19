/*
 * Author: Kyle Beyer
 * Last Updated: 11/30/17
 *
 *  This is a class for storing binning structures of arbitrary particle attributes 
 *  
 *  It wraps the class template Utility::BinningStucture and adds interface get Particle attributes and setup the 
 *  binning structure, with each derived class being specific to a particle attribute
 *
 *  Should be constructed during input parsing for each type of particle attribute binning in the tally specification
 *
 */

#include "ParticleAttributeBinningStructure.h"

// operator overload for ParticleAttribute for ostream
std::ostream& operator<<(std::ostream& out, const ParticleAttribute attr){  
  // map from ParticleAttribute to string
  std::map < ParticleAttribute , std::string > particleAttributeToString;
  if ( particleAttributeToString.size() == 0 ) {
    particleAttributeToString[ParticleAttribute::group          ] = "energy group";
    particleAttributeToString[ParticleAttribute::collisionOrder ] = "collision order";
    particleAttributeToString[ParticleAttribute::angle          ] = "angle";
  }
  return out << particleAttributeToString[attr];
};

/* Integer Particle Attributes */

int GroupBinningStructure::getIndex( Part_ptr p ) {
  return(binning.getIndex( p->getGroup() ) );
};

int CollisionOrderBinningStructure::getIndex( Part_ptr p ) {
  return(binning.getIndex( p->getNumCollisions() ) );
};

/* Continous Particle Attributes */

int AngleBinningStructure::getIndex( Part_ptr p ) {
  // calculates the cosine of the angle between dir and the direction of p, and gets the corresponding index from binningStructure
  return( binning.getIndex(  p->getDir() * dir  ) );
};
