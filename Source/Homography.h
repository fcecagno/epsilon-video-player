#ifndef __HOMOGRAPHY_H
#define __HOMOGRAPHY_H

#include <vector>
#include <RX/mat3d.h>
#include <RX/vec2d.h>
using namespace std;

class Homography
{
public:
	Homography();
	~Homography();

	void clear();
	void load(std::string filename);

	RX::vec2d transform(int frame, RX::vec2d vec);

private:
	vector<RX::mat3d> _hom;


};

#endif // __HOMOGRAPHY_H
