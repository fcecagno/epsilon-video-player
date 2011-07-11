#include "color.h"
using namespace RX;
using namespace std;

color::color()
{
	color(0, 0, 0);
}
color::color(const vec3 &v)
{
	color(v.x, v.y, v.z);
}
color::color(const color &c)
{
	color(c.r, c.g, c.b);
}
color::color(float r1, float g1, float b1)
{
	r = r1;
	g = g1;
	b = b1;
}

color color::operator=(const color &v)
{
	r = v.r;
	g = v.g;
	b = v.b;
	return *this;
}

color color::operator+(const color &v) const
{
	return color(r+v.r, g+v.g, b+v.b);
}

color color::operator+=(const color &v)
{
	r += v.r;
	g += v.g;
	b += v.b;
	return *this;
}

color color::operator-(const color &v) const
{
	return color(r-v.r, g-v.g, b-v.b);
}

color color::operator-=(const color &v)
{
	r -= v.r;
	g -= v.g;
	b -= v.b;
	return *this;
}
/*
0.4124564  0.3575761  0.1804375
 0.2126729  0.7151522  0.0721750
 0.0193339  0.1191920  0.9503041
 */
const mat4 color::gRGBtoXYZ(0.4124564,  0.3575761,  0.1804375, 0,
                            0.2126729,  0.7151522,  0.0721750, 0,
                            0.0193339,  0.1191920,  0.9503041, 0,
							0, 0, 0, 1);

const mat4 color::gXYZtoRGB = !color::gRGBtoXYZ;

const mat4 color::gRGBtoXYZ_Judd_Vos_1978(0.4096, 0.3550, 0.1792, 0,
                                          0.2134, 0.7067, 0.0799, 0,
                                          0.0186, 0.1146, 0.9124, 0,
										  0, 0, 0, 1);

const mat4 color::gXYZtoRGB_Judd_Vos_1978 = !color::gRGBtoXYZ_Judd_Vos_1978;

const mat4 color::gRGBtoYUV(0.299, 0.587, 0.114, 0,
                            -0.14713, -0.28886, 0.436, 0,
                            0.615,  -0.51499, -0.10001, 0,
							0, 0, 0, 1);

const mat4 color::gYUVtoRGB = !color::gRGBtoYUV;

const mat4 color::gRGBtoYIQ(0.299, 0.587, 0.114, 0,
							0.595716, -0.274453, -0.321263, 0,
							0.211456, -0.522591, 0.311135, 0,
							0, 0, 0, 1);

const mat4 gYIQtoRGB = !color::gRGBtoYIQ;

const mat4 color::gRGBtoYCbCr(0.299, 0.587, 0.114, 0,
                              -0.168736, -0.331264, 0.5, 0,
                              0.5, -0.418688, -0.081312, 0,
							  0, 0, 0, 1);

const mat4 color::gYCbCrToRGB = !color::gRGBtoYCbCr;

const mat4 color::gRGBtoYPQ_Grundland_2007(0.299, 0.587, 0.114, 0,
                                           0.5, 0.5, -1.0, 0,
                                           1.0, -1.0, 0.0, 0,
										   0, 0, 0, 1);

const mat4 color::gYPQtoRGB_Grundland_2007 = !color::gRGBtoYPQ_Grundland_2007;

const mat4 color::gRGBtoLMS_Brettel_1997(0.1992, 0.4112, 0.0742, 0,
										 0.0353, 0.2226, 0.0574, 0,
                                         0.0185, 0.1231, 1.3550, 0,
										 0, 0, 0, 1);

const mat4 color::gLMStoRGB_Brettel_1997 = !color::gRGBtoLMS_Brettel_1997;

const mat4 color::gRGBtoLMS_Reinhard_2001(0.3811, 0.5783, 0.0402, 0,
										  0.1967, 0.7244, 0.0782, 0,
										  0.0241, 0.1288, 0.8444, 0,
										  0, 0, 0, 1);

const mat4 color::gLMStoRGB_Reinhard_2001 = !color::gRGBtoLMS_Reinhard_2001;

const mat4 color::gLMStoLAlphaBeta_Ruderman_1998(0.57735027, 0.57735027, 0.57735027, 0,
												 0.40824829, 0.40824829, -0.81649658, 0,
												 0.70710678, -0.70710678, 0.00000000, 0,
												 0, 0, 0, 1);

const mat4 color::gLAlphaBetaToLMS_Ruderman_1998 = !color::gLMStoLAlphaBeta_Ruderman_1998;
