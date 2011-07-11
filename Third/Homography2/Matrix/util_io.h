//---------------------------------------------------------------------//
//  util_io.h: library for matrix computation (input/output functions) //
//                                                                     //
//  Programmed by Naoya Ohta (1998/11/11)                              //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//

#ifndef _util_io_h_
#define _util_io_h_

#include <iostream.h>

// input and output elements for iostream.
istream& operator>>(istream&, matrix&);
ostream& operator<<(ostream&, const matrix&);

#endif // _util_io_h_
