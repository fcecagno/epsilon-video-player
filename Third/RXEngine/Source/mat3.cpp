/*
=================================================
mat3.h
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	30/04/2009
=================================================
*/

#include "mat3.h"
#include "Mathematics.h"

using namespace std;
using namespace RX;

// Create/destroy
mat3::mat3()
{
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			_m[i][j] = 0.0;
		}
	}
}

mat3::mat3(float m00, float m01, float m02,
		   float m10, float m11, float m12,
		   float m20, float m21, float m22)
{
	_m[0][0] = m00; _m[0][1] = m01; _m[0][2] = m02;
	_m[1][0] = m10; _m[1][1] = m11; _m[1][2] = m12;
	_m[2][0] = m20; _m[2][1] = m21; _m[2][2] = m22;
}

mat3::mat3(float mat[9])
{
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			_m[i][j] = mat[j*3 + i];
		}
	}
}

mat3::mat3(vec3 row1, vec3 row2, vec3 row3)
{
	for(int i = 0; i < 3; ++i) {
 		_m[0][i] = row1[i];
		_m[1][i] = row2[i];
		_m[2][i] = row3[i];
	}
}

mat3::~mat3()
{
}

// Matrices
mat3 mat3::operator*(const mat3 &other) const
{
	mat3 r;

	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			r._m[i][j] = 0.0f;
			for(int k = 0; k < 3; ++k) {
				r._m[i][j] += _m[i][k] * other._m[k][j];
			}
		}
	}
	return r;
}
mat3 mat3::operator+(const mat3 &other) const
{
	mat3 r;
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			r._m[i][j] = _m[i][j] + other._m[i][j];
	return r;
}

mat3 mat3::operator-(const mat3 &other) const
{
	mat3 r;
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			r._m[i][j] = _m[i][j] - other._m[i][j];
	return r;
}

mat3 mat3::operator/(const float f) const
{
	mat3 r;
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			r._m[i][j] = _m[i][j]/f;
	return r;
}

mat3 mat3::operator*(const float f) const
{
	mat3 r;
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			r._m[i][j] = _m[i][j]*f;
	return r;
}

mat3 mat3::operator*=(const float f)
{
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			_m[i][j] *= f;
		}
	}
	return *this;
}

mat3 mat3::operator/=(const float f)
{
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			_m[i][j] /= f;
		}
	}
	return *this;
}

mat3 mat3::operator!() const
{
	C3::Math::mat3 m = this->toC3Math();
	m = !m;
	mat3 answer;
	answer.fromC3Math(m);
	return answer;
}

// Transform a vec4
vec3 mat3::transform(const vec3 &v) const
{
	float x = v * vec3(_m[0][0], _m[0][1], _m[0][2]);
	float y = v * vec3(_m[1][0], _m[1][1], _m[1][2]);
	float z = v * vec3(_m[2][0], _m[2][1], _m[2][2]);

	return vec3(x, y, z);
}
vec3 mat3::operator*(const vec3 &v) const
{
	float x = v * vec3(_m[0][0], _m[0][1], _m[0][2]);
	float y = v * vec3(_m[1][0], _m[1][1], _m[1][2]);
	float z = v * vec3(_m[2][0], _m[2][1], _m[2][2]);

	return vec3(x, y, z);
}

void mat3::setIdentity()
{
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			if(i == j) _m[i][j] = 1.0f;
			else _m[i][j] = 0.0f;
		}
	}
}


ostream& RX::operator<<(ostream &out, mat3 m)
{
	out << m._m[0][0] << " " << m._m[0][1] << " " << m._m[0][2] << endl;
	out << m._m[1][0] << " " << m._m[1][1] << " " << m._m[1][2] << endl;
	out << m._m[2][0] << " " << m._m[2][1] << " " << m._m[2][2] << endl;

	return out;
}

istream& RX::operator>>(istream &in, mat3 &m)
{
	in >> m._m[0][0] >> m._m[0][1] >> m._m[0][2];
	in >> m._m[1][0] >> m._m[1][1] >> m._m[1][2];
	in >> m._m[2][0] >> m._m[2][1] >> m._m[2][2];

	return in;
}
