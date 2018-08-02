#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <iostream>
#include <vector>

#include "../Utility.h"
#include "../Catch.h"
#include "../Distribution.h"
#include "../Random.h"

TEST_CASE( "UniformContinuous", "[distribution]" ) {

  std::vector<double> uniNums = { 1. }; 
  activateTesting(uniNums);

  // test sample for this derived type
  SECTION ( " sample simple " ) {

    uniformContinuous uni( 0, 1 );

    REQUIRE( uni.sample() == 1);
    }

  std::vector<double> uniNums2 = { .25, .34, .64 }; 
  activateTesting(uniNums2);

  SECTION ( " sample complex " ) {

    uniformContinuous uni( 9, 11 );

    REQUIRE( uni.sample() == 9.5);
    REQUIRE( uni.sample() == 9.68);
    REQUIRE( uni.sample() == 10.28);
    }
}

TEST_CASE( "ExponentialContinuous", "[distribution]" ) {

  std::vector<double> expNums = { 1., .5 }; 
  activateTesting(expNums);

  // test sample for this derived type
  SECTION ( " sample " ) {

    exponentialContinuous Exp( 0, 1 );

    REQUIRE( Utility::FloatZero(Exp.sample()));
    REQUIRE( Utility::FloatEqual(Exp.sample(),.6931471806, .001));
    }
}

TEST_CASE( "Gaussian", "[distribution]" ) {

  std::vector<double> normNums = { .6065306597, .25, .6065306597, .5}; 
  activateTesting(normNums);

  // test sample for this derived type
  SECTION ( " sample standard " ) {

    gaussian Norm(0,1);

    REQUIRE( Utility::FloatZero(Norm.sample()));
    REQUIRE( Utility::FloatEqual(Norm.sample(), -1, .01));
  }

  std::vector<double> normNums2 = { .6065306597, .25, .6065306597, .5}; 
  activateTesting(normNums2);

  // test sample for this derived type
  SECTION ( " sample " ) {

    gaussian Norm(.34,3);

    REQUIRE( Utility::FloatEqual(Norm.sample(),.34, .001) );
    REQUIRE( Utility::FloatEqual(Norm.sample(),-2.66, .001));
  }
}

TEST_CASE( "IsotropicDirection", "[distribution]") {

  // test sample for this derived type
  SECTION ( " sample " ) {
    isotropicDirection randDir;

    std::vector<double> dirNums = { .5, .25 }; 
    activateTesting(dirNums);

    point correct(0,0,1);
    REQUIRE(Utility::PointEqual(randDir.sample(), correct, .001));  
  }
}

TEST_CASE( "CuboidGeometry", "[distribution]") {

  // test sample for this derived type
  SECTION ( " sample " ) {

    cuboidGeometry cuboid( 0, 0, 0, 1, 2, 3 );
    cuboidGeometry cube( 0, 0, 0, 10 );

    std::vector<double> cubeNums = { 1.5 }; 
    activateTesting(cubeNums);

    point correctoid(1,2,3);
    point correct(10,10,10);
    
    REQUIRE(  cuboid.sample() == correctoid);
    REQUIRE(  cube.sample() == correct);
  }
}

TEST_CASE("SphereicalGeometry", "[distribution]") {
    // test sample for this derived type
    SECTION ( " sample sphere " ) {

      std::vector<double> sphereNums = { 0.5, 0.25, 0.75 };
      activateTesting(sphereNums);

      sphericalGeometry sphere( 0, 0, 0, 0, 10 );

      point correct(0,-5,5);
      REQUIRE( sphere.sample() == correct);
    }

    SECTION ( " sample shell " ) {

      std::vector<double> shellNums = { 0.5, 0.5, 0.25 };
      activateTesting(shellNums);

      sphericalGeometry shell( -1, -1, 10, 9, 10 );

      point Correct(8,8,19.5);
      REQUIRE( shell.sample() == Correct);
    }
}

TEST_CASE( "XAnnular", "[distribution]" ) {
    // test sample for this derived type
    SECTION ( " sample " ) {

      std::vector<double> xcylNums = { 1, 0.25, 0.125 };
      activateTesting(xcylNums);

      xAnnularGeometry xCyl(0, 0, 0, 1, 1, 5);

      point correct(1,0,2);
      REQUIRE(Utility::PointEqual(xCyl.sample(), correct, .001)); 
   }
}

TEST_CASE( "YAnnular", "[distribution]" ) {
    // test sample for this derived type
    SECTION ( " sample " ) {

      std::vector<double> ycylNums = { 1, 0.25, 0.125 };
      activateTesting(ycylNums);

      yAnnularGeometry yCyl(0, 0, 0, 1, 1, 5);

      point correct(0,1,2);
      REQUIRE(Utility::PointEqual(yCyl.sample(), correct, .001));  
    }
}

TEST_CASE( "ZAnnular", "[distribution]" ) {
    // test sample for this derived type
    SECTION ( " sample " ) {

      std::vector<double> zcylNums = { 1, 0.25, 0.125 };
      activateTesting(zcylNums);

      zAnnularGeometry zCyl(0, 0, 0, 1, 1, 5);

      point correct(0,2,1);
      REQUIRE(Utility::PointEqual(zCyl.sample(), correct, .001));  
    }
}

TEST_CASE("Cylinder", "[distribution]" ) {
    // test sample for this derived type
    SECTION ( " sample " ) {

      std::vector<double> cylNums = { 1, 0.55, 0.45 };
      activateTesting(cylNums);

      xAnnularGeometry cyl(0, 0, 0, 5, 0, 10);

      point correct(5,1,-1);
      REQUIRE(Utility::PointEqual(cyl.sample(), correct, .001)); 
    }
}

TEST_CASE( "Catagorical", "[distribution]" ) {
    // test sample for this derived type
    SECTION ( " sample " ) {

      std::vector<double> catNums = { .2, .57, .8, .84 };
      activateTesting(catNums);

      std::vector<char> elements = { 'a', 'b', 'c', 'd' };
      std::vector<double> probs = { 3, 4, 3, 2 };

      catagoricalWeighted<char> catDist(elements, probs);
      REQUIRE( catDist.sample() == 'a' );
      REQUIRE( catDist.sample() == 'b' );
      REQUIRE( catDist.sample() == 'c' );
      REQUIRE( catDist.sample() == 'd' );
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
To test the accuaracy of the algorithms themselves their output can be visually inspected 
using matlab plots. The following scripts can be copied into matlab.


***************************************************************************************************
%Uniform Continuous Sampling 
close all;
clear;

%C++ code
% double width = end - start;

% double inbetween = (Urand() * width) + start;

% return inbetween;

%Number of points generated
n = 1000;

%Seed the plot
Start = 5;
End = 10;

%Compute the random points
width = End - Start;
spot = (rand(1,n) .* width) + Start;

%Plot the point for visual
figure(1);
hold on;
scatter(Start, 0, 'r', 'filled');
scatter(End, 0, 'r', 'filled');
scatter(spot, zeros(1,n), 'b');
hold off;
***************************************************************************************************


***************************************************************************************************
%Exponential Continuous Sampling 
close all;
clear;

%C++ code
%   double inbetween = (-log(Urand())/lambda) + start;
% 
%   return inbetween;

%Number of points generated
n = 10000;

%Seed for dist
Start = 5;
lambda = 2;

%Calculate rand spot 
%pdf = L*e^(-L*x)
%integrate from zero to x ~ 1-e^(-L*x)
%set equal to rand and solve for x ~ x = -ln(-rand + 1)/L
%-rand+1 == rand so ~ x = -ln(rand)/L
spot = (-log(rand(1,n)) ./ lambda) + Start;

%plot the points for visual check
figure(1);
histogram(spot, 'BinMethod', 'auto');
***************************************************************************************************


***************************************************************************************************
%Gaussian sampling
clear;
close all;

%C++ code
%   //Box-Muller method to get standard normally distributed point N(0,1)
%   double norm = sqrt(-2*log(Urand())) * cos(2*pi*Urand());
% 
%   //Shift point using the location-scale property of standard normal fxns
%   norm = norm*sigma + mu;
% 
%   return norm;
    
%Number of points generated
n = 10000;

%Seed for first dist
mu1 = 0;
std1 = 1;

%Seed for second dist
mu2 = 0;
std2 = 10;

r1 = rand(1,n);
r2 = rand(1,n);

%Use Box-Muller method to get both points
Z0 = sqrt(-2.*log(r1)).*cos(2.*pi.*r2);
Z1 = sqrt(-2.*log(r1)).*sin(2.*pi.*r2);

%Shift the plots points using location-scale property
Z0 = Z0.*std1 + mu1;
Z1 = Z1.*std2 + mu2;

%plot both side by side (User needs to play with upper y-axis bound if
%changes are made to n value)
figure(1);
subplot(1,2,1);
histogram(Z0, 'BinMethod', 'scott');
axis([-5.*std1,5.*std1,0, 1000]);

subplot(1,2,2);
histogram(Z1, 'BinMethod', 'scott');
axis([-5.*std2,5.*std2,0, 1000]);
***************************************************************************************************


***************************************************************************************************
%Isotropic direction sampling
clear; 
close all;

%C++ code
%   double mu = 2 * Urand() - 1;
% 
%   double phi = 2 * pi*Urand();
%   double omegaX=mu; 
%   double omegaY=sqrt(1-mu*mu)*cos(phi); //sin(acos(mu)) 
%   double omegaZ=sqrt(1-mu*mu)*sin(phi);
%   point dir = point(omegaX,omegaY,omegaZ);

%   return dir;

%Number of points generated
n = 10000;

%Pick a random point between -1 and 1
mu = 2.*rand(1,n)-1;

%Pick a random angle between 0 and 2*pi
phi = 2.*pi.*rand(1,n);

%X is mu and use trig to get other two sides
x = mu;
y = sqrt(1-mu.*mu).*cos(phi);
z = sqrt(1-mu.*mu).*sin(phi);

%plot in 3d for a visual check
figure(1);
scatter3(x,y,z);
axis([-1,1,-1,1]);
axis square;
***************************************************************************************************


***************************************************************************************************
%Cuboid distribution samping
clear;
close all;

%C++ code
%   double x, y, z;
%   x = (Urand()-0.5)*xaxis;
%   y = (Urand()-0.5)*yaxis;
%   z = (Urand()-0.5)*zaxis;
% 
%   point pos(x + x0, y + y0, z + z0);
%
%   return pos;

%Number of points generated
n = 100000;

%Seed the cube 
xaxis = 3;
yaxis = 2;
zaxis = 3;

x0 = 0;
y0 = 0;
z0 = 0;

%Change rand to [-.5,.5] to pick point inside box from origin
x = (rand(1,n) - 0.5).*xaxis;
y = (rand(1,n) - 0.5).*yaxis;
z = (rand(1,n) - 0.5).*zaxis;

%Shift to correct origin
x = x + x0;
y = y + y0;
z = z + z0;

%Plot points to provide a visual
figure(1);
scatter3(x,y,z);
***************************************************************************************************


***************************************************************************************************
%testing for rejection sampling sphere
clear;
close all;

%C++ code
% bool reject = true;
% while(reject)
% {
%   x = (2*Urand()-1)*radOuter;
%   y = (2*Urand()-1)*radOuter;
%   z = (2*Urand()-1)*radOuter;
%   double dist = (x*x+y*y+z*z);
%   if(dist < radOuter*radOuter)
%   {
%     reject = false;
%   }

%Number of points
p = 50000;

%Seed sphere charateristics
x0 = 0;
y0 = 0;
z0 = 0;
radOut = 5;

%Initialize in proper scope
x = 1;
y = 2;
z = 0;
xs = zeros(1,p);
ys = zeros(1,p);
zs = zeros(1,p);

%Loop for total points
for k = (1:p)
    %Guess a point in box and check to see if its in the sphere
    n = 0;
    while n ~= 1
        r = rand(1,3);
    
        x = ( 2.*r(1,1) - 1) .* radOut;
        y = ( 2.*r(1,2) - 1) .* radOut;
        z = ( 2.*r(1,3) - 1) .* radOut;
    
        dist = (x.^2 + y.^2 + z.^2);
        if dist < (radOut*radOut)
            n = 1;
        end
    
    end
    xs(1,k) = x;
    ys(1,k) = y;
    zs(1,k) = z;
end

%Add in origin 
xs = xs + x0;
ys = ys + y0;
zs = zs + z0;

%Plot for visual
figure(1);
scatter3(xs,ys,zs);
axis([-radOut, radOut, -radOut, radOut]);
axis square;

%%
%Test equation sampling shell

%C++ code
% double thickness = (radOuter - radInner) * Urand();
% point origin(x0,y0,z0);
% isotropicDirection unitRand;
% 
% point unitPoint = unitRand.sample();
% //Scale the random point to the thickness of the shell
% unitPoint = unitPoint * thickness;
% //Add the inner rad so point is inside of shell
% unitPoint = unitPoint + radInner;
% //Add the origin to place the point in the correct 3-d  space
% unitPoint = unitPoint + origin;
% 
% return unitPoint;

%Number of points generated
n = 50000;

%Seed shell characteristics
X0 = 0;
Y0 = 0;
Z0 = 0;
rin = 2;
rout = 5.5;

%Calculate random distance from the origin (random point in shell plus
%inner radius)
radius = rin + rand(1,n) .* (rout - rin);

%Use Isotropic Direction formula to get random point with radius one and
%scale to radius
mu = 2.*rand(1,n)-1;
phi = 2.*pi.*rand(1,n);
x = radius .* mu;
y = radius .* sqrt(1-mu.*mu).*cos(phi);
z = radius .* sqrt(1-mu.*mu).*sin(phi);

%Add in origin
x = x + X0;
y = y + Y0;
z = z + Z0;

%Plot for visual
figure(2);
scatter3(x,y,z);
axis([0, rout, -rout, 0]);

figure(3);
scatter3(x,y,z);
axis([0, rout, -rout, rout]);

figure(4);
hold on;
scatter3(x,y,z);
axis([-rout, rout, -rout, rout]);
axis square;
***************************************************************************************************


***************************************************************************************************
%test for Annular equation
clear;
close all;

%C++ code
% x = height*Urand();
%     
% double phi = 2 * pi*Urand();
% double dist = std::sqrt(radInner*radInner + (radOuter*radOuter - radInner*radInner)*Urand()); 
% y = dist*cos(phi);
% z = dist*sin(phi);
        
%Number of points generated
n = 50000;

%Seed characteristics of annulus
x0 = 0;
y0 = 0;
z0 = 0;
height = 10;
r1 = 2.5; r2 = 5;

%Calculate random radius in annulus and an angle
r = sqrt(r1^2+(r2^2-r1^2)*rand(1,n));
t = 2*pi*rand(1,n);

%Two coordinates from radius and angle - other along the height of the annulus
x = r.*cos(t);
y = r.*sin(t);
z = height*rand(1,n);

%Add in the origin
x = x + x0;
y = y + y0;
z = z + z0;

%Plot for visual
figure(1);
scatter3(x,y,z,'b.')
axis equal

%%
%Test for cylindrical using rejection 

%C++ code
% bool reject = true;
% while(reject)
% {
%   y = (2*Urand()-1)*radOuter;
%   z = (2*Urand()-1)*radOuter;
%   double dist = sqrt(y*y+z*z);
%   if(dist < radOuter)
%     reject = false;
% }

%Number of points generated
p = 50000;

%Seed characteristics of cylinder
X0 = 0;
Y0 = 0;
Z0 = 0;
H = 10;
rad = 5;

%Initialize in proper scope
X = 0; Y = 0;
xs = zeros(1,p);
ys = zeros(1,p);
zs = zeros(1,p);

%Loop through all points
for k = (1:p)
    %Guess a point in box and check to see if its in the cylinder
    n = 0;
    while n ~= 1
        r = rand(1,2);
   
        x = ( 2.*r(1,1) - 1) .* rad;
        y = ( 2.*r(1,2) - 1) .* rad;
    
        dist = (x.^2 + y.^2);
        if dist < (rad*rad)
            n = 1;
        end
    end
    xs(1,k) = x + X0;
    ys(1,k) = y + Y0;  
end
zs = rand(1,p).*H + Z0;


%Plot for visual
figure(2);
scatter3(xs,ys,zs);
axis square;
***************************************************************************************************
*/