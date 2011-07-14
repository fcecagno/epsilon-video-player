/*
=================================================
mat3.h
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	30/04/2009
=================================================
*/

#include "mat3d.h"
#include "Mathematics.h"

using namespace std;
using namespace RX;

// Create/destroy
mat3d::mat3d()
{
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			_m[i][j] = 0.0;
		}
	}
}

mat3d::mat3d(double m00, double m01, double m02,
		     double m10, double m11, double m12,
		     double m20, double m21, double m22)
{
	_m[0][0] = m00; _m[0][1] = m01; _m[0][2] = m02;
	_m[1][0] = m10; _m[1][1] = m11; _m[1][2] = m12;
	_m[2][0] = m20; _m[2][1] = m21; _m[2][2] = m22;
}

mat3d::mat3d(double mat[9])
{
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			_m[i][j] = mat[j*3 + i];
		}
	}
}

mat3d::mat3d(vec3d row1, vec3d row2, vec3d row3)
{
	for(int i = 0; i < 3; ++i) {
 		_m[0][i] = row1[i];
		_m[1][i] = row2[i];
		_m[2][i] = row3[i];
	}
}

mat3d::~mat3d()
{
}

// Multiply matrices
mat3d mat3d::operator*(const mat3d &other) const
{
	mat3d r;

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

mat3d mat3d::operator/=(const double d)
{
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			_m[i][j] /= d;
		}
	}
	return *this;
}

mat3d mat3d::operator!() const
{
	C3::Math::mat3 m = this->toC3Math();
	m = !m;
	mat3d answer;
	answer.fromC3Math(m);
	return answer;
}

// Transform a vec4
vec3d mat3d::transform(const vec3d &v) const
{
	double x = v * vec3d(_m[0][0], _m[0][1], _m[0][2]);
	double y = v * vec3d(_m[1][0], _m[1][1], _m[1][2]);
	double z = v * vec3d(_m[2][0], _m[2][1], _m[2][2]);

	return vec3d(x, y, z);
}
vec3d mat3d::operator*(const vec3d &v) const
{
	double x = v * vec3d(_m[0][0], _m[0][1], _m[0][2]);
	double y = v * vec3d(_m[1][0], _m[1][1], _m[1][2]);
	double z = v * vec3d(_m[2][0], _m[2][1], _m[2][2]);

	return vec3d(x, y, z);
}

void mat3d::setIdentity()
{
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			if(i == j) _m[i][j] = 1.0f;
			else _m[i][j] = 0.0f;
		}
	}
}


ostream& RX::operator<<(ostream &out, mat3d m)
{
	out << m._m[0][0] << " " << m._m[0][1] << " " << m._m[0][2] << endl;
	out << m._m[1][0] << " " << m._m[1][1] << " " << m._m[1][2] << endl;
	out << m._m[2][0] << " " << m._m[2][1] << " " << m._m[2][2] << endl;

	return out;
}

istream& RX::operator>>(istream &in, mat3d &m)
{
	in >> m._m[0][0] >> m._m[0][1] >> m._m[0][2];
	in >> m._m[1][0] >> m._m[1][1] >> m._m[1][2];
	in >> m._m[2][0] >> m._m[2][1] >> m._m[2][2];

	return in;
}
