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
using namespace std;

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

// Division by scalar
vec2d vec2d::operator/=(const double &f)
{
	x /= f;
	y /= f;
	return *this; 
}

vec2d vec2d::operator/(const double &f)
{
	return vec2d(x/f, y/f);
}

vec2d RX::operator*(const double &f, const vec2d &v)
{
    return vec2d(v.x*f, v.y*f);
}

vec2d RX::operator*(const vec2d &v, const double &f)
{
    return vec2d(v.x*f, v.y*f);
}

ostream& RX::operator<<(ostream &out, vec2d v)
{
	out << v.x << v.y;
	return out;
}

istream& RX::operator>>(istream &in, vec2d &v)
{
	double x, y, z;
	in >> x >> y >> z;
	v = vec2d(x, y);

	return in;
}