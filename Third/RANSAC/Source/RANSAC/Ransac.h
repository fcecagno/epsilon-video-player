/*************************************************************************
Copyright (c) 2008 The University of Western Ontario.  All rights reserved.

Contributors:
    * Islam Ali (Computer Science Department)

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

- Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer listed
  in this license in the documentation and/or other materials
  provided with the distribution.

- Neither the name of the copyright holders nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
*************************************************************************/

#pragma once
#define eps 2.220446049250313e-16

#include <RX/mat3d.h>
using namespace RX;

int matinv(mat3d a, mat3d &inv);

class Ransac
{
public:
	Ransac();
	~Ransac();

	Ransac(matd&, matd&, int, double);
	void normalize();
	void fitHomography(mat3d &H);
	void mainLoop(int mnop, double t, mat3d &H);
	void denormalize(mat3d &H);

	void MatrixMul(mat3d&, int, int, matd&, int, int, matd&) const;
	void getInliers(matd&, matd&, int, mat3d&, double, vector<int>&, int&, int&);

	// Gets
	int noi() { return _noi; }

private:
	double mean(vector<double>, int) const;
	void Normalize(matd&, int, matd&, mat3d&) const;
	void randgen(int, int, vector<int>&);
	bool isDegenerate(matd&, int);
	bool isCollinear(double*, double*, double*);
	void cross(double*, double*, double[]);
	double length(double[], int);

	void fitHomography(matd&, matd&, int, mat3d&);

private:
	mat3d _T1, _T2;
	matd _x1, _x2, _normx1, _normx2;
	matd _x1In, _x2In;
	int _nof, _noi;

	double _threshold;
	matd _H;
	vector<bool> _inliers;
};
