/*
=================================================
vec4.cpp
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	13/05/2009
=================================================
*/

#include "vec4.h"
using namespace RX;

// Create/destroy
vec4::vec4() 
: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

vec4::vec4(float x1, float y1, float z1, float w1) 
:x(x1), y(y1), z(z1), w(w1) 
{
}

vec4::vec4(const vec3 &v) 
:x(v.x), y(v.y), z(v.z), w(1.0f) 
{
}

vec4::vec4(const vec3 &v, float w) 
:x(v.x), y(v.y), z(v.z), w(w) 
{
}

float vec4::operator[](int id) const
{
	if(id == 0) return x;
	if(id == 1) return y;
	if(id == 2) return z;
	if(id == 3) return w;
}

// Operators
// Dot product
float vec4::operator*(const vec4 &v) const
{
	return x*v.x + y*v.y + z*v.z + w*v.w;
}

// Addition/Subtraction
vec4 vec4::operator+(const vec4 &v) const
{ 	
	return vec4(x + v.x, y + v.y, z + v.z, w + v.w); 
}
vec4 vec4::operator+=(const vec4 &v)
{ 
	x += v.x;
	y += v.y;
	z += v.z;	
	w += v.w;
	return *this; 
}

vec4 vec4::operator-(const vec4 &v) const
{ 
	return vec4(x - v.x, y - v.y, z - v.z, w - v.w); 
}

vec4 vec4::operator-=(const vec4 &v)
{ 
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this; 
}

bool vec4::operator==(const vec4 &v)
{ 
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

// Transform by scalar
vec4 vec4::operator*=(const float &f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this; 
}
vec4 vec4::operator/=(const float &f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this; 
}
vec4 vec4::operator*(const float &f)
{
	return vec4(x*f, y*f, z*f, w);
}
vec4 vec4::operator/(const float &f)
{
	return vec4(x/f, y/f, z/f, w);
}