/*
=================================================
mat4.h
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	30/04/2009
=================================================
*/

#include "mat4.h"
#include "Mathematics.h"

using namespace std;
using namespace RX;

// Create/destroy
mat4::mat4()
{
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			_m[i][j] = 0.0f;
		}
	}
}

mat4::mat4(float mat[16])
{
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			_m[i][j] = mat[j*4 + i];
		}
	}
}

mat4::mat4(vec4 row1, vec4 row2, vec4 row3, vec4 row4)
{
	for(int i = 0; i < 4; ++i) {
 		_m[0][i] = row1[i];
		_m[1][i] = row2[i];
		_m[2][i] = row3[i];
		_m[3][i] = row4[i];
	}
}

mat4::mat4(float m00, float m01, float m02, float m03,
		   float m10, float m11, float m12, float m13,
		   float m20, float m21, float m22, float m23,
		   float m30, float m31, float m32, float m33)
{
	_m[0][0] = m00; _m[0][1] = m01; _m[0][2] = m02; _m[0][3] = m03;
	_m[1][0] = m10; _m[1][1] = m11; _m[1][2] = m12;	_m[1][3] = m13;
	_m[2][0] = m20; _m[2][1] = m21; _m[2][2] = m22;	_m[2][3] = m23;
	_m[3][0] = m30; _m[3][1] = m31; _m[3][2] = m32;	_m[3][3] = m33;
}

mat4::~mat4()
{
}

// Multiply matrices
mat4 mat4::operator*(const mat4 &other) const
{
	mat4 r;

	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			r._m[i][j] = 0.0f;
			for(int k = 0; k < 4; ++k) {
				r._m[i][j] += _m[i][k] * other._m[k][j];
			}
		}
	}
	return r;
}

mat4 mat4::operator+=(const mat4 &other)
{
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			_m[i][j] += other._m[i][j];
		}
	}
	return *this;
}

mat4 mat4::operator-=(const mat4 &other)
{
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			_m[i][j] -= other._m[i][j];
		}
	}
	return *this;
} 

mat4 mat4::operator!() const
{
	/*
	C3::Math::mat4 m = this->toC3Math();
	m = !m;
	mat4 answer;
	answer.fromC3Math(m);
	return answer;
	*/
	return mat4();
}

// Transform a vec4
vec4 mat4::operator*(const vec4 &v) const
{
	float x = v * vec4(_m[0][0], _m[0][1], _m[0][2], _m[0][3]);
	float y = v * vec4(_m[1][0], _m[1][1], _m[1][2], _m[1][3]);
	float z = v * vec4(_m[2][0], _m[2][1], _m[2][2], _m[2][3]);
	float w = v * vec4(_m[3][0], _m[3][1], _m[3][2], _m[3][3]);

	return vec4(x, y, z, w);
}
vec4 mat4::transform(const vec4 &v) const
{
	float x = v * vec4(_m[0][0], _m[0][1], _m[0][2], _m[0][3]);
	float y = v * vec4(_m[1][0], _m[1][1], _m[1][2], _m[1][3]);
	float z = v * vec4(_m[2][0], _m[2][1], _m[2][2], _m[2][3]);
	float w = v * vec4(_m[3][0], _m[3][1], _m[3][2], _m[3][3]);

	return vec4(x, y, z, w);
}

// Create matrices
void mat4::setView(vec3 position, vec3 target, vec3 up)
{
	vec3 n = (position - target).normalized();
	vec3 u = (up ^ n).normalized();
	vec3 v = (n ^ u).normalized();
	vec3 Oc = position;

	_m[0][0] = u.x;
	_m[0][1] = u.y;
	_m[0][2] = u.z;

	_m[1][0] = v.x;
	_m[1][1] = v.y;
	_m[1][2] = v.z;

	_m[2][0] = n.x;
	_m[2][1] = n.y;
	_m[2][2] = n.z;

	_m[0][3] = -(Oc * u);
	_m[1][3] = -(Oc * v);
	_m[2][3] = -(Oc * n);

	_m[3][0] = 0.0f;
	_m[3][1] = 0.0f;
	_m[3][2] = 0.0f;
	_m[3][3] = 1.0f;
}

void mat4::setOrthoProj(float x0, float x1, float y0, float y1, float z0, float z1)
{
}

void mat4::setPerspProj(float hfov, float vfov, float nearPlane, float farPlane)
{
	mat4 shear;
	shear.setIdentity();

	mat4 scale;
	float r = tan(Math::toRadians(hfov/2.0f))*nearPlane;
	float l = -r;
	float t = tan(Math::toRadians(vfov/2.0f))*nearPlane;
	float b = -t;

	scale.setIdentity();
	//scale.m[0][0] = (2*nearPlane)/(r - l);
	//scale.m[1][1] = (2*nearPlane)/(t - b);

	mat4 normalization;
	normalization.setIdentity();

	float alpha = -(farPlane + nearPlane)/(farPlane - nearPlane);
	float beta = -(2*farPlane*nearPlane)/(farPlane - nearPlane);

	normalization._m[2][2] = alpha;
	normalization._m[2][3] = beta;
	normalization._m[3][2] = -1.0f;
	normalization._m[3][3] = 0.0f;

	setIdentity();
	*this = *this * normalization;
	*this = *this * scale;
	*this = *this * shear;
}

void mat4::setViewport(int left, int bottom, int right, int top)
{
	setIdentity();

	_m[0][0] = (right - left)/2.0f;
	_m[1][1] = (top - bottom)/2.0f;

	_m[0][3] = (right + left)/2.0f;
	_m[1][3] = (top + bottom)/2.0f;
}

void mat4::setRotate(float angle, vec3 axis)
{
	float s = Math::sine(angle);
	float c = Math::cosine(angle);

	axis.normalize();

	float ux = axis.x;
	float uy = axis.y;
	float uz = axis.z;

	_m[0][0]  = c + (1-c) * ux;
	_m[1][0]  = (1-c) * ux*uy + s*uz;
	_m[2][0]  = (1-c) * ux*uz - s*uy;
	_m[3][0]  = 0;
		
	_m[0][1]  = (1-c) * uy*ux - s*uz;
	_m[1][1]  = c + (1-c) * uy*uy;
	_m[2][1]  = (1-c) * uy*uz + s*ux;
	_m[3][1]  = 0;
		
	_m[0][2] = (1-c) * uz*ux + s*uy;
	_m[1][2] = (1-c) * uz*uz - s*ux;
	_m[2][2] = c + (1-c) * uz*uz;
	_m[3][2] = 0;
		
	_m[0][3] = 0;
	_m[1][3] = 0;
	_m[2][3] = 0;
	_m[3][3] = 1;
}

void mat4::setScale(vec3 scaleFactor)
{
	setIdentity();
	_m[0][0] = scaleFactor.x;
	_m[1][1] = scaleFactor.y;
	_m[2][2] = scaleFactor.z;
}

void mat4::setTranslate(vec3 translationFactor)
{
	setIdentity();
	_m[0][3] = translationFactor.x;
	_m[1][3] = translationFactor.y;
	_m[2][3] = translationFactor.z;
}

void mat4::setIdentity()
{
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			if(i == j) _m[i][j] = 1.0f;
			else _m[i][j] = 0.0f;
		}
	}
}


ostream& RX::operator<<(ostream &out, mat4 m)
{
	out << m._m[0][0] << " " << m._m[0][1] << " " << m._m[0][2] << " " << m._m[0][3] << endl;
	out << m._m[1][0] << " " << m._m[1][1] << " " << m._m[1][2] << " " << m._m[1][3] << endl;
	out << m._m[2][0] << " " << m._m[2][1] << " " << m._m[2][2] << " " << m._m[2][3] << endl;
	out << m._m[3][0] << " " << m._m[3][1] << " " << m._m[3][2] << " " << m._m[3][3] << endl;
	return out;
}

istream& RX::operator>>(istream &in, mat4 &m)
{
	in >> m._m[0][0] >> m._m[0][1] >> m._m[0][2] >> m._m[0][3];
	in >> m._m[1][0] >> m._m[1][1] >> m._m[1][2] >> m._m[1][3];
	in >> m._m[2][0] >> m._m[2][1] >> m._m[2][2] >> m._m[2][3];
	in >> m._m[3][0] >> m._m[3][1] >> m._m[3][2] >> m._m[3][3];

	return in;
}
