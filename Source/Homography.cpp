#include "Homography.h"


Homography::Homography()
{
}

Homography::~Homography()
{
}

void Homography::clear()
{
	_hom.clear();
}

void Homography::load(std::string filename)
{
	ifstream input(filename);

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
		_hom.push_back(m);
	}

}

RX::vec2 Homography::transform(int frame, RX::vec2 vec)
{
	if(frame > _hom.size()-1)
		return vec;

	RX::vec2 res = RX::vec2(vec.x-_hom[frame].at(2, 0), vec.y-_hom[frame].at(2, 1));
	return res;
}
