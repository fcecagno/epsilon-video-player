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
#include "ap.h"
#include "reflections.h"
#include "bidiagonal.h"
#include "qr.h"
#include "lq.h"
#include "blas.h"
#include "rotations.h"
#include "bdsvd.h"
#include "svd.h"

Ransac::Ransac(void)
{
}
/**
 * Constructor.
 * @param x1 x and y coordinates for the frst image).
 * @param x2 x and y coordinates for the scnd image).
 * @param nof number of features.
 * @param t Threshold.
 */
Ransac::Ransac(matd& x1, matd& x2, int nof, double t)
{
	//Random Seed
	srand((unsigned)time(0)); 
	m_NOF = nof;	//Number of Featured as returned by SIFT
	m_Threshold = t;	//RANSAC Threshold

	createMatrix(m_X1, 3, m_NOF);
	createMatrix(m_X2, 3, m_NOF);

	//Copy the Data to create your own copy
	for(int i=0; i <3; i++)
	{
		for(int j = 0; j < m_NOF; j++)
		{
			m_X1[i][j] = x1[i][j];
			m_X2[i][j] = x2[i][j];
		}
	}
}
/**
 * Destructor.
 */
Ransac::~Ransac(void)
{
}

/**
 * Normalize
 * This Function Normalizes the 2 Member Data Sets(m_X1, m_X2)(Features Locations) and returns the two Normalized data sets .
 * and the Trasnformation matrixes between the Original and the Normalized Data sets.
 * @return norm_x1 normalized set for the frst data set.
 * @return T1 Transformation matrix between m_X1 and norm_x1.
 * @return norm_x2 normalized set for the scnd data set.
 * @return T2 Transformation matrix between m_X2 and norm_x2.
 */
void Ransac::Normalize(matd &norm_x1, mat3 &T1, matd &norm_x2, mat3 &T2)
{
	Normalize(m_X1, m_NOF, norm_x1, T1);
	Normalize(m_X2, m_NOF, norm_x2, T2);
}
/**
 * Normalize
 * This Function Normalizes the Data Sets(x)and returns the Normalized data set.
 * and the Trasnformation matrixes between the Original and the Normalized Data set.
 * @param x Original Data Set.
 * @param noe Number Of Elements in the Data Set.
 * @return norm_x normalized Data Set.
 * @return T Transformation matrix between x and norm_x.
 */
void Ransac::Normalize(matd& x, int noe, matd& norm_x, mat3 &T) const	//Number of Elements
{
	// In case if the Scale is not 1
	for(int i=0; i<noe; i++)
	{
		x[0][i]/=x[2][i];
		x[1][i]/=x[2][i];
		x[2][i]=1;
	}
	
	//1- Find the means(Center)
	double mean_x = mean(x[0], noe);
	double mean_y = mean(x[1], noe);
	//2- shift to Center
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
/**
 * mean
 * This Function calculates the mean of a Data Set.
 * and the Trasnformation matrixes between the Original and the Normalized Data set.
 * @param x Original Data Set.
 * @param noe Number Of Elements in the Data Set.
 */
double Ransac::mean(vector<double> arr, int noe)const	//Numner of elements
{
	double mean = 0;
	for(int i = 0; i < noe; i++)
		mean+= arr[i];
	return (double)mean/noe;
	
}
/**
 * MatrixMul 
 * This Function Multiply two 2D Matrixes.
 * @param m1 The first matrix.
 * @param row1 Number of rows of the first Matrix.
 * @param col1 Number of columns of the first Matrix.
 * @param m2 The second matrix.
 * @param row2 Number of rows of the second Matrix.
 * @param col2 Number of columns of the second Matrix.
 * @return m3 The multiplication of the two matrixes.
 */
void Ransac::MatrixMul(mat3 &m1, int row1, int col1, matd& m2, int row2, int col2, matd& m3) const
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
/**
 * MainLoop.
 * @param x1 Features Locations of the frst image.
 * @param x2 Features Locations of the scnd image.
 * @param noe Number of Elements.
 * @param mnop Minimum number of points needed for the homography.
 * @param t Threshold.
 * @return H Homography.
 * @return inliers indexes of inliers.
 * @return noi Number of Inliers.
 * @return noo Number of Outliers.
 */
void Ransac::MainLoop(matd& x1, matd& x2, int noe, int mnop, double t, mat3 &H, vector<int> &inliers, int& noi, int& noo)
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
	mat3 H_crt;			//current Homography
	H_crt.setIdentity();

	vector<int> inliers_crt;	// current inliers
	for(int i = 0; i < noe; ++i)
		inliers_crt.push_back(0);

	noi = 0;	//Initial Number of Inliers
	noo = 0;	//Initial Number of Inliers

  while (tcount <= maxNOT)
  {
    degenerate = true;
    count = 1;
    while (degenerate && count <= maxDataTrials)
	{
		// Generate mnop random indicies in the range 1..npts
		randgen(noe, mnop+1, rand4);

		for(int i=0; i<3; i++)
			for(int j=0; j<mnop; j++)
			{
				x11[i][j] = x1[i][rand4[j]];
				x21[i][j] = x2[i][rand4[j]];
			}

      // Test that these points are not a degenerate configuration.
      degenerate = (isdegenerate(x11, mnop) || isdegenerate(x21, mnop) );
	    
	  if (!degenerate)
	  {
        // Fit homography to this random selection of data points.
		  fitHomography(x11, x21, mnop, H_crt);	//Here H is the resulting 3x3 homography
	  }//(!degenerate)
      count = count + 1;
	}	//(degenerate && count <= maxDataTrials)

	if (degenerate)
	{
      break;
	}
  

	//Find the inliers to the calculated homography
	getInliers(x1, x2, noe, H_crt, t, inliers_crt, noi_tmp, noo_tmp);	//Here H is the resulting 3x3 homography

    if (noi_tmp > noi)   // Largest set of inliers so far...
	{
		noi = noi_tmp;    // Record data for this model
		noo = noo_tmp;    // Record data for this model

		//Update the inliers
		for(int i=0; i<noe; i++)
			inliers[i] = inliers_crt[i];

		//Update the Homography
		H = H_crt;

	  N = log(1-p)/log(1 -  pow((double)noi/(double)noe,(double)mnop));
	}

    tcount = tcount + 1;  
  }	//(N > tcount && tcount <= maxNOT)
}
/**
 * randgen
 * This Function generates nPlus1-1 randon numbers between 0 and noe-1.
 * @param noe Original Data Set.
 * @param nPlus1 Desired number of Random Numbers + 1.
 * @return randn Array of the Desired Random Number.
 */
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
/**
 * isdegenerate
 * This Function checks if 3 of the four choosen 4 points are colinear.
 * @param x Data Set.
 * @param noe Number Of Elements.
 */
bool Ransac::isdegenerate(matd &x, int noe)
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

				if(iscolinear(a, b, c))
				{
					return true;
				}
			}
		}
	}

	return false;
}
/**
 * isdegenerate
 * This Function checks if 3 points are colinear.
 * @param a first Point.
 * @param b second Point.
 * @param c third Point.
 */
bool Ransac::iscolinear(double* a, double*b, double*c)
{
	double v1[] = {b[0]-a[0], b[1]-a[1], b[2]-a[2]};
	double v2[] = {c[0]-a[0], c[1]-a[1], c[2]-a[2]};
	double v[3];
	cross(v1, v2, v);
	return (magnitude(v,3)<eps)?true:false;
}
/**
 * cross
 * This Function calculates the cross product of two 3x3 vectors.
 * @param v1 the first vector.
 * @param v2 the second vector.
 * @return v The resulting vector.
 */
void Ransac::cross(double* v1, double* v2, double v[])
{
	v[0] = v1[1]*v2[2] - v1[2]*v2[1];
	v[1] = v1[2]*v2[0] - v1[0]*v2[2];
	v[2] = v1[0]*v2[1] - v1[1]*v2[0];
}
/**
 * norm
 * This Function calculates the magnitude of a vector.
 * @param v The input vector.
 * @param d The diminsion of the vector.
 */
double Ransac::magnitude(double v[], int d)
{
	double mag=0.0;
	
	for(int i=0; i<d; i++)
		mag+=v[i]*v[i];

	return sqrt(mag);
}
/**
 * fitHomography
 * This Function Tries to fit a Homography to a set of Data.
 * @param x1 The Data Set from the frst image.
 * @param x2 The Data Set from the scnd image.
 * @param noe The Number of corresponding elements.
 * @return H The resulting Homography.
 */
void Ransac::fitHomography(matd &x1, matd &x2, int noe, mat3 &H)
{
	//Normalized points
	matd new_x1; createMatrix(new_x1, 3, noe);
	matd new_x2; createMatrix(new_x2, 3, noe);

	//Transformation Matrixes
	mat3  T1;
	mat3  T2;
	mat3 H_temp;	//Temporary matrix for Homography denormalization
	
	Normalize(x1, noe, new_x1, T1);
	Normalize(x2, noe, new_x2, T2);

	int m = 3*noe;
	int n = 9;

	//Construct the A Matrix
	matd A; createMatrix(A, m, n);
	double x;
	double y;
	double z;

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

	ap::real_2d_array a;
    ap::real_2d_array u;
    ap::real_2d_array vt;
    ap::real_1d_array w;

	a.setbounds(0, m, 0, n);
	for(int i=0; i<m; i++)
		for(int j=0; j<n; j++)
			a(i,j)=A[i][j];

    if( rmatrixsvd(a, m, n, 2, 2, 2, w, u, vt) )
    {
		for(int i=0; i<3; i++)
			for(int j=0; j<3; j++)
				H.set(i,j, vt(n-1, 3*i+j));

		mat3 T2i;
		if(matinv(T2, T2i) == 0)
		{
			//H = inv(T2)*H*T1
			H_temp = H * T1;
			H = T2i * H_temp;
		}
		else
		{
			//Fail
		}
    }
	else
	{
			//Fail
	}
}

/**
 * fitHomography
 * This Function Determines the number of points that fits the Homography H(inliers).
 * @param x1 The Data Set from the frst image.
 * @param x2 The Data Set from the scnd image.
 * @param noe The Number of corresponding elements.
 * @param H The input Homography.
 * @return t The threshold.
 * @return inliers The indexes of the inliers and outliers(1= inlier 0= outlier).
 * @return noi The number of inliers.
 * @return noo The number of outliers.
 */
void Ransac::getInliers(matd &x1, matd &x2, int noe, mat3 &H, double t, vector<int> &inliers, int& noi, int& noo)
{
	matd Hx1; createMatrix(Hx1, 3, noe);
	matd invHx2; createMatrix(invHx2, 3, noe);
	
	mat3 invH;
	if(matinv(H, invH) == 0)
	{
		
	}
	else
	{
		//Fail
	}

    // Calculate, in both directions, the transfered points    
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
