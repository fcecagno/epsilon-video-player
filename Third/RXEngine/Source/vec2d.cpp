/*
=================================================
vec2d.cpp
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	06/03/2009
=================================================
*/

#include "vec2d.h"
using namespace RX;

// Constructors
vec2d::vec2d(double x1, double y1) 
:x(x1), y(y1)
{
}

vec2d::vec2d() 
: x(0), y(0) 
{
}

// Addition/Subtraction
vec2d vec2d::operator+(const vec2d &v) const
{ 	
	return vec2d(x + v.x, y + v.y); 
}
vec2d vec2d::operator+=(const vec2d &v)
{ 
	x += v.x;
	y += v.y;
	return *this; 
}

vec2d vec2d::operator-(const vec2d &v) const
{ 
	return vec2d(x - v.x, y - v.y); 
}

vec2d vec2d::operator-=(const vec2d &v)
{ 
	x -= v.x;
	y -= v.y;
	return *this; 
}