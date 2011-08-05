//---------------------------------------------------------------------//
//  util_kk.c: library for matrix computation                          //
//             (functions for Kanatani's style formalization)          //
//  Programmed by Naoya Ohta (1998/11/11)                              //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//

#include <iostream>
#include <math.h>
#include "matrix_base.h"
#include "util_gen.h"
#include "util_kk.h"

// inner product
double operator,(const matrix& m1, const matrix& m2)
{
  if(m1.rsz() != m2.rsz() || m1.csz() != m2.csz()) {
    std::cerr << ",(matrix,matrix): dimensional error." << std::endl;
    exit(1); }

  double x = 0.0;

  for(int j=0; j<m1.csz(); j++) 
    for(int i=0; i<m1.rsz(); i++)
      x += m1[j][i]*m2[j][i];

  return x;
}

// exterior product (v % v).
vector operator%(const vector& v1, const vector& v2)
{
  if(v1.sz() != 3 || v2.sz() != 3) {
    std::cerr << "%(vector,vector): dimensional error." << std::endl;
    exit(1); }

  vector u(3);

  u[0] = v1[1]*v2[2] - v1[2]*v2[1];
  u[1] = v1[2]*v2[0] - v1[0]*v2[2];
  u[2] = v1[0]*v2[1] - v1[1]*v2[0];

  return u;
}

// exterior product (v % m).
matrix operator%(const vector& v, const matrix& m)
{
  if(v.sz() != 3 || m.rsz() != 3 || m.csz() != 3) {
    std::cerr << "%(vector,matrix): dimensional error." << std::endl;
    exit(1); }

  matrix n(3,3);

  for(int i=0; i<3; i++) {
    n[0][i] = v[1]*m[2][i] - v[2]*m[1][i];
    n[1][i] = v[2]*m[0][i] - v[0]*m[2][i];
    n[2][i] = v[0]*m[1][i] - v[1]*m[0][i];
  }

  return n;
}

// exterior product (m % v).
matrix operator%(const matrix& m, const vector& v)
{
  if(m.rsz() != 3 || m.csz() != 3 || v.sz() != 3) {
    std::cerr << "%(matrix,vector): dimensional error." << std::endl;
    exit(1); }

  matrix n(3,3);

  for(int i=0; i<3; i++) {
    n[i][0] = m[i][2]*v[1] - m[i][1]*v[2];
    n[i][1] = m[i][0]*v[2] - m[i][2]*v[0];
    n[i][2] = m[i][1]*v[0] - m[i][0]*v[1];
  }

  return n;
}

// exterior product (m % m).
matrix operator%(const matrix& m1, const matrix& m2)
{
  if(m1.rsz() != 3 || m1.csz() != 3 || m2.rsz() != 3 || m2.csz() != 3) {
    std::cerr << "%(matrix,matrix): dimensional error." << std::endl;
    exit(1); }

  matrix n(3,3);

  n[0][0] =   m1[1][1]*m2[2][2] - m1[1][2]*m2[2][1]
            - m1[2][1]*m2[1][2] + m1[2][2]*m2[1][1];
  n[0][1] = - m1[1][0]*m2[2][2] + m1[1][2]*m2[2][0]
            + m1[2][0]*m2[1][2] - m1[2][2]*m2[1][0];
  n[0][2] =   m1[1][0]*m2[2][1] - m1[1][1]*m2[2][0]
            - m1[2][0]*m2[1][1] + m1[2][1]*m2[1][0];
  n[1][0] = - m1[0][1]*m2[2][2] + m1[0][2]*m2[2][1]
            + m1[2][1]*m2[0][2] - m1[2][2]*m2[0][1];
  n[1][1] =   m1[0][0]*m2[2][2] - m1[0][2]*m2[2][0]
            - m1[2][0]*m2[0][2] + m1[2][2]*m2[0][0];
  n[1][2] = - m1[0][0]*m2[2][1] + m1[0][1]*m2[2][0]
            + m1[2][0]*m2[0][1] - m1[2][1]*m2[0][0];
  n[2][0] =   m1[0][1]*m2[1][2] - m1[0][2]*m2[1][1]
            - m1[1][1]*m2[0][2] + m1[1][2]*m2[0][1];
  n[2][1] = - m1[0][0]*m2[1][2] + m1[0][2]*m2[1][0]
            + m1[1][0]*m2[0][2] - m1[1][2]*m2[0][0];
  n[2][2] =   m1[0][0]*m2[1][1] - m1[0][1]*m2[1][0]
            - m1[1][0]*m2[0][1] + m1[1][1]*m2[0][0];

  return n;
}

// symmetrize operator
matrix S(const matrix& m)
{
  if(m.rsz() != m.csz()) {
    std::cerr << "S(matrix): not square matrix." << std::endl;
    exit(1); }

  return (m+tp(m))/2.0;
}

// anti-symmetrize operator
matrix A(const matrix& m)
{
  if(m.rsz() != m.csz()) {
    std::cerr << "A(matrix): not square matrix." << std::endl;
    exit(1); }

  return (m-tp(m))/2.0;
}

// operator t3
vector t3(const matrix& m)
{
  if(m.rsz() != 3 || m.csz() != 3) {
    std::cerr << "t3(matrix): dimensional error." << std::endl;
    exit(1); }

  if(m[0][0] != 0.0 || m[1][1] != 0.0 || m[2][2] != 0.0 ||
     m[0][1] != -m[1][0] || m[0][2] != -m[2][0] || m[1][2] != -m[2][1]) {
    std::cerr << "t3(matrix): not anti-symmetric matrix." << std::endl;
    exit(1); }

  vector u(3);

  u[0] = m[2][1]; u[1] = m[0][2]; u[2] = m[1][0];

  return u;
}

// Eddington's epsilon.
int eeps(int i, int j, int k)
{
  if((i==0 && j==1 && k==2) || (i==1 && j==2 && k==0) ||
     (i==2 && j==0 && k==1))
    return 1;
  else if((i==0 && j==2 && k==1) || (i==1 && j==0 && k==2) ||
	  (i==2 && j==1 && k==0))
    return -1;
  else
    return 0;
}

// Kronecker's delta.
int kdlt(int i, int j)
{
  if(i == j)
    return 1;
  else
    return 0;
}
