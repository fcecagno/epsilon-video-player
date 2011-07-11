/*
=================================================
Mathematics.cpp
-------------------------------------------------
Author:			Rosália Galiazzi Schneider
Last Modified:	06/03/2009
=================================================
*/

#include "Mathematics.h"
using namespace RX;

const int Math::infinity	= 0xFFFFFFFF;
const float Math::pi		= 3.1415926535897932384626433832795f;
const float Math::epsilon	= 0.001;

// Degrees/radians convertion
const float Math::toDegrees(const float radians)
{
	return radians * 180.0f / pi;
}
const float Math::toRadians(const float degrees)
{
	return degrees * pi / 180.0f;
}

// Sine/cosine
const float Math::cosine(float degrees)
{
	degrees -= ((int)degrees/360)*360; // -360.0 - 360.0 (float mod)
	if(degrees < 0.0f) degrees += 360.0f; // 0 - 360.0

	// cos function works with radians, and only for the first quadrant
	int sig = 1;

	// quad2
	if((degrees > 90.0f)&&(degrees <= 180.0f)) {
		sig = -1;
		degrees = (180.0f - degrees);
	}
	// quad3
	if((degrees > 180.0f)&&(degrees <= 270.0f)) {
		sig = -1;
		degrees = (degrees - 180.0f);
	}
	// quad4
	if((degrees > 270.0f)&&(degrees <= 360.0f)) degrees = (360.0f - degrees);

	return (float)(sig*cos(toRadians(degrees)));
}

const float Math::sine(float degrees)
{
	degrees -= ((int)degrees/360)*360; // -360.0 - 360.0 (float mod)
	if(degrees < 0.0f) degrees += 360.0f; // 0 - 360.0

	// sin function works with radians, and only for the first quadrant
	int sig = 1;

	// quad2
	if((degrees > 90.0f)&&(degrees <= 180.0f)) degrees = (180.0f - degrees);
	// quad3
	if((degrees > 180.0f)&&(degrees <= 270.0f)) {
		sig = -1;
		degrees = (degrees - 180.0f);
	}
	// quad4
	if((degrees > 270.0f)&&(degrees <= 360.0f)) {
		sig = -1;
		degrees = (360.0f - degrees);
	}

	return (float)(sig*sin(toRadians(degrees)));
}

// Arcsine/arccosine
const float Math::arccosine(const float sin, const float cos)
{
	float angle = acos(cos)*180/pi;
	if(sin < 0) angle = 360 - angle;
	return angle;
}

const float Math::arcsine(const float sin, const float cos)
{
	// NOT IMPLEMENTED
	return 0.0;
}

