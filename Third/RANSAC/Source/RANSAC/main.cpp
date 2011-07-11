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
#include <iostream>
#include <fstream>
#include <vector>
#include <RX/mat3.h>
#include "define.h"
#include "Ransac.h"

using namespace std;
using namespace RX;

typedef vector< vector<double> > matd;

int main()
{
	matd x1, x2;
	matd x1_inliers, x2_inliers;
	matd x1_outliers, x2_outliers;
	matd new_x1, new_x2;
	
	mat3 T1, T2, H, H_temp;

	for(int frame = 497; frame <= 1036; ++frame)
	{
		string str("corresp1/corresp");
		char buf[10];
		sprintf(buf, "%d", frame);
		str += buf;
		str += ".txt";

		ifstream in(str);

		if(in.is_open())
		{
			//Read the Number and Location of the Features returned by SIFT
			int nof;
			in>>nof;

			 createMatrix(x1, 3, nof);
			 createMatrix(x2, 3, nof);
			 createMatrix(x1_inliers, 3, nof);
			 createMatrix(x2_inliers, 3, nof);
			 createMatrix(x1_outliers, 3, nof);
			 createMatrix(x2_outliers, 3, nof);
			 createMatrix(new_x1, 3, nof);
			 createMatrix(new_x2, 3, nof);

			for(int i=0; i<nof; i++)
				in>> x1[1][i];
			for(int i=0; i<nof; i++)
				in>> x1[0][i];
			for(int i=0; i<nof; i++)
				x1[2][i] = 1;

			for(int i=0; i<nof; i++)
				in>> x2[1][i];
			for(int i=0; i<nof; i++)
				in>> x2[0][i];
			for(int i=0; i<nof; i++)
				x2[2][i] = 1;

			double t = 0.0001;	//Define your RANSAC threshold

			Ransac R1(x1,x2,nof,t);

			//Normalize the Data Sets
			R1.Normalize(new_x1, T1, new_x2, T2);

			vector<int> inliers;
			for(int i = 0; i < nof; ++i)
				inliers.push_back(0);

			int noi;	// TO return number of inliers
			int noo;	// TO return number of outliers
		
			//The Main Loop of the RANSAC
			R1.MainLoop(new_x1, new_x2, nof, 4, t, H, inliers, noi, noo);

			//Separate the inliers and outliers
			int j=0;
			int k=0;
			for(int i=0; i<nof; i++)
			{
				if(inliers[i] == 1)
				{
					x1_inliers[0][j] = new_x1[0][i];
					x1_inliers[1][j] = new_x1[1][i];
					x1_inliers[2][j] = new_x1[2][i];

					x2_inliers[0][j] = new_x2[0][i];
					x2_inliers[1][j] = new_x2[1][i];
					x2_inliers[2][j] = new_x2[2][i];
					j++;
				}
				else
				{
					x1_outliers[0][k] = x1[0][i];
					x1_outliers[1][k] = x1[1][i];
					x1_outliers[2][k] = x1[2][i];

					x2_outliers[0][k] = x2[0][i];
					x2_outliers[1][k] = x2[1][i];
					x2_outliers[2][k] = x2[2][i];
					k++;
				}
			}

			// Update your homography by fitting a homography to the whole Number of Inliers
			R1.fitHomography(x1_inliers, x2_inliers, noi, H);

			//Denormalize H
			mat3 T2i;
			if(matinv(T2, T2i) == 0)
			{
				//H = inv(T2)*H*T1
				H_temp = H * T1;
				H = T2i * H_temp;
			}
			else
			{
				cout << "fail" << endl;
			}

			ofstream results;
			results.open("corresp1/results.txt",ios::out|ios::app);
			results.setf(ios::fixed, ios::floatfield);
			results.setf(ios::showpoint);

			results<< noi << endl;
			for(int i=0; i<3; i++) {
				for(int j=0; j<3; j++) {
					double a = H.at(j, i);
					double b = H.at(0, 0);
					double ab = a/b;
					results<< ab << "\t";
				}
				results << endl;
			}

			//For More than One Homography, 
			//u can call the algorithm again 
			//using the outliers as Input
			results.close();
		}
		in.close();
		cout << frame << endl;
	}

	return 0;
}