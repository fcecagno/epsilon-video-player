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
#include <string>
#include <vector>
#include <RX/mat3d.h>
#include <RX/vec3d.h>
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
	
	mat3d T1, T2, H_temp;
	vector<mat3d> H;
	vector<vec3d> centroid;
	mat3d Hlast;
	Hlast.setIdentity();

	string folder = "correspSIFTb";
	ofstream results;
	results.open(folder+"/results.txt");
	for(int frame = 0; frame <= 1036; ++frame)
	{
		while(H.size() <= frame) {
			H.push_back(mat3d());
			centroid.push_back(vec3d());
		}

		string str(folder+"/corresp");
		char buf[10];
		sprintf(buf, "%d", frame);
		str += buf;
		str += ".txt";

		/*
		string str2(folder+"/feat");
		str2 += buf;
		str2 += ".txt";

		string str3(folder+"/inliers");
		str3 += buf;
		str3 += ".txt";
		*/

		ifstream in(str);
		//ofstream feat(str2);
		//ofstream inliersf(str3);

		if(in.is_open())
		{
			//Read the Number and Location of the Features returned by SIFT
			int nof;
			in>>nof;

			 createMatrix(x1, 3, nof);
			 createMatrix(x2, 3, nof);
			 
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

			double t = 0.00001;	//Define your RANSAC threshold

			Ransac R1(x1,x2,nof,t);
			R1.normalize();
			R1.mainLoop(4, t, H[frame]);
			R1.fitHomography(H[frame]);
			R1.denormalize(H[frame]);
			

			// get centroid
			//centroid[frame] = !(H[frame]) * vec3d(256, 144, 1);
			// find closest to origin with distance to centroid still small


			H[frame] =  H[frame] * Hlast;
			Hlast = H[frame];
			H[frame] = !(H[frame]);

			results << R1.noi() << " " << frame << std::endl;
			for(int i=0; i<3; i++) {
				for(int j=0; j<3; j++) {
					double r = H[frame].at(i,j);
					results << r << "\t";
				}
				results << std::endl;
			}

		}
		
		in.close();
		cout << frame << endl;
	}
	results.close();

	return 0;
}