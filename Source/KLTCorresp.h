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

	void findCorresp(uchar *img1, uchar *img2, int w, int h, vector<pair<int,int>> *f1, vector<pair<int,int>> *f2);
	void print(string filename);

private:
	KLT_TrackingContext tc;
	KLT_FeatureList fl;
	int nFeatures;
	int w, h;

	vector<pair<int,int>> f1;
	vector<pair<int,int>> f2;

};

#endif // __KLTCORRESP_H