/*
	Author: Blake
	Date: 11/8/17
	Req. Files: Material.cpp, Particle.h
*/

#include "Material.h"

void Material::addNuclide( Nuclide_ptr newNuclide, double atomFrac )
{
	nuclides.push_back( std::make_pair( newNuclide, atomFrac ) );
	return;
}

double Material::getMicroXS( Part_ptr p ) 
{
  double xs = 0.0;
  for ( auto n : nuclides ) 
  { 
    // first is pointer to nuclide, second is atomic fraction
    xs += n.first->getTotalXS( p ) * n.second;
  }
  return xs;
}

double Material::getMacroXS( Part_ptr p ) 
{
  return getAtomDensity() * getMicroXS( p );
}

// randomly Urand a nuclide based on total cross sections and atomic fractions
Nuclide_ptr Material::sampleNuclide( Part_ptr p ) 
{
  double u = getMicroXS( p ) * rng->Urand();
  double s = 0.0;

  for ( auto n : nuclides ) 
  {
    // first is pointer to nuclide, second is atomic fraction
    s += n.first->getTotalXS( p ) * n.second;
    if ( s > u ) { return n.first; }
  }
  assert( false ); // should never reach here
  return nullptr;
}

// function that Urands an entire collision: Urand nuclide, then its reaction, 
// and finally process that reaction with input pointers to the working particle p
// and the particle bank
void Material::sampleCollision( Part_ptr p, std::stack< Part_ptr > &bank ) {
  // first Urand nuclide
  Nuclide_ptr  N = sampleNuclide( p );

  // now get the reaction
  Reaction_ptr R = N->sampleReaction( p );

  // finally process the reaction
  R->sample( p, bank );
}
