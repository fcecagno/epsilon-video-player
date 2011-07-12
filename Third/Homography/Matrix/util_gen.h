//---------------------------------------------------------------------//
//  util_gen.h: library for matrix computation                         //
//              (general utility functions)                            //
//  Programmed by Naoya Ohta (1998/11/11)                              //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//

#ifndef _util_gen_h_
#define _util_gen_h_

#include "matrix_base.h"
#include "matrix.h"

// convert a 1x1 matrix to scalar.
double scalar(const matrix&);
// set a matrix on another matrix.
void set(matrix&, const matrix&, int, int);
// set a vector on another vector.
void set(vector&, const vector&, int);
// extract part of a matrix.
matrix part(const matrix&, int, int, int, int);
// extract part of a vector.
vector part(const vector&, int, int);
// transpose.
matrix tp(const matrix&);
// trace.
double tr(const matrix&);
// norm.
double norm(const matrix&);
// normalized matrix.
matrix normal(const matrix&);

// inner and exterior products for vectors.
double inpro(const vector&, const vector&);
vector expro(const vector&, const vector&);

// unit matrix.
matrix unit(int);
// diagonal matrix
matrix diag(const vector&);
// square root of elements of a matrix
matrix sqrt(const matrix&);
// square root of elements of a vector
vector sqrt(const vector&);

#endif // _util_gen_h_
