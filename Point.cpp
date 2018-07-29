#include <cmath>

#include "Point.h"


point point::operator + (point const &pt) {
  point p(pt.x + x , pt.y + y, pt.z + z);
  return(p);
}

point point::operator - (point const &pt) {
  point p(x - pt.x  , y - pt.y, z - pt.z);
  return(p);
}

double point::operator * (point const &pt) {
  return( x * pt.x + y * pt.y + z * pt.z );
}

bool point::operator == (point const &pt) {
  if (x == pt.x && y == pt.y && z ==pt.z)
    return true;
  return false;
}

bool point::operator != (point const &pt) {
  if (*this == pt)
    return false;
  return true;
}

point point::operator / (double scalar) {
  point p(x / scalar  , y / scalar , z / scalar);
  return(p);
}

point point::operator * (double scalar) {
  point p(x * scalar  , y * scalar , z * scalar);
  return(p);
}

point point::operator + (double scalar) {
  point p(scalar + x , scalar + y, scalar + z);
  return(p);
}

ray::ray( point p, point d ) : pos(p), dir(d) {
    
    // normalize direciton vector
    double norm = 1.0 / std::sqrt( dir.x * dir.x  +  dir.y * dir.y  +  dir.z * dir.z );
    dir.x *= norm; dir.y *= norm; dir.z *= norm;
    
}