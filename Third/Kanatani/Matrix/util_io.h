//---------------------------------------------------------------------//
//  util_io.h: library for matrix computation (input/output functions) //
//                                                                     //
//  Programmed by Naoya Ohta (1998/11/11)                              //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//

#ifndef _util_io_h_
#define _util_io_h_

#include <iostream>
#include "matrix.h"

// input and output elements for iostream.
std::istream& operator>>(std::istream&, matrix&);
std::ostream& operator<<(std::ostream&, const matrix&);

#endif // _util_io_h_
