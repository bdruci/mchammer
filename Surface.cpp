#include <cmath>
#include <limits>

#include "Surface.h"

void surface::scoreTally(Part_ptr p , double xs) {
  // for each EstimatorCollection
    // for each attribute
      // get the index of the Estimator to score
      // score the estimator
};

void surface::endTallyHist() {
    for(auto est : estimators) {
        est->endHist();
    }
};

double plane::eval( point p ) {
    return a * p.x  +  b * p.y  +  c * p.z  - d;
}

double plane::distance( point p, point u ) {
    
    double dist = ( d - a * p.x - b * p.y - c * p.z ) / ( a * u.x + b * u.y + c * u.z );
    if ( dist > 0.0 ) { return dist; }
    else { return std::numeric_limits<double>::max(); }
    
}

point plane::getNormal(point pt){
  if(eval(pt) == 0) {
    point normal(a , b , c);
    return( normal / std::sqrt(normal * normal) );
  }
  else {
    // if the point is not on the surface, return a null vector
    // client must check for this condition
    point p(0 , 0 ,0);
    return(p);
  }
}

double sphere::eval( point p ) {
    return std::pow( p.x - x0, 2 ) + std::pow( p.y - y0, 2 ) + std::pow( p.z - z0, 2 )  - rad*rad;
}

double sphere::distance( point p, point u ) {
    // difference between each coordinate and current point
    point q( p.x - x0, p.y - y0, p.z - z0 );
    
    double b = 2.0 * ( q.x * u.x  +  q.y * u.y  +  q.z * u.z);
    double c = eval( p );
    
    return Utility::quadSolve( 1.0, b, c );   
}

point sphere::getNormal(point pt){
  // check if the crossing point is on the surface
  if(eval(pt) == 0) {
    // make a point at the center of the sphere
    point center(x0 , y0 , z0);
    // make an outward facing surface normal ray from the center to the edge
    ray normal(pt , center - pt);
    return(normal.dir);
  }
  else {
    // if the point is not on the surface, return a null vector
    // client must check for this condition
    point p(0 , 0 ,0);
    return(p);
  }
}

//Effects: Returns the solution to the equation of the equation at a point
double cylinder::eval( point p ) {
  //If along x-axis 
  if(dir.x != 0) {
    //Equ: (y-y0)^2 + (z-z0)^2 - r^2 = s
    //Return S;
    return std::pow(p.y - y0, 2) + std::pow(p.z - z0, 2) - rad*rad;
  }
  //If along y-axis 
  if(dir.y != 0) {
    //Equ: (x-x0)^2 + (z-z0)^2 - r^2 = s
    return std::pow(p.x - x0, 2) + std::pow(p.z - z0, 2) - rad*rad;
  }
  //If along z-axis 
  if(dir.z != 0) {
    //Equ: (x-x0)^2 + (y-y0)^2 - r^2 = s
    return std::pow(p.x - x0, 2) + std::pow(p.y - y0, 2) - rad*rad;
  }
}

//Effects: returns the distance from the point p on its path u to the surface
double cylinder::distance (point p, point u ) {
  // difference between each coordinate and current point
  point q( p.x - x0, p.y - y0, p.z - z0 );

  //If along x-axis
  if(dir.x != 0) {
    //Equ: (y-y0)^2 + (z-z0)^2 - r^2 = s

    double a = ( std::pow(u.y, 2) + std::pow(u.z,2) );
    double b = 2.0 * ( q.y * u.y  +  q.z * u.z);
    double c = eval(p);

    //special case - line on cylinder
    if(a == 0 && c == 0) {
      return 0;
    }

    return Utility::quadSolve(a,b,c);
  }
  //If along y-axis
  if(dir.y != 0) {
    //Equ: (x-x0)^2 + (z-z0)^2 - r^2 = s

    double a = ( std::pow(u.x, 2) + std::pow(u.z,2) );
    double b = 2.0 * ( q.x * u.x  +  q.z * u.z);
    double c = eval(p);

    //special case - line on cylinder
    if(a == 0 && c == 0) {
      return 0;
    }

    return Utility::quadSolve(a,b,c);
  }
  //If along z-axis
  if(dir.z != 0) {
    //Equ: (x-x0)^2 + (y-y0)^2 - r^2 = s

    double a = ( std::pow(u.x, 2) + std::pow(u.y,2) );
    double b = 2.0 * ( q.x * u.x  +  q.y * u.y);
    double c = eval(p);

    //special case - line on cylinder
    if(a == 0 && c == 0) {
      return 0;
    }

    return Utility::quadSolve(a,b,c);
 }
}

//Effects: returns the normal vector of the surface at the point or nullptr if
//point fails to be on the surface
point  cylinder::getNormal( point p ) {
  // check if the crossing point is on the surface
  if(eval(p) == 0) {
    // the gradient vector of the cylinder, general for all orientations
    point normal( 2.0*(p.x - x0), 2.0*(p.y - y0), 2.0*(p.z - z0) );
    
    //return normal;
    //Fix for the axis centered cylinders
    if(dir.x != 0){
      normal.x = 0;
    }
    else if(dir.y != 0){
      normal.y = 0;
    }
    else if(dir.z != 0){
      normal.z = 0;
    }

    // return the unit normal vector
    return( normal / std::sqrt(normal * normal) );
  }
  else {
    // if the point is not on the surface, return a null vector
    // client must check for this condition
    point p(0 , 0 ,0);
    return(p);
  }
}