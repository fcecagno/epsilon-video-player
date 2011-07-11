/*
=================================================
Camera.h
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	06/03/2009
=================================================
*/

#include <iostream>
#include "Camera.h"
#include "Definitions.h"
using namespace RX;

// Constructor
Camera::Camera()
: _position(0.0, 0.0, 1.0)
, _target(0.0, 0.0, 0.0)
, _up(0.0, 1.0, 0.0)
, _hfov(90.0f)
, _vfov(90.0f)
, _near(1.0f)
, _far(2000.0f)
{
}

void Camera::reset()
{
	_modelview.setIdentity();
	_projection.setIdentity();
}


void Camera::setPerspCamera(float hfov, float vfov, float nearPlane, float farPlane)
{
	_projection.setPerspProj(hfov, vfov, nearPlane, farPlane);
}

void Camera::setOrthoCamera(float x0, float x1, float y0, float y1, float z0, float z1)
{
	_modelview.setOrthoProj(x0, x1, y0, y1, z0, z1);
}

/*
// Transformations
void Camera::move(float magnitude, Direction dir)
{
	vec3 axis;

	vec3 n = (_position - _target).normalized();
	vec3 u = (_up ^ n).normalized();
	vec3 v = (n ^ u).normalized();

	switch(dir) {
		case X_CAM:
			axis = u;
			break;
		case Y_CAM:
			axis = v;
			break;
		case Z_CAM:
			axis = n;
			break;
	}

	_position += axis * magnitude;
	if(!_fixedTarget) _target += axis * magnitude;
}

void Camera::rotate(float angle, Direction dir)
{
	vec3 axis;

	vec3 n = (_position - _target).normalized();
	vec3 u = (_up ^ n).normalized();
	vec3 v = (n ^ u).normalized();

	switch(dir) {
		case X_CAM:
			axis = u;
			break;
		case Y_CAM:
			axis = v;
			break;
		case Z_CAM:
			axis = n;
			break;
	}

	mat4 rotation;
	rotation.setRotate(angle, axis);
	
	_up = rotation.transform(_up);

	vec3 _look = _position - _target;

	_look = rotation.transform(_look);
	_target = _position - _look;
	
	look();
}
*/
