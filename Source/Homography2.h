#ifndef __HOMOGRAPHY2_H
#define __HOMOGRAPHY2_H

#include <vector>
#include <RX/mat3.h>
#include <RX/vec2.h>
using namespace std;

class Homography2
{
public:
	Homography2();
	~Homography2();

	void load(std::string filename);

	RX::vec2 transform(int frame, RX::vec2 vec);

private:
	vector<RX::mat3> _homography;


};

#endif // __HOMOGRAPHY2_H
