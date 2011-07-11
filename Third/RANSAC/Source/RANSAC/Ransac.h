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

#include <RX/mat3.h>
using namespace RX;

int matinv(mat3 a, mat3 &inv);

class Ransac
{
public:
	Ransac(void);
	Ransac(matd&, matd&, int, double);
	void Normalize(matd&, mat3&, matd&, mat3&);
	void MainLoop(matd&, matd&, int, int, double, mat3&, vector<int>&, int&, int&);
	void MatrixMul(mat3&, int, int, matd&, int, int, matd&) const;
	void fitHomography(matd&, matd&, int, mat3&);
	void getInliers(matd&, matd&, int, mat3&, double, vector<int>&, int&, int&);
public:
	~Ransac(void);

private:
	double mean(vector<double>, int) const;
	void Normalize(matd&, int, matd&, mat3&) const;
	void randgen(int, int, vector<int>&);
	bool isdegenerate(matd&, int);
	bool iscolinear(double*, double*, double*);
	void cross(double*, double*, double[]);
	double magnitude(double[], int);

private:
	matd m_X1;	//3xNOE
	matd m_X2;	//3xNOE
	int m_NOF;
	double m_Threshold;
	matd m_H;
};
