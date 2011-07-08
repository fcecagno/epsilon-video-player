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

	f1->clear();
	for(int i = 0; i < nFeatures; ++i)
		f1->push_back(pair<int, int>(fl->feature[i]->x, fl->feature[i]->y));

	 KLTTrackFeatures(tc, img1, img2, w, h, fl);

	f2->clear();
	for(int i = 0; i < nFeatures; ++i)
		f2->push_back(pair<int, int>(fl->feature[i]->x, fl->feature[i]->y));

}

void KLTCorresp::print(string filename)
{
	ofstream out(filename);
	out << nFeatures << endl;
	for(int i = 0; i < nFeatures; ++i) {
		out << f1[i].first << endl;
		//out << fl->feature[i]->y << endl;
	}
	for(int i = 0; i < nFeatures; ++i) {
		out << f1[i].second << endl;
		//out << fl->feature[i]->y << endl;
	}
}