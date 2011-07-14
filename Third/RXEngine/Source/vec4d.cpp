/*
=================================================
vec4d.cpp
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	13/05/2009
=================================================
*/

#include "vec4d.h"
using namespace RX;

// Create/destroy
vec4d::vec4d() 
: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

vec4d::vec4d(double x1, double y1, double z1, double w1) 
:x(x1), y(y1), z(z1), w(w1) 
{
}

vec4d::vec4d(const vec3d &v) 
:x(v.x), y(v.y), z(v.z), w(1.0f) 
{
}

vec4d::vec4d(const vec3d &v, double w) 
:x(v.x), y(v.y), z(v.z), w(w) 
{
}

double vec4d::operator[](int id) const
{
	if(id == 0) return x;
	if(id == 1) return y;
	if(id == 2) return z;
	if(id == 3) return w;
}

// Operators
// Dot product
double vec4d::operator*(const vec4d &v) const
{
	return x*v.x + y*v.y + z*v.z + w*v.w;
}

// Addition/Subtraction
vec4d vec4d::operator+(const vec4d &v) const
{ 	
	return vec4d(x + v.x, y + v.y, z + v.z, w + v.w); 
}
vec4d vec4d::operator+=(const vec4d &v)
{ 
	x += v.x;
	y += v.y;
	z += v.z;	
	w += v.w;
	return *this; 
}

vec4d vec4d::operator-(const vec4d &v) const
{ 
	return vec4d(x - v.x, y - v.y, z - v.z, w - v.w); 
}

vec4d vec4d::operator-=(const vec4d &v)
{ 
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this; 
}

bool vec4d::operator==(const vec4d &v)
{ 
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

// Transform by scalar
vec4d vec4d::operator*=(const double &f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this; 
}
vec4d vec4d::operator/=(const double &f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this; 
}
vec4d vec4d::operator*(const double &f)
{
	return vec4d(x*f, y*f, z*f, w);
}
vec4d vec4d::operator/(const double &f)
{
	return vec4d(x/f, y/f, z/f, w);
}