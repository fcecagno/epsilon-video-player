/*
=================================================
vec3.cpp
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	06/03/2009
=================================================
*/

#include "color.h"
#include "vec3.h"
#include "vec4.h"
using namespace std;
using namespace RX;

// Static members
const vec3 vec3::unitX = vec3(1.0f, 0.0f, 0.0f);
const vec3 vec3::unitY = vec3(0.0f, 1.0f, 0.0f);
const vec3 vec3::unitZ = vec3(0.0f, 0.0f, 1.0f);

const vec3 vec3::zero = vec3(0.0f, 0.0f, 0.0f);
const vec3 vec3::one = vec3(1.0f, 1.0f, 1.0f);

// Create/destroy
vec3::vec3(const vec3 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
vec3::vec3(const vec4 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
vec3::vec3(const color &c)
{
	x = c.r;
	y = c.g;
	z = c.b;
}

vec3::vec3(float x1, float y1, float z1) 
:x(x1), y(y1), z(z1) 
{
}

vec3::vec3(const float *v)
:x(v[0]), y(v[1]), z(v[2]) 
{
}

vec3::vec3() 
: x(0), y(0), z(0) 
{
}

float vec3::operator[](int id) const
{
	if(id == 0) return x;
	if(id == 1) return y;
	if(id == 2) return z;
}

// Operators
// Dot product
float vec3::operator*(const vec3 &v) const
{
	return x*v.x + y*v.y + z*v.z;
}

// Cross product
vec3 vec3::operator^(const vec3 &v) const
{ 
	float x1 = y*v.z - z*v.y;
	float y1 = z*v.x - x*v.z;
	float z1 = x*v.y - y*v.x;

	return vec3(x1, y1, z1); 
}

// Addition/Subtraction
vec3 vec3::operator+(const vec3 &v) const
{ 	
	return vec3(x + v.x, y + v.y, z + v.z); 
}
vec3 vec3::operator+=(const vec3 &v)
{ 
	x += v.x;
	y += v.y;
	z += v.z;	
	return *this; 
}

vec3 vec3::operator-(const vec3 &v) const
{ 
	return vec3(x - v.x, y - v.y, z - v.z); 
}

vec3 vec3::operator-=(const vec3 &v)
{ 
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this; 
}
vec3 vec3::operator=(const vec3 &v)
{ 
	x = v.x;
	y = v.y;
	z = v.z;
	return *this; 
}

// Multiplication by scalar
vec3 vec3::operator*=(const float &f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this; 
}

vec3 RX::operator*(const float &f, const vec3 &v)
{
    return vec3(v.x*f, v.y*f, v.z*f);
}

vec3 RX::operator*(const vec3 &v, const float &f)
{
    return vec3(v.x*f, v.y*f, v.z*f);
}

// Division by scalar
vec3 vec3::operator/=(const float &f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this; 
}

vec3 vec3::operator/(const float &f)
{
	return vec3(x/f, y/f, z/f);
}

ostream& RX::operator<<(ostream &out, vec3 v)
{
	out << v.x << v.y << v.z;
	return out;
}

istream& RX::operator>>(istream &in, vec3 &v)
{
	float x, y, z;
	in >> x >> y >> z;
	v = vec3(x, y, z);

	return in;
}