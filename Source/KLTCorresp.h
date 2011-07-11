#ifndef __KLTCORRESP_H
#define __KLTCORRESP_H

#include <string>
#include <vector>
#include "defines.h"
#include "VideoRegistration/klt.h"
using namespace std;

class KLTCorresp
{
public:
	KLTCorresp();
	~KLTCorresp();

	void findCorresp(uchar *img1, uchar *img2, int w, int h);
	void print(string filename);

	vector<pair<int,int>> getF1() { return f1; }
	vector<pair<int,int>> getF2() { return f2; }

private:
	KLT_TrackingContext tc;
	KLT_FeatureList fl;
	int nFeatures;
	int w, h;

	vector<pair<int,int>> f1;
	vector<pair<int,int>> f2;

};

#endif // __KLTCORRESP_H