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

	float n;
	while(input >> n)
	{
		input >> n;
		RX::mat3d m;
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

RX::vec2d Homography::transform(int frame, RX::vec2d vec)
{
	if(frame > _hom.size()-1)
		return vec;
	
	double x = vec.x*_hom[frame].at(0, 0) + vec.y*_hom[frame].at(0, 1) + _hom[frame].at(0, 2);
	double y = vec.x*_hom[frame].at(1, 0) + vec.y*_hom[frame].at(1, 1) + _hom[frame].at(1, 2);
	double z = vec.x*_hom[frame].at(2, 0) + vec.y*_hom[frame].at(2, 1) + _hom[frame].at(2, 2);

	return RX::vec2d(x/z, y/z);
}
