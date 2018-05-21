#ifndef _SURFACE_HEADER_
#define _SURFACE_HEADER_

#include <string>

#include "Point.h"
#include "Utility.h"
#include "EstimatorCollection.h"

typedef std::shared_ptr<EstimatorCollection> EstCol_ptr;

class surface {
private:
    std::string surface_name;
    
    // EstimatorCollections
    vector< EstCol_ptr > estimators;
    
public:
    surface( std::string label ) : surface_name(label) {};
    ~surface() {};

    
    virtual std::string name() { return surface_name; };
    
    // returns a normal vector given a point on the surface
    // and a crossing direction
    virtual point  getNormal(point p)           = 0;
    virtual double eval( point p )              = 0;
    virtual double distance( point p, point u ) = 0;
    
    // estimator interface
    void addEstimator( EstCol_ptr newEstimator );
    std::vector< EstCol_ptr > getEstimators() { return estimators; };
};

class plane : public surface {
private:
    double a, b, c, d;
public:
    plane( std::string label, double p1, double p2, double p3, double p4 ) : surface(label), a(p1), b(p2), c(p3), d(p4) {};
    ~plane() {};
    
    point  getNormal(point p);
    double eval( point p );
    double distance( point p, point u );
};

class sphere : public surface {
private:
    double x0, y0, z0, rad;
public:
    sphere( std::string label, double p1, double p2, double p3, double p4 ) : surface(label), x0(p1), y0(p2), z0(p3), rad(p4) {};
    ~sphere() {};
    
    point  getNormal(point p);
    double eval( point p );
    double distance( point p, point u );
};

//Currently only takes cylinders along a x/y/z axis
class xCylinder : public surface {
private:
    double y0, z0, rad;
public:
    xCylinder( std::string label, double y_in, double z_in, double rad_in)
            : surface(label), y0(y_in), z0(z_in), rad(rad_in) { 
        assert(rad > 0);
    };
    ~xCylinder() {};

    point  getNormal(point p);
    double eval( point p );
    double distance( point p, point u );
};

class yCylinder : public surface {
private:
    double x0, z0, rad;
public:
    yCylinder( std::string label, double x_in, double z_in, double rad_in)
            : surface(label), x0(x_in), z0(z_in), rad(rad_in) { 
        assert(rad > 0);
    };
    ~yCylinder() {};

    point  getNormal(point p);
    double eval( point p );
    double distance( point p, point u );
};

class zCylinder : public surface {
private:
    double x0, y0, rad;
public:
    zCylinder( std::string label, double x_in, double y_in, double rad_in)
            : surface(label), x0(x_in), y0(y_in), rad(rad_in) { 
        assert(rad > 0);
    };
    ~zCylinder() {};

    point  getNormal(point p);
    double eval( point p );
    double distance( point p, point u );
};

#endif
