/*
 * Author: Kyle Beyer
 * Last Updated: 11/30/17
 *
 *  This is a class for storing binning structures of arbitrary particle attributes 
 *  
 *  It wraps the class template Utility::BinningStucture and adds interface get 
 *  Particle attributes and setup the binning structure, with each derived class
 *  being specific to a particle attribute
 *
 *  Should be constructed during input parsing for each type of particle attribute 
 *  binning in the tally specification
 *
 */

#include "ParticleAttributeBinningStructure.h"

/* Integer Particle Attributes */
int GroupBinningStructure::getIndex( Part_ptr p )
{
  return(binning.getIndex( p->getGroup() ) );
};

void GroupBinningStructure::printTabularData( const vector< double > &data        , 
                                              const vector< double > &uncertainty , 
                                              std::ofstream &out                    ) 
{
  binning.printTabularData( data , uncertainty , out );
}

void GroupBinningStructure::printLowerEdge( unsigned int ind , 
                                            std::ofstream &out ) 
{
  out << getStrAttribute(attr) << " = ";
  binning.printLowerEdge( ind , out );
}

int CollisionOrderBinningStructure::getIndex( Part_ptr p ) 
{
  return(binning.getIndex( p->getNumCollisions() ) );
};
    
void CollisionOrderBinningStructure::printTabularData( const vector< double > &data        , 
                                                       const vector< double > &uncertainty , 
                                                       std::ofstream &out                    ) 
{
  binning.printTabularData( data , uncertainty , out );
}

void CollisionOrderBinningStructure::printLowerEdge( unsigned int ind , 
                                                     std::ofstream &out ) 
{
  binning.printLowerEdge( ind , out );
}

/* Continous Particle Attributes */

void HistogramBinningStructure::printTabularData( const vector< double > &data        , 
                                                  const vector< double > &uncertainty , 
                                                  std::ofstream &out                    ) 
{
  binning.printTabularData( data , uncertainty , out );
}

void HistogramBinningStructure::printLowerEdge( unsigned int ind , 
                                                std::ofstream &out ) {
  binning.printLowerEdge( ind , out );
}

int AngleBinningStructure::getIndex( Part_ptr p ) 
{
  // calculates the cosine of the angle between dir and the direction of p, and gets the corresponding index from binningStructure
  return( binning.getIndex(  p->getDir() * dir  ) );
};
