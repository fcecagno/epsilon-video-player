#include <fstream>
#include <iostream>
#include <string>
#include "Matrix/matrix.h"
#include "homoest.h"

//  mhomoest: Homography estimation from correcpondence between two    //
//            sets of image points (main program).                     //
//  Programmed by Naoya Ohta and Shimizu Yoshiyuki (1999/2/25)         //
//  Computer Science Department, Gunma University                      //

int main (int argc, char *argv[])
{
	vector  *i1, *i2, *x1, *x2, ic1(2), ic2(2);
	matrix  *Vx1, *Vx2, Hhat(3,3), Hp(3,3), Hm(3,3);
	double  f0, eps2;
	int     pn, it, a;

	// check number of arguments.
	if(argc != 1) {
		std::cerr << "Usage: " << argv[0] << " < in-file > out-file" << std::endl;
		exit(1); 
	}

	double maxDiffNorm = 0;
	std::string folder = "correspSIFTa";
	std::ofstream results;
	results.open(folder+"/results.txt");
	for(int frame = 1; frame <= 1036; ++frame)
	{
		std::string str(folder+"/corresp");
		char buf[10];
		sprintf(buf, "%d", frame);
		str += buf;
		str += ".txt";
		std::ifstream in(str);

		// read number of points.
		in >> pn;

		i1  = new vector[pn]; i2  = new vector[pn];
		x1  = new vector[pn]; x2  = new vector[pn];
		Vx1 = new matrix[pn]; Vx2 = new matrix[pn];

		for (a=0; a<pn; a++) {
		// initialize size of vectors and matrices in arrays.
		i1[a].init(2); i2[a].init(2);
		x1[a].init(3); x2[a].init(3);
		Vx1[a].init(3,3); Vx2[a].init(3,3);

		// set default covariance matrices.
		Vx1[a].clear(); Vx2[a].clear();
		Vx1[a][0][0] = Vx1[a][1][1] = 1.0;
		Vx2[a][0][0] = Vx2[a][1][1] = 1.0;
		}
		for (a=0; a<pn; a++) {
		// read points in image 1.
		in >> i1[a][1];
		}
		for (a=0; a<pn; a++) {
		in >> i1[a][0];
		}
		for (a=0; a<pn; a++) {
		// read points in image 2.
		in >> i2[a][1];
		}
		for (a=0; a<pn; a++) {
		in >> i2[a][0];
		}

		// scale the data.
		scale(i1,i2,x1,x2,ic1,ic2,f0,pn);

		// estimate homography.
		it = homoest(x1,Vx1,x2,Vx2,pn,Hhat,Hp,Hm,eps2);

		// convert the scaled coordinate to original one.
		Hhat = crdcnv(Hhat,ic1,ic2,f0);
		Hp   = crdcnv(Hp,ic1,ic2,f0);
		Hm   = crdcnv(Hm,ic1,ic2,f0);

		double diffNorm = 0;
		for(int i = 0; i < 3; ++i) {
			for(int j = 0; j < 3; ++j) {
				double val1 = *(Hp[i*3+j]);
				double val2 = *(Hm[i*3+j]);
				double val = val1-val2;
				diffNorm += val*val;
			}
		}

		// print results
		results << diffNorm << " " << frame << std::endl;
		for(int i = 0; i < 3; ++i) {
			for(int j = 0; j < 3; ++j) {
				results << Hhat[i][j]/Hhat[2][2] << " ";
			}
			results << std::endl;
		}

		if(diffNorm > maxDiffNorm)
			maxDiffNorm = diffNorm;

		std::cout << frame << " " << maxDiffNorm << std::endl;
	}
	
	system("pause");
	delete [] i1, i2, x1, x2, Vx1, Vx2;
	exit(0);
}
