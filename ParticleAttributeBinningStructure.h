/*
 * Author: Kyle Beyer
 * Last Updated: 11/30/17
 *
 *  This is a class for storing binning structures of arbitrary particle attributes 
 *  
 *  It wraps the class template BinningStucture<> and adds interface
 *  Given particle attributes, it sets up the binning structure, 
 *  with each derived class being specific to a particle attribute
 *
 *  Should be constructed during input parsing for each type of particle attribute
 *  binning in the tally specification
 *
 */

#ifndef _PARTICLEATTRIBUTEBINNINGSTRUCTURE_HEADER_
#define _PARTICLEATTRIBUTEBINNINGSTRUCTURE_HEADER_

#include <utility>
#include <iostream>
#include <cassert>
#include <map>

#include "Particle.h"
#include "BinningStructure.h"
#include "Utility.h"

typedef std::shared_ptr<Particle> Part_ptr;

// needed to limit the interface with wrapping classes to only the
// allowed particle attributes
enum class ParticleAttribute {
  group , 
  collisionOrder , 
  angle
};


class ParticleAttributeBinningStructure {
  protected:
    int size;
    const ParticleAttribute attr;

  public:
    ParticleAttributeBinningStructure(int sizein , ParticleAttribute a): 
      size(sizein) , attr(a) {};
    virtual int getIndex( Part_ptr p) = 0; 
    int getSize() { return(size); };
    
    ParticleAttribute getAttribute() { return(attr); };

    inline static std::string getStrAttribute( ParticleAttribute attr ) {
      switch( attr )  {
        case ParticleAttribute::group:          return("group");          break;
        case ParticleAttribute::collisionOrder: return("collisionOrder"); break;
        case ParticleAttribute::angle:          return("angle");          break;
      }
    }

    // prints the entire domain of bin edges alongside the data corresponding
    // to each bin
    virtual void printTabularData( const vector< double > &data        , 
                                   const vector< double > &uncertainty ,
                                   std::ofstream &out                    ) = 0;

    // prints the lower bin edge given an ofstream and the index
    virtual void printLowerEdge( unsigned int ind , std::ofstream &out ) = 0;

};

/* ------------------------------------------------------------------------------
 * Integer Particle Attributes 
 * ------------------------------------------------------------------------------ */

class GroupBinningStructure : public ParticleAttributeBinningStructure {
  private:
    int numGroups;
    BinningStructure<int> binning;
    
  public:
    // default constructor - all groups
    GroupBinningStructure(int numGroups): 
      ParticleAttributeBinningStructure(numGroups , ParticleAttribute::group ) , 
      binning(1 , numGroups + 1, numGroups  , false) {};

    // constructor  - single group
    GroupBinningStructure(int numGroups , int group): 
      ParticleAttributeBinningStructure(1 , ParticleAttribute::group) , 
      binning(group , group + 1, 1 , false) {};

    // constructor  - range of groups
    GroupBinningStructure(int numGroups , int min , int max): 
      ParticleAttributeBinningStructure(1 + max - min , ParticleAttribute::group) , 
      binning(min , max + 1, 1 + max - min , false) {};

   ~GroupBinningStructure() {};
    
    int getIndex( Part_ptr p );

    // prints the entire domain of bin edges alongside the data corresponding
    // to each bin
    virtual void printTabularData( const vector< double > &data        , 
                                   const vector< double > &uncertainty ,
                                   std::ofstream &out                    );

    // prints the lower bin edge given an ofstream and the index
    void printLowerEdge( unsigned int ind , std::ofstream &out );
};
 
class CollisionOrderBinningStructure : public ParticleAttributeBinningStructure {
  private:
    BinningStructure<int> binning;
  
  public:
    // default constructor - all orders TODO needs adaptable binning structure
    // constructor - single order
    CollisionOrderBinningStructure(int order): 
      ParticleAttributeBinningStructure( 1 , ParticleAttribute::collisionOrder ) , 
      binning(order , order + 1 , 1 , false) {};

    // constructor - range of orders
    CollisionOrderBinningStructure(int min , int max): 
      ParticleAttributeBinningStructure( 1 + max - min , ParticleAttribute::collisionOrder) , 
      binning(min , max + 1 , 1 + max - min , false) {};

   ~CollisionOrderBinningStructure() {};
    
    int getIndex( Part_ptr p );   
 
    // prints the entire domain of bin edges alongside the data corresponding
    // to each bin
    virtual void printTabularData( const vector< double > &data        , 
                                   const vector< double > &uncertainty ,
                                   std::ofstream &out                    );

    // prints the lower bin edge given an ofstream and the index
    void printLowerEdge( unsigned int ind , std::ofstream &out );
};

/* ------------------------------------------------------------------------------
 * Continous Particle Attributes 
 * ------------------------------------------------------------------------------ */

// all ParticleAttributeBinningStructures for continous attributes 
// inherit from HistogramBinningStructure
class HistogramBinningStructure : public ParticleAttributeBinningStructure {
  protected:
    BinningStructure<double> binning;
  public:
    //  1st bin: [min , min + (max - min)/size )
    // last bin: [max - (max - min)/size  , max)
    
    HistogramBinningStructure(double min         , 
                              double max         ,  
                              int size           , 
                              ParticleAttribute a  ):
      ParticleAttributeBinningStructure(size , a)     , 
      binning(min , max , size , false)                {};

   ~HistogramBinningStructure() {};
    
    virtual int getIndex( Part_ptr p ) = 0;
    
    // prints the entire domain of bin edges alongside the data corresponding
    // to each bin
    virtual void printTabularData( const vector< double > &data        , 
                                   const vector< double > &uncertainty ,
                                   std::ofstream &out                    );

    // prints the lower bin edge given an ofstream and the index
    void printLowerEdge( unsigned int ind , std::ofstream &out );
};

class AngleBinningStructure : public HistogramBinningStructure {
  // this class creates a HistogramBinningStructure in the cosine of the angle
  // between a particles direction and an arbitrary unit vector 'dir'

  private:
    point dir;
  public:
    AngleBinningStructure(double min, double max , int size , point dirin): 
         HistogramBinningStructure(min , max , size , ParticleAttribute::angle) , 
         dir( dirin / (dirin * dirin) )                                          {};

    int getIndex( Part_ptr p );
};

#endif
