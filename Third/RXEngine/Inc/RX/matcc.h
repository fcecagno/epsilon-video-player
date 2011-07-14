#ifndef __MATCC_H
#define __MATCC_H

// PROVISORY
//#include <newamatap.h>
// NOT COLUMN COMPRESSED RIGHT NOW

class matcc
{
public:
	matcc(int columns, int rows);
	~matcc();
	
	matcc &operator!();

	void setItem(int row, int col, float value);
	
private:

	float *_m;

};

#endif // __MATCC_H