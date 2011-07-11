/*
=================================================
vec2.cpp
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	06/03/2009
=================================================
*/

#include "vec2.h"
using namespace RX;

// Constructors
vec2::vec2(float x1, float y1) 
:x(x1), y(y1)
{
}

vec2::vec2() 
: x(0), y(0) 
{
}

// Addition/Subtraction
vec2 vec2::operator+(const vec2 &v) const
{ 	
	return vec2(x + v.x, y + v.y); 
}
vec2 vec2::operator+=(const vec2 &v)
{ 
	x += v.x;
	y += v.y;
	return *this; 
}

vec2 vec2::operator-(const vec2 &v) const
{ 
	return vec2(x - v.x, y - v.y); 
}

vec2 vec2::operator-=(const vec2 &v)
{ 
	x -= v.x;
	y -= v.y;
	return *this; 
}