//---------------------------------------------------------------------//
//  util_gen.c: library for matrix computation                         //
//              (general utility functions)                            //
//  Programmed by Naoya Ohta (1998/11/11)                              //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//

#include <iostream>
#include <cmath>
#include "matrix_base.h"
#include "util_gen.h"

// convert a 1x1 matrix to scalar.
double scalar(const matrix& m)
{
  if(m.rsz() != 1 || m.csz() != 1) {
    std::cerr << "scalar(matrix): invalid dimension." << std::endl;
    exit(1); }

  return m[0][0];
}

// set a matrix on another matrix.
void set(matrix& m1, const matrix& m2, int sr, int sc)
{
  if(sr < 0 || sc < 0 || sr+m2.rsz() > m1.rsz() || sc+m2.csz() > m1.csz()) {
    std::cerr << "set(matrix,matrix,int,int): invalid position." << std::endl;
    exit(1); }

  for(int j=0; j<m2.rsz(); j++)
    for(int i=0; i<m2.csz(); i++)
      m1[j+sr][i+sc] = m2[j][i];
}

// set a vector on another vector.
void set(vector& v1, const vector& v2, int sr)
{
  if(sr < 0 || sr+v2.sz() > v1.sz()) {
    std::cerr << "set(vector,vector,int): invalid position." << std::endl;
    exit(1); }

  for(int i=0; i<v2.sz(); i++)
    v1[i+sr] = v2[i];
}

// extract part of a matrix.
matrix part(const matrix& m, int sr, int sc, int rsize, int csize)
{
  if(sr < 0 || sc < 0 || rsize < 0 || csize < 0 ||
     sr+rsize > m.rsz() || sc+csize > m.csz()) {
    std::cerr << "part(matrix,int,int,int,int): invalid part." << std::endl;
    exit(1); }

  matrix n(rsize,csize);

  for(int j=0; j<rsize; j++)
    for(int i=0; i<csize; i++)
      n[j][i] = m[j+sr][i+sc];

  return n;
}

// extract part of a vector.
vector part(const vector& v, int sr, int rsize)
{
  if(sr < 0 || rsize < 0 || sr+rsize > v.sz()) {
    std::cerr << "part(vector,int,int): invalid part." << std::endl;
    exit(1); }

  vector u(rsize);

  for(int i=0; i<rsize; i++)
    u[i] = v[i+sr];

  return u;
}

// transpose.
matrix tp(const matrix& m)
{
  matrix n(m.csz(),m.rsz());

  for(int j=0; j<n.rsz(); j++)
    for(int i=0; i<n.csz(); i++)
      n[j][i] = m[i][j];

  return n;
}  

// trace.
double tr(const matrix& m)
{
  double x = 0.0;

  if(m.rsz() != m.csz()) {
    std::cerr << "tr(matrix): not square matrix." << std::endl;
    exit(1); }

  for(int i=0; i<m.rsz(); i++)  x += m[i][i];

  return x;
}

// norm.
double norm(const matrix& m)
{
  double x = 0.0;

  for (int j=0; j<m.rsz(); j++)
    for (int i=0; i<m.csz(); i++)
      x += m[j][i]*m[j][i];

  return sqrt(x);
}

// normalized matrix.
matrix normal(const matrix& m)
{
  double x;

  if((x=norm(m)) == 0.0) {
    std::cerr << "normal(matrix): zero matrix." << std::endl;
    exit(1); }

  return m/x;
}

// inner product for vectors.
double inpro(const vector& v1, const vector& v2)
{
  if (v1.sz() != v2.sz()) {
    std::cerr << "inpro(vector,vector): dimensional error." << std::endl;
    exit(1); }

  double x = 0.0;

  for(int i=0; i<v1.sz(); i++) x += v1[i] * v2[i];

  return x;
}

// exterior product for vectors.
vector expro(const vector& v1, const vector& v2)
{
  if (v1.sz() != 3 || v2.sz() != 3) {
    std::cerr << "expro(vector,vector): dimensional error." << std::endl;
    exit(1); }

  vector u(3);

  u[0] = v1[1]*v2[2] - v1[2]*v2[1];
  u[1] = v1[2]*v2[0] - v1[0]*v2[2];
  u[2] = v1[0]*v2[1] - v1[1]*v2[0];

  return u;
}

// unit matrix.
matrix unit(int d)
{
  if (d < 1) {
    std::cerr << "unit(int): invalid dimension." << std::endl;
    exit(1); }

  matrix n(d,d);

  for(int i=0; i<d; i++) n[i][i] = 1.0;

  return n;
}

matrix diag(const vector& v)
{
  matrix n(v.sz(),v.sz());

  for(int i=0; i<v.sz(); i++) n[i][i] = v[i];

  return n;
}

// square root of elements of a matrix
matrix sqrt(const matrix& m)
{
  matrix n(m.rsz(),m.csz());

  for(int j=0; j<m.rsz(); j++)
    for(int i=0; i<m.csz(); i++)
      n[j][i] = sqrt(m[j][i]);

  return n;
}

// square root of elements of a vector
vector sqrt(const vector& v)
{
  vector u(v.sz());

  for(int i=0; i<v.sz(); i++)
    u[i] = sqrt(v[i]);

  return u;
}
