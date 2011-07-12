//---------------------------------------------------------------------//
//  util_kk.h: library for matrix computation                          //
//             (functions for Kanatani's style formalization)          //
//  Programmed by Naoya Ohta (1998/11/11)                              //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//

#ifndef _util_kk_h_
#define _util_kk_h_

#include "matrix_base.h"

// inner product
double operator,(const matrix&, const matrix&);

// exterior products
vector operator%(const vector&, const vector&);
matrix operator%(const vector&, const matrix&);
matrix operator%(const matrix&, const vector&);
matrix operator%(const matrix&, const matrix&);

// symmetrize operator
matrix S(const matrix&);
// anti-symmetrize operator
matrix A(const matrix&);

// operator t3
vector t3(const matrix&);

// Eddington's epsilon.
int eeps(int, int, int);
// Kronecker's delta.
int kdlt(int, int);

#endif // _util_kk_h_
