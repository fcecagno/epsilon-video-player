#ifndef __HOMOGRAPHY_H
#define __HOMOGRAPHY_H

#include <vector>
#include <RX/mat3.h>
#include <RX/vec2.h>
using namespace std;

class Homography
{
public:
	Homography();
	~Homography();

	void clear();
	void load(std::string filename);

	RX::vec2 transform(int frame, RX::vec2 vec);

private:
	vector<RX::mat3> _hom;


};

#endif // __HOMOGRAPHY_H
