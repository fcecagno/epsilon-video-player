/*
=================================================
mat3.h
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	30/04/2009
=================================================
*/

#ifndef __MAT3D_H
#define __MAT3D_H

#include <sstream>
#include "vec3d.h"
#include "vec4d.h"
#include "../Third/Inc/C3/C3_Math.h"

namespace RX
{
class mat3d
{
	friend std::ostream &operator<<(std::ostream &out, mat3d m);
	friend std::istream &operator>>(std::istream &in, mat3d &m);

public:
	// Create/destroy
	mat3d();
	mat3d(double mat[9]);
	mat3d(double m00, double m01, double m02,
		 double m10, double m11, double m12,
		 double m20, double m21, double m22);
	mat3d(vec3d row1, vec3d row2, vec3d row3);
	~mat3d();

	// Multiply matrices
	mat3d operator!() const;
	mat3d operator*(const mat3d &other) const;
	
	mat3d operator/=(const double d);

	// Transform a vec3
	vec3d operator*(const vec3d &v) const;
	vec3d transform(const vec3d &v) const;

	// Create matrices
	void setView(vec3d position, vec3d target, vec3d up);
	void setOrthoProj(double x0, double x1, double y0, double y1, double z0, double z1);
	void setPerspProj(double hfov, double vfov, double nearPlane, float farPlane);
	void setViewport(int left, int bottom, int right, int top);
	void setRotate(float angle, vec3d axis);
	void setScale(vec3d scaleFactor);
	void setTranslate(vec3d translationFactor);
	void setIdentity();

	const double *doubles() const { return &_m[0][0]; }
	const double at(int i, int j) { return _m[i][j]; }
	void set(int i, int j, double value) { _m[i][j] = value; }

public:
	double _m[3][3];

private:
	void fromC3Math(C3::Math::mat3 matrix)
	{
		for(int i = 0; i < 3; ++i)
			for(int j = 0; j < 3; ++j)
				_m[i][j] = matrix[i][j];
		
	}
	C3::Math::mat3 toC3Math() const
	{
		C3::Math::mat3 matrix;
		for(int i = 0; i < 3; ++i)
			for(int j = 0; j < 3; ++j)
				matrix[i][j] = _m[i][j];
		return matrix;
	}
};

std::ostream &operator<<(std::ostream &out, mat3d m);
std::istream &operator>>(std::istream &in, mat3d &m);

};

#endif // __MAT3D_H