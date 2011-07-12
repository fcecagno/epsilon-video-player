//---------------------------------------------------------------------//
//  tensor3333.c: library for 3333-tensor computation                  //
//                                                                     //
//  Programmed by Naoya Ohta (1998/11/11)                              //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//

#include <stdio.h>
#include "matrix_base.h"
#include "util_gen.h"
#include "util_kk.h"
#include "util_num.h"
#include "tensor3333.h"

//constructors
tensor3333::tensor3333()
{
  for(int i=0; i<81; i++) _t[i] = 0.0;
}

tensor3333::tensor3333(const tensor3333& t)
{
   for(int i=0; i<81; i++) _t[i] = t._t[i];
}

// initializer
void tensor3333::clear()
{
   for(int i=0; i<81; i++) _t[i] = 0.0;
}

// operators
tensor3333& tensor3333::operator=(const tensor3333& t)
{
  if(&t != this)
    for(int i=0; i<81; i++) _t[i] = t._t[i];
  return *this;
}

double& tensor3333::operator()(int i, int j, int k, int l)
{
  return _t[27*i+9*j+3*k+l];
}

const double& tensor3333::operator()(int i, int j, int k, int l) const
{
  return _t[27*i+9*j+3*k+l];
}

// arithmetic operators
tensor3333& tensor3333::operator+=(const tensor3333& t)
{
  for(int i=0; i<81; i++) _t[i] += t._t[i];
  return *this;
}

tensor3333& tensor3333::operator-=(const tensor3333& t)
{
  for(int i=0; i<81; i++) _t[i] -= t._t[i];
  return *this;
}

tensor3333& tensor3333::operator*=(double a)
{
  for(int i=0; i<81; i++) _t[i] *= a;
  return *this;
}

tensor3333& tensor3333::operator/=(double a)
{
  for(int i=0; i<81; i++) _t[i] /= a;
  return *this;
}

tensor3333 operator+(const tensor3333& t1, const tensor3333& t2)
{
  tensor3333 s;

  for(int i=0; i<81; i++) s._t[i] = t1._t[i] + t2._t[i];
  return s;
}

tensor3333 operator-(const tensor3333& t1, const tensor3333& t2)
{
  tensor3333 s;

  for(int i=0; i<81; i++) s._t[i] = t1._t[i] - t2._t[i];
  return s;
}

tensor3333 operator-(const tensor3333& t)
{
  tensor3333 s;

  for(int i=0; i<81; i++) s._t[i] = -t._t[i];
  return s;
}

matrix operator*(const tensor3333& t, const matrix& m)
{
  int    i, j, k, l;
  matrix n(3,3);

  for(i=0; i<3; i++)
    for(j=0; j<3; j++) {
      double sum = 0.0;
      for(k=0; k<3; k++)
        for(l=0; l<3; l++)
          sum += t._t[27*i+9*j+3*k+l]*m[k][l];     
      n[i][j] = sum;
    }

  return n;
}

tensor3333 operator*(double a, const tensor3333& t)
{
  tensor3333 s;

  for(int i=0; i<81; i++) s._t[i] = a*t._t[i];
  return s;
}

tensor3333 operator*(const tensor3333& t, double a)
{
  tensor3333 s;

  for(int i=0; i<81; i++) s._t[i] = a*t._t[i];
  return s;
}

tensor3333 operator/(const tensor3333& t, double a)
{
  tensor3333 s;

  for(int i=0; i<81; i++) s._t[i] = t._t[i]/a;
  return s;
}

// conversion from 3333-tensor to 99-martix
matrix type99(const tensor3333& t)
{
  matrix n(9,9);

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      for(int k=0; k<3; k++)
        for(int l=0; l<3; l++)
          n[3*i+j][3*k+l] = t(i,j,k,l);

  return n;
}

// conversion from 99-martix to 3333-tensor
tensor3333 type3333(const matrix& m)
{
  if(m.rsz() != 9 || m.csz() != 9) {
    fprintf(stderr,"type3333(matrix&): invalid dimension.\n");
    exit(1); }

  tensor3333 s;

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      for(int k=0; k<3; k++)
        for(int l=0; l<3; l++)
           s(i,j,k,l) = m[3*i+j][3*k+l];

  return s;
}

// conversion from (33)(33)-tensor to 66-martix 
matrix type66(const tensor3333& t)
{
  int i, j, k, l;

  for(i=0; i<3; i++)
    for(k=0; k<3; k++)
      for(l=k+1; l<3; l++)
        if(t(i,i,l,k) != t(i,i,k,l)) {
          fprintf(stderr,"type66(tensor3333&): not symmetric tensor.\n");
          exit(1); }

  for(i=0; i<3; i++)
    for(j=i+1; j<3; j++)
      for(k=0; k<3; k++)
        for(l=k+1; l<3; l++)
          if(t(i,j,k,l) != t(i,j,l,k) || t(i,j,k,l) != t(j,i,k,l) ||
	     t(i,j,k,l) != t(j,i,l,k)) {
            fprintf(stderr,"type66(tensor3333&): not symmetric tensor.\n");
            exit(1); }

  matrix n(6,6);
  const double sr2 = 1.41421356237309504880168872421;

  n[0][0] = t(0,0,0,0);
  n[1][0] = t(1,1,0,0);
  n[2][0] = t(2,2,0,0);
  n[3][0] = sr2*t(1,2,0,0);
  n[4][0] = sr2*t(2,0,0,0);
  n[5][0] = sr2*t(0,1,0,0);

  n[0][1] = t(0,0,1,1);
  n[1][1] = t(1,1,1,1);
  n[2][1] = t(2,2,1,1);
  n[3][1] = sr2*t(1,2,1,1);
  n[4][1] = sr2*t(2,0,1,1);
  n[5][1] = sr2*t(0,1,1,1);

  n[0][2] = t(0,0,2,2);
  n[1][2] = t(1,1,2,2);
  n[2][2] = t(2,2,2,2);
  n[3][2] = sr2*t(1,2,2,2);
  n[4][2] = sr2*t(2,0,2,2);
  n[5][2] = sr2*t(0,1,2,2);

  n[0][3] = sr2*t(0,0,1,2);
  n[1][3] = sr2*t(1,1,1,2);
  n[2][3] = sr2*t(2,2,1,2);
  n[3][3] = 2.0*t(1,2,1,2);
  n[4][3] = 2.0*t(2,0,1,2);
  n[5][3] = 2.0*t(0,1,1,2);

  n[0][4] = sr2*t(0,0,2,0);
  n[1][4] = sr2*t(1,1,2,0);
  n[2][4] = sr2*t(2,2,2,0);
  n[3][4] = 2.0*t(1,2,2,0);
  n[4][4] = 2.0*t(2,0,2,0);
  n[5][4] = 2.0*t(0,1,2,0);

  n[0][5] = sr2*t(0,0,0,1);
  n[1][5] = sr2*t(1,1,0,1);
  n[2][5] = sr2*t(2,2,0,1);
  n[3][5] = 2.0*t(1,2,0,1);
  n[4][5] = 2.0*t(2,0,0,1);
  n[5][5] = 2.0*t(0,1,0,1);

  return n;
}

// conversion from 66-martix to (33)(33)-tensor
tensor3333 type3333s(const matrix& m)
{
  if(m.rsz() != 6 || m.csz() != 6) {
    fprintf(stderr,"type3333s(matrix&): invalid dimension.\n");
    exit(1); }

  tensor3333 s;
  const double sr2 = 1.41421356237309504880168872421;

  s(0,0,0,0) = m[0][0];
  s(0,0,1,1) = m[0][1];
  s(0,0,2,2) = m[0][2];
  s(0,0,1,2) = s(0,0,2,1) = m[0][3]/sr2;
  s(0,0,2,0) = s(0,0,0,2) = m[0][4]/sr2;
  s(0,0,0,1) = s(0,0,1,0) = m[0][5]/sr2;

  s(1,1,0,0) = m[1][0];
  s(1,1,1,1) = m[1][1];
  s(1,1,2,2) = m[1][2];
  s(1,1,1,2) = s(1,1,2,1) = m[1][3]/sr2;
  s(1,1,2,0) = s(1,1,0,2) = m[1][4]/sr2;
  s(1,1,0,1) = s(1,1,1,0) = m[1][5]/sr2;

  s(2,2,0,0) = m[2][0];
  s(2,2,1,1) = m[2][1];
  s(2,2,2,2) = m[2][2];
  s(2,2,1,2) = s(2,2,2,1) = m[2][3]/sr2;
  s(2,2,2,0) = s(2,2,0,2) = m[2][4]/sr2;
  s(2,2,0,1) = s(2,2,1,0) = m[2][5]/sr2;

  s(1,2,0,0) = s(2,1,0,0) = m[3][0]/sr2;
  s(1,2,1,1) = s(2,1,1,1) = m[3][1]/sr2;
  s(1,2,2,2) = s(2,1,2,2) = m[3][2]/sr2;
  s(1,2,1,2) = s(1,2,2,1) = s(2,1,1,2) = s(2,1,2,1) = m[3][3]/2.0;
  s(1,2,2,0) = s(1,2,0,2) = s(2,1,2,0) = s(2,1,0,2) = m[3][4]/2.0;
  s(1,2,0,1) = s(1,2,1,0) = s(2,1,0,1) = s(2,1,1,0) = m[3][5]/2.0;

  s(2,0,0,0) = s(0,2,0,0) = m[4][0]/sr2;
  s(2,0,1,1) = s(0,2,1,1) = m[4][1]/sr2;
  s(2,0,2,2) = s(0,2,2,2) = m[4][2]/sr2;
  s(2,0,1,2) = s(2,0,2,1) = s(0,2,1,2) = s(0,2,2,1) = m[4][3]/2.0;
  s(2,0,2,0) = s(2,0,0,2) = s(0,2,2,0) = s(0,2,0,2) = m[4][4]/2.0;
  s(2,0,0,1) = s(2,0,1,0) = s(0,2,0,1) = s(0,2,1,0) = m[4][5]/2.0;

  s(0,1,0,0) = s(1,0,0,0) = m[5][0]/sr2;
  s(0,1,1,1) = s(1,0,1,1) = m[5][1]/sr2;
  s(0,1,2,2) = s(1,0,2,2) = m[5][2]/sr2;
  s(0,1,1,2) = s(0,1,2,1) = s(1,0,1,2) = s(1,0,2,1) = m[5][3]/2.0;
  s(0,1,2,0) = s(0,1,0,2) = s(1,0,2,0) = s(1,0,0,2) = m[5][4]/2.0;
  s(0,1,0,1) = s(0,1,1,0) = s(1,0,0,1) = s(1,0,1,0) = m[5][5]/2.0;

  return s;
}

// conversion from 33-matrix to 9-vector
vector type9(const matrix& m)
{
  if(m.rsz() != 3 || m.csz() != 3) {
    fprintf(stderr,"type9(matrix&): invalid dimension.\n");
    exit(1); }

  vector u(9);

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      u[3*i+j] = m[i][j];

  return u;
}

// conversion from 9-vector to 33-matrix
matrix type33(const vector& v)
{
  if(v.sz() != 9) {
    fprintf(stderr,"type33(vector&): invalid dimension.\n");
    exit(1); }

  matrix n(3,3);
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      n[i][j] = v[3*i+j];

  return n;
}

// conversion from (33)-matrix to 6-vector
vector type6(const matrix& m)
{
  if(m.rsz() != 3 || m.csz() != 3) {
    fprintf(stderr,"type6(matrix&): invalid dimension.\n");
    exit(1); }

  if(m[0][1] != m[1][0] || m[0][2] != m[2][0] || m[1][2] != m[2][1]) {
    fprintf(stderr,"type6(matrix&): not symmetric matrix.\n");
    exit(1); }

  vector u(6);
  const double sr2 = 1.41421356237309504880168872421;

  u[0] = m[0][0];     u[1] = m[1][1];     u[2] = m[2][2];
  u[3] = sr2*m[1][2]; u[4] = sr2*m[2][0]; u[5] = sr2*m[0][1];

  return u;
}

// conversion from 6-vector to (33)-matrix
matrix type33s(const vector& v)
{
  if(v.sz() != 6) {
    fprintf(stderr,"type33s(vector&): invalid dimension.\n");
    exit(1); }

  matrix n(3,3);
  const double sr2 = 1.41421356237309504880168872421;

  n[0][0] = v[0];     n[0][1] = v[5]/sr2; n[0][2] = v[4]/sr2;
  n[1][0] = v[5]/sr2; n[1][1] = v[1];     n[1][2] = v[3]/sr2;
  n[2][0] = v[4]/sr2; n[2][1] = v[3]/sr2; n[2][2] = v[2];

  return n;
}

// conversion from [33]-matrix to 3-vector
vector type3(const matrix& m)
{
  if(m.rsz() != 3 || m.csz() != 3) {
    fprintf(stderr,"type3(matrix&): invalid dimension.\n");
    exit(1); }

  if(m[0][0] != 0.0 || m[1][1] != 0.0 || m[2][2] != 0.0 ||
     m[0][1] != -m[1][0] || m[0][2] != -m[2][0] || m[1][2] != -m[2][1]) {
    fprintf(stderr,"type3(matrix&): not antisymmetric matrix.\n");
    exit(1); }

  vector u(3);

  u[0] = m[2][1]; u[1] = m[0][2]; u[2] = m[1][0];

  return u;
}

// conversion from 3-vector to [33]-matrix
matrix type33a(const vector& v)
{
  if(v.sz() != 3) {
    fprintf(stderr,"type33a(vector&): invalid dimension.\n");
    exit(1); }

  matrix n(3,3);

  n[0][0] = n[1][1] = n[2][2] = 0.0;
  n[2][1] = v[0]; n[1][2] = -v[0];
  n[0][2] = v[1]; n[2][0] = -v[1];
  n[1][0] = v[2]; n[0][1] = -v[2];

  return n;
}

// tensor product of 33matrix and 33matrix
tensor3333 tensprod(const matrix& m1, const matrix& m2)
{
  if(m1.rsz() != 3 || m1.csz() != 3 || m2.rsz() != 3 || m2.csz() != 3) {
    fprintf(stderr,"tensprod(matrix&,matrix&): invalid dimension.\n");
    exit(1); }

  tensor3333 s;

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      for(int k=0; k<3; k++)
	for(int l=0; l<3; l++)
	  s(i,j,k,l) = m1[i][j]*m2[k][l];

  return s;
}

// eigenvalues and eigenmatrices
void eigens(const tensor3333& t, matrix* ma, vector& v)
{
  matrix ev(9,9);

  eigens(S(type99(t)),ev,v);
  for(int i=0; i<9; i++) ma[i] = type33(part(ev,0,i,9,1));
}

// generalized inverse
tensor3333 ginvs(const tensor3333& t, int rank)
{
  return type3333(ginvs(S(type99(t)),rank));
}
