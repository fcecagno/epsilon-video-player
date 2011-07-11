#include <fstream>
#include "KLTCorresp.h"

KLTCorresp::KLTCorresp()
{
	nFeatures = 100;
	tc = KLTCreateTrackingContext();
	fl = KLTCreateFeatureList(nFeatures);
}

KLTCorresp::~KLTCorresp()
{
}

void KLTCorresp::findCorresp(uchar *img1, uchar *img2, int w, int h)
{
	KLTSelectGoodFeatures(tc, img1, w, h, fl);

	f1.clear();
	for(int i = 0; i < nFeatures; ++i)
		f1.push_back(pair<int, int>(fl->feature[i]->x, fl->feature[i]->y));

	 KLTTrackFeatures(tc, img1, img2, w, h, fl);

	f2.clear();
	for(int i = 0; i < nFeatures; ++i)
		f2.push_back(pair<int, int>(fl->feature[i]->x, fl->feature[i]->y));

	for(int i = 0; i < f1.size(); ++i) {
		if(f2[i] == pair<int,int>(-1,-1)) {
			f2.erase(f2.begin()+i);
			f1.erase(f1.begin()+i);
			--i;
		}
	}

}

void KLTCorresp::print(string filename)
{
	ofstream out(filename);
	out << f1.size() << endl;
	for(int i = 0; i < f1.size(); ++i)
		out << f1[i].first << endl;
	for(int i = 0; i < f1.size(); ++i)
		out << f1[i].second << endl;
	for(int i = 0; i < f1.size(); ++i)
		out << f2[i].first << endl;
	for(int i = 0; i < f1.size(); ++i)
		out << f2[i].second << endl;
}