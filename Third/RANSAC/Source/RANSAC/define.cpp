#include "define.h"

void createMatrix(matd &m, int rows, int cols)
{
	m.clear();
	for(int i = 0; i < rows; ++i) {
		vector<double> v;
		for(int j = 0; j < cols; ++j) {
			v.push_back(0.0);
		}
		m.push_back(v);
	}
}
