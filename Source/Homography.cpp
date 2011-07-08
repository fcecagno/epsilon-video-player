#include "VideoRegistration/Ransac.h"
#include "Homography.h" 

Homography::Homography()
{
}

Homography::~Homography()
{
}

void Homography::findHomography(vector< pair<int, int> > f1, vector< pair<int, int> > f2)
{
	int nof = f1.size();

	matd x1, x2;
	matd x1_inliers, x2_inliers;
	matd x1_outliers, x2_outliers;
	matd new_x1, new_x2;

	createMatrix(x1, 3, nof);
	createMatrix(x2, 3, nof);
	createMatrix(x1_inliers, 3, nof);
	createMatrix(x2_inliers, 3, nof);
	createMatrix(x1_outliers, 3, nof);
	createMatrix(x2_outliers, 3, nof);
	createMatrix(new_x1, 3, nof);
	createMatrix(new_x2, 3, nof);
	
	mat3 T1, T2, H, H_temp;

	for(int i=0; i<f1.size(); i++)
		x1[1][i] = f1[i].first;
	for(int i=0; i<f1.size(); i++)
		x1[1][i] = f1[i].second;
	for(int i=0; i<f1.size(); i++)
		x1[2][i] = 1;

	for(int i=0; i<f1.size(); i++)
		x2[1][i] = f2[i].first;
	for(int i=0; i<f1.size(); i++)
		x2[1][i] = f2[i].second;
	for(int i=0; i<f1.size(); i++)
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

	h = mat3(H.at(0,0), H.at(1,0), H.at(2,0),
		     H.at(0,1), H.at(1,1), H.at(2,1),
			 H.at(0,2), H.at(1,2), H.at(2,2));
}

void Homography::findHomographyNormalized(vector< pair<int, int> > f1, vector< pair<int, int> > f2)
{
	findHomography(f1, f2);
	h /= h.at(0,0);
}

void Homography::print(string filename)
{
	ofstream out(filename);
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			out << h.at(i, j) << " ";
		}
		out << endl;
	}
}