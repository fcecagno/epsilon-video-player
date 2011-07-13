#include "Homography2.h"


Homography2::Homography2()
{
}

Homography2::~Homography2()
{
}

void Homography2::load(std::string filename)
{
        ifstream input(filename.c_str());
	int n;
	while(input >> n)
	{
		RX::mat3 m;
		float a;
		for(int i = 0; i < 3; ++i) {
			for(int j = 0; j < 3; ++j) {
				input >> a;
				m.set(i, j, a);
			}
		}
		_homography.push_back(m);
	}

}

RX::vec2 Homography2::transform(int frame, RX::vec2 vec)
{
	if(frame > _homography.size()-1)
		return vec;

	RX::vec2 res = RX::vec2(vec.x-_homography[frame].at(2, 0), vec.y-_homography[frame].at(2, 1));
	return res;
}
