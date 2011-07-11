#include "matcc.h"

matcc::matcc(int columns, int rows)
{
	//_m = new float[columns][rows];
}
matcc::~matcc()
{
	delete _m;
}

void matcc::setItem(int row, int col, float value)
{
	//_m[row][col] = value;
}

matcc &matcc::operator!()
{
	//Matrix m;
	matcc m(0, 0);
	return m;
}
