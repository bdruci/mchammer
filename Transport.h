/*
 Author: Blake
 Date: 11/21/17
 Date: 11/21/17
 Date: 11/21/17
 Req. Files: Cell.cpp, Material.h, Surface.h, Random.h
 */

#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#include <iostream>
#include <memory>
#include <stack>
#include <limits>
#include <string>


#include "Cell.h"
#include "Material.h"
#include "Reaction.h"
#include "Surface.h"
#include "Random.h"
#include "Particle.h"
#include "Constants.h"
#include "Geometry.h"
#include "Source.h"
#include "Mesh.h"
#include "Tet.h"
#include "HammerTime.h"
#include "EventHandler.h"

using std::vector;
using std::stack;
using std::cout;
using std::endl;


typedef std::shared_ptr<Cell>       Cell_ptr;
typedef std::shared_ptr<Reaction>   React_ptr;
typedef std::shared_ptr<Particle>   Part_ptr;
typedef std::shared_ptr<Material>   Mat_ptr;
typedef std::shared_ptr<surface>    Surf_ptr;
typedef std::shared_ptr<Mesh>       Mesh_ptr;
typedef std::shared_ptr<HammerTime> Time_ptr;
typedef std::shared_ptr<Geometry>   Geom_ptr;
typedef std::shared_ptr<Constants>  Cons_ptr;

typedef std::shared_ptr< EstimatorCollection >  EstCol_ptr;

class Transport {
private:
    int numHis;
    stack<Part_ptr> pstack;
    Cons_ptr constants;
    Geom_ptr geometry; 
    Mesh_ptr mesh;
    Time_ptr timer;
    
    // all estimators in the problem must be accessible as pointers from 
    // transport so as to efficiently end histories 
    vector < EstCol_ptr > estimators;
    
public:
    //constructor
    Transport( Geom_ptr geoin, Cons_ptr consti, Mesh_ptr meshin , Time_ptr timein, vector< EstCol_ptr > estimatorsin );
   ~Transport() {}; 
        //to be altered once input is added
    
    //functions
    //void setup();
    void runTransport();
    void output();
    
    // Estimator interface
    void endHist(); 
    void printEstimators();
};

#endif

