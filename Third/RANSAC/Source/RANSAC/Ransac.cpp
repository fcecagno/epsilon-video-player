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

#include <cmath>
#include <ctime> 
#include <cstdlib> 
#include "define.h"
#include "Ransac.h"

//SVD header files
#include "svd.h"

Ransac::Ransac(void)
{
}

// Constructor
Ransac::Ransac(matd& x1, matd& x2, int nof, double t)
{
	// random seed
	srand((unsigned)time(0)); 
	_nof = nof;
	_threshold = t;

	createMatrix(_x1, 3, _nof);
	createMatrix(_x2, 3, _nof);
	createMatrix(_normx1, 3, _nof);
	createMatrix(_normx2, 3, _nof);
	createMatrix(_x1In, 3, _nof);
	createMatrix(_x2In, 3, _nof);

	for(int i = 0; i < _nof; ++i)
		_inliers.push_back(false);

	// copy the data to create your own copy
	for(int i=0; i <3; i++)
	{
		for(int j = 0; j < _nof; j++)
		{
			_x1[i][j] = x1[i][j];
			_x2[i][j] = x2[i][j];
		}
	}
}

// Destructor
Ransac::~Ransac(void)
{
}

// Normalize
 void Ransac::normalize()
{
	Normalize(_x1, _nof, _normx1, _T1);
	Normalize(_x2, _nof, _normx2, _T2);
}

// Normalize
void Ransac::Normalize(matd& x, int noe, matd& norm_x, mat3d &T) const
{
	// In case if the Scale is not 1
	for(int i=0; i<noe; i++)
	{
		x[0][i]/=x[2][i];
		x[1][i]/=x[2][i];
		x[2][i]=1;
	}
	
	// find the means (center)
	double mean_x = mean(x[0], noe);
	double mean_y = mean(x[1], noe);
	// shift to center
	vector<double> new_x;
	vector<double> new_y;
	vector<double> dist;
	for(int i = 0; i < noe; i++)
	{
		new_x.push_back(x[0][i]-mean_x);
		new_y.push_back(x[1][i]-mean_y);
		dist.push_back(sqrt(new_x[i]*new_x[i] + new_y[i]*new_y[i]));
	}
	double mean_dist = mean(dist, noe);
	double scale = sqrt(2.0f)/mean_dist;

	T.set(0,0, scale);	T.set(0,1, 0.0);	T.set(0,2, -scale*mean_x);
	T.set(1,0, 0.0);	T.set(1,1, scale);	T.set(1,2, -scale*mean_y);
	T.set(2,0, 0.0);	T.set(2,1, 0.0);	T.set(2,2, 1);

	MatrixMul(T, 3, 3, x, 3, noe, norm_x);
}

// Mean
double Ransac::mean(vector<double> arr, int noe) const
{
	double mean = 0;
	for(int i = 0; i < noe; i++)
		mean+= arr[i];
	return (double)mean/noe;
	
}

// MatrixMul 
 void Ransac::MatrixMul(mat3d &m1, int row1, int col1, matd& m2, int row2, int col2, matd& m3) const
{
	//if col1 != row2 then ERROR
	for(int i=0; i<row1; i++)
	{
		for(int j=0; j<col2; j++)
		{
			m3[i][j] = 0.0;
			for(int k=0; k< col1; k++)
			{
				m3[i][j] += m1.at(i,k) * m2[k][j]; 
			}
		}
	}
}

// MainLoop
void Ransac::mainLoop(int mnop, double t, mat3d &H)
{
 	double p = 0.99f;    // Probability of choosing at least one data set without outliers
	double maxNOT = 1000.0;	//Maximum Number of Trials.
	double maxDataTrials = 100;	//Max number of attempts to select a non Colinear Points

	double tcount = 0;	//Trial Counter
	int noi_tmp =  0;	//Temporary Number of Inliers
	int noo_tmp =  0;	//Temporary Number of Outliers
	double N = 1;		// Initial Number of Trials


	bool degenerate;
	int count;

	vector<int> rand4; //indexes of the 4 ramndom numbers
	for(int i = 0; i < mnop; ++i)
		rand4.push_back(0);
	
	matd x11; createMatrix(x11, 3, mnop);	//the 4 random samples from the frst image
	matd x21; createMatrix(x21, 3, mnop); //the 4 random samples from the scnd image
	mat3d H_crt;			//current Homography
	H_crt.setIdentity();

	vector<int> inliers_crt;	// current inliers
	for(int i = 0; i < _nof; ++i)
		inliers_crt.push_back(0);

	_noi = 0;	//Initial Number of Inliers
	//_noo = 0;	//Initial Number of Inliers

  while (N > tcount && tcount <= maxNOT)
  {
    degenerate = true;
    count = 1;
    while (degenerate && count <= maxDataTrials)
	{
		// Generate mnop random indicies in the range 1..npts
		randgen(_nof, mnop+1, rand4);

		for(int i=0; i<3; i++) {
			for(int j=0; j<mnop; j++) {
				x11[i][j] = _normx1[i][rand4[j]];
				x21[i][j] = _normx2[i][rand4[j]];
			}
		}

      degenerate = (isDegenerate(x11, mnop) || isDegenerate(x21, mnop) );
	  if(!degenerate)
		  fitHomography(x11, x21, mnop, H_crt);	//Here H is the resulting 3x3 homography

      ++count;
	}

	if(degenerate)
		break;
  

	//Find the inliers to the calculated homography
	getInliers(_normx1, _normx2, _nof, H_crt, t, inliers_crt, noi_tmp, noo_tmp);	//Here H is the resulting 3x3 homography

    if (noi_tmp > _noi)   // Largest set of inliers so far...
	{
		_noi = noi_tmp;    // Record data for this model
		//_noo = noo_tmp;    // Record data for this model

		//Update the inliers
		for(int i=0; i< _nof; i++)
			_inliers[i] = inliers_crt[i];

		//Update the Homography
		H = H_crt;

	  N = log(1-p)/log(1 -  pow((double)_noi/(double)_nof,(double)mnop));
	}

    ++tcount;  
  }

  	int j=0;
	//inliersf << noi << endl;
	for(int i=0; i<_nof; i++)
	{
		if(_inliers[i] == 1)
		{
			//inliersf << new_x1[0][i] << ' ' << new_x1[1][i] << ' ' << new_x2[0][i] << ' ' << new_x2[1][i] << endl;
			_x1In[0][j] = _normx1[0][i];
			_x1In[1][j] = _normx1[1][i];
			_x1In[2][j] = _normx1[2][i];

			_x2In[0][j] = _normx2[0][i];
			_x2In[1][j] = _normx2[1][i];
			_x2In[2][j] = _normx2[2][i];

			j++;
		}
	}

}

void Ransac::denormalize(mat3d &H)
{
	mat3d H_temp;
				mat3d T2i;
			if(matinv(_T2, T2i) == 0)
			{
				//H = inv(T2)*H*T1
				H_temp = H * _T1;
				H = T2i * H_temp;
			}
			else
			{
				cout << "fail" << endl;
			}

			for(int i = 0; i < 3; ++i)
				for(int j = 0; j < 3; ++j)
					H.set(i,j,H.at(i, j)/H.at(2, 2));
}

// randgen
void Ransac::randgen(int noe, int nPlus1, vector<int> &randn)
{
    int lowest=0, highest=noe-1; 
    int range=(highest-lowest)+1;
	int randnum;
    for(int index=0; index<nPlus1; index++)
	{ 
		randnum = lowest+int(range*rand()/(RAND_MAX + 1.0));	//Ignore the first random value
		if (index !=0)
			randn[index-1] = randnum; 
    } 
}

// isDegenerate
bool Ransac::isDegenerate(matd &x, int noe)
{
	double a[3];
	double b[3];
	double c[3];

	for(int i=0; i<noe-2; i++)
	{
		a[0] = x[0][i];
		a[1] = x[1][i];
		a[2] = x[2][i];

		for(int j=i+1; j<noe-1; j++)
		{
			b[0] = x[0][j];
			b[1] = x[1][j];
			b[2] = x[2][j];

			for(int k=j+1; k<noe; k++)
			{
				c[0] = x[0][k];
				c[1] = x[1][k];
				c[2] = x[2][k];

				if(isCollinear(a, b, c))
					return true;
			}
		}
	}

	return false;
}

// isCollinear
bool Ransac::isCollinear(double* a, double*b, double*c)
{
	double v1[] = {b[0]-a[0], b[1]-a[1], b[2]-a[2]};
	double v2[] = {c[0]-a[0], c[1]-a[1], c[2]-a[2]};
	double v[3];
	cross(v1, v2, v);
	return (length(v,3)<eps)?true:false;
}

// Cross
void Ransac::cross(double* v1, double* v2, double v[])
{
	v[0] = v1[1]*v2[2] - v1[2]*v2[1];
	v[1] = v1[2]*v2[0] - v1[0]*v2[2];
	v[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

// Norm of a vector
double Ransac::length(double v[], int d)
{
	double n=0.0;
	for(int i=0; i<d; ++i)
		n+=v[i]*v[i];

	return sqrt(n);
}

// Fit Homography
void Ransac::fitHomography(mat3d &H)
{
	fitHomography(_x1In, _x2In, _noi, H);
}

// Fit Homography
void Ransac::fitHomography(matd &x1, matd &x2, int noe, mat3d &H)
{
	matd new_x1; createMatrix(new_x1, 3, noe);
	matd new_x2; createMatrix(new_x2, 3, noe);

	mat3d T1, T2;
	mat3d H_temp;
	
	Normalize(x1, noe, new_x1, T1);
	Normalize(x2, noe, new_x2, T2);

	int m = 3*noe;
	int n = 9;

	matd A; createMatrix(A, m, n);
	double x, y, z;
	
	for(int i=0; i<noe; i++)
	{
		x=new_x2[0][i];
		y=new_x2[1][i];
		z=new_x2[2][i];
		A[3*i][0] =						A[3*i][1] =						A[3*i][2] = 0;
		A[3*i][3] = -z*new_x1[0][i];	A[3*i][4] = -z*new_x1[1][i];	A[3*i][5] = -z*new_x1[2][i];
		A[3*i][6] = y*new_x1[0][i];		A[3*i][7] = y*new_x1[1][i];		A[3*i][8] = y*new_x1[2][i];

		A[3*i+1][0] = z*new_x1[0][i];	A[3*i+1][1] = z*new_x1[1][i];	A[3*i+1][2] = z*new_x1[2][i];
		A[3*i+1][3] =					A[3*i+1][4] =					A[3*i+1][5] = 0;
		A[3*i+1][6] = -x*new_x1[0][i];	A[3*i+1][7] = -x*new_x1[1][i];	A[3*i+1][8] = -x*new_x1[2][i];

		A[3*i+2][0] = -y*new_x1[0][i];	A[3*i+2][1] = -y*new_x1[1][i];	A[3*i+2][2] = -y*new_x1[2][i];
		A[3*i+2][3] = x*new_x1[0][i];	A[3*i+2][4] = x*new_x1[1][i];	A[3*i+2][5] = x*new_x1[2][i];
		A[3*i+2][6] =					A[3*i+2][7] =					A[3*i+2][8] = 0;
	}

	ap::real_2d_array a, u, vt;
    ap::real_1d_array w;

	a.setbounds(0, m, 0, n);
	for(int i=0; i<m; i++)
		for(int j=0; j<n; j++)
			a(i,j)=A[i][j];

    if(rmatrixsvd(a, m, n, 2, 2, 2, w, u, vt))
    {
		for(int i=0; i<3; i++)
			for(int j=0; j<3; j++)
				H.set(i,j, vt(n-1, 3*i+j));

		mat3d T2i;
		if(matinv(T2, T2i) == 0)
		{
			//H = inv(T2)*H*T1
			H_temp = H * T1;
			H = T2i * H_temp;
		}
    }
}

// Get inliers
void Ransac::getInliers(matd &x1, matd &x2, int noe, mat3d &H, double t, vector<int> &inliers, int& noi, int& noo)
{
	matd Hx1; createMatrix(Hx1, 3, noe);
	matd invHx2; createMatrix(invHx2, 3, noe);
	mat3d invH = !H;
	
    // Calculate, in both directions, the transferred points    
	MatrixMul(H, 3, 3, x1, 3, noe, Hx1);
	MatrixMul(invH, 3, 3, x2, 3, noe, invHx2);
	
	for(int i=0; i<noe; i++)
	{
		x1[0][i] /= x1[2][i];
		x1[1][i] /= x1[2][i];
		x1[2][i]		= 1.0;

		x2[0][i] /= x2[2][i];
		x2[1][i] /= x2[2][i];
		x2[2][i]		= 1.0;

		Hx1[0][i] /= Hx1[2][i];
		Hx1[1][i] /= Hx1[2][i];
		Hx1[2][i]		= 1.0;

		invHx2[0][i] /= invHx2[2][i];
		invHx2[1][i] /= invHx2[2][i];
		invHx2[2][i]	= 1.0;
	}

	double d2[1000];
	noi = 0;
	noo = 0;
	for(int i=0; i<noe; i++)
	{
		d2[i] = 0;
		d2[i] += (x1[0][i]-invHx2[0][i]) * (x1[0][i]-invHx2[0][i]);
		d2[i] += (x1[1][i]-invHx2[1][i]) * (x1[1][i]-invHx2[1][i]);

		d2[i] += (x2[0][i]-Hx1[0][i]) * (x2[0][i]-Hx1[0][i]);
		d2[i] += (x2[1][i]-Hx1[1][i]) * (x2[1][i]-Hx1[1][i]);
		if(d2[i] < t)	{	noi++;	inliers[i]	= 1;}
		else			{	noo++;	inliers[i]	= 0;}
	}
}
