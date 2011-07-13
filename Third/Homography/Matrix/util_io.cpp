//---------------------------------------------------------------------//
//  util_io.c: library for matrix computation (input/output functions) //
//                                                                     //
//  Programmed by Naoya Ohta (1998/11/11)                              //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//

#include <iostream>
#include <stdio.h>
#include <string>
#include "matrix_base.h"
#include "util_io.h"


// input from stream.
std::istream& operator>>(std::istream& str, matrix& m)
{
  for(int j=0; j<m.rsz(); j++)
    for(int i=0; i<m.csz(); i++)
      str >> m[j][i];

  return str;
}

// output to stream.
std::ostream& operator<<(std::ostream& str, const matrix& m)
{
  char buf[20];

  for(int j=0; j<m.rsz(); j++) {
    for(int i=0; i<(m.csz()-1); i++) {
      sprintf(buf, "%13.7g ",m[j][i]);// Somehow, function "form" doesn't work.
      str << buf; }
    sprintf(buf, "%13.7g\n",m[j][m.csz()-1]);
    str << buf;
  }

  return str;
}
