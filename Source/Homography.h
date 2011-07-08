#ifndef __HOMOGRAPHY_H
#define __HOMOGRAPHY_H

#include <vector>
#include <RX/mat3.h>
#include "../defines.h"
using namespace std;

class Homography
{
public:
	Homography();
	~Homography();

	void findHomography(vector< pair<int, int> > f1, vector< pair<int, int> > f2);
	void findHomographyNormalized(vector< pair<int, int> > f1, vector< pair<int, int> > f2);

	void print(string filename);

private:
	RX::mat3 h;
};


#endif // __HOMOGRAPHY_H