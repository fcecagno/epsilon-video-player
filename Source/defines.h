#ifndef __DEFINES_H
#define __DEFINES_H

#include <vector>
using namespace std;

typedef vector< vector<double> > matd;
typedef unsigned char uchar;

void createMatrix(matd &m, int rows, int cols);

#endif // __DEFINES_H