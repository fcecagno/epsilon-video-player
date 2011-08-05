/*
=================================================
vec3d.cpp
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	06/03/2009
=================================================
*/

#include "color.h"
#include "vec3d.h"
#include "vec4.h"
using namespace std;
using namespace RX;

// Static members
const vec3d vec3d::unitX = vec3d(1.0f, 0.0f, 0.0f);
const vec3d vec3d::unitY = vec3d(0.0f, 1.0f, 0.0f);
const vec3d vec3d::unitZ = vec3d(0.0f, 0.0f, 1.0f);

const vec3d vec3d::zero = vec3d(0.0f, 0.0f, 0.0f);
const vec3d vec3d::one = vec3d(1.0f, 1.0f, 1.0f);

// Create/destroy
vec3d::vec3d(const vec3d &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
vec3d::vec3d(const vec4 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
vec3d::vec3d(const color &c)
{
	x = c.r;
	y = c.g;
	z = c.b;
}

vec3d::vec3d(double x1, double y1, double z1) 
:x(x1), y(y1), z(z1) 
{
}

vec3d::vec3d() 
: x(0), y(0), z(0) 
{
}

double vec3d::operator[](int id) const
{
	if(id == 0) return x;
	if(id == 1) return y;
	if(id == 2) return z;
}

// Operators
// Dot product
double vec3d::operator*(const vec3d &v) const
{
	return x*v.x + y*v.y + z*v.z;
}

// Cross product
vec3d vec3d::operator^(const vec3d &v) const
{ 
	double x1 = y*v.z - z*v.y;
	double y1 = z*v.x - x*v.z;
	double z1 = x*v.y - y*v.x;

	return vec3d(x1, y1, z1); 
}

// Addition/Subtraction
vec3d vec3d::operator+(const vec3d &v) const
{ 	
	return vec3d(x + v.x, y + v.y, z + v.z); 
}
vec3d vec3d::operator+=(const vec3d &v)
{ 
	x += v.x;
	y += v.y;
	z += v.z;	
	return *this; 
}

vec3d vec3d::operator-(const vec3d &v) const
{ 
	return vec3d(x - v.x, y - v.y, z - v.z); 
}

vec3d vec3d::operator-=(const vec3d &v)
{ 
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this; 
}
vec3d vec3d::operator=(const vec3d &v)
{ 
	x = v.x;
	y = v.y;
	z = v.z;
	return *this; 
}

// Multiplication/Division by scalar
vec3d vec3d::operator*=(const double &f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this; 
}
vec3d vec3d::operator/=(const double &f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this; 
}

vec3d vec3d::operator/(const double &f)
{
	return vec3d(x/f, y/f, z/f);
}

vec3d RX::operator*(const double &f, const vec3d &v)
{
    return vec3d(v.x*f, v.y*f, v.z*f);
}

vec3d RX::operator*(const vec3d &v, const double &f)
{
    return vec3d(v.x*f, v.y*f, v.z*f);
}

ostream& RX::operator<<(ostream &out, vec3d v)
{
	out << v.x << v.y << v.z;
	return out;
}

istream& RX::operator>>(istream &in, vec3d &v)
{
	double x, y, z;
	in >> x >> y >> z;
	v = vec3d(x, y, z);

	return in;
}