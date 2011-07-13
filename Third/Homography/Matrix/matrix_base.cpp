//---------------------------------------------------------------------//
//  matrix_base.c: library for matrix computation (basic part)         //
//                                                                     //
//  Programmed by Naoya Ohta (1998/11/11)                              //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//

#include <iostream>
#include "matrix_base.h"

//
// member functions of class matrix
//

// constructors
matrix::matrix()
{
  _m = (double *)0;
  _rsz = _csz = 0;
}

matrix::matrix(int rsz, int csz)
{
  if(rsz < 1 || csz < 1) {
    std::cerr << "matrix::matrix(int,int): invalid dimension." << std::endl;
    exit(1); }

  _rsz = rsz;
  _csz = csz;

  if((_m = new double[_rsz*_csz]) == 0) {
    std::cerr << "matrix::matrix(int,int): memory allocation error." << std::endl;
    exit(1); }

  for(int i=0; i<(_rsz*_csz); i++) _m[i] = 0.0;
}

// copy constructor
matrix::matrix(const matrix& m)
{
  if(m._m == 0) {
    std::cerr << "matrix::matrix(matrix): copy null matrix." << std::endl;
    exit(1); }

  _rsz = m._rsz;
  _csz = m._csz;

  if((_m = new double[_rsz*_csz]) == 0) {
    std::cerr << "matrix::matrix(matrix): memory allocation error." << std::endl;
    exit(1); }

  for(int i=0; i<(_rsz*_csz); i++) _m[i] = m._m[i];
}

// assignment operator
matrix& matrix::operator=(const matrix& m)
{
  if(_m == 0 || m._m == 0) {
    std::cerr << "matrix::=(matrix): assign null matrix." << std::endl;
    exit(1); }
  
  if(this == &m) return *this;

  if(_rsz != m._rsz || _csz != m._csz) {
    std::cerr << "matrix::=(matrix): dimension conflict." << std::endl;
    exit(1); }

  for(int i=0; i<(_rsz*_csz); i++) _m[i] = m._m[i];

  return *this;
}

// destructor
matrix::~matrix()
{
  if(_m != 0) delete [] _m;
}

// initializers
void matrix::init(int rsz, int csz)
{
  if(csz < 1 || rsz < 1) {
    std::cerr << "matrix::init(int,int): invalid dimension." << std::endl;
    exit(1); }

  if(_m != 0) delete [] _m;

  _rsz = rsz;
  _csz = csz;

  if((_m = new double[_rsz*_csz]) == 0) {
    std::cerr << "matrix::init(int,int): memory allocation error." << std::endl;
    exit(1); }

  for(int i=0; i<(_rsz*_csz); i++) _m[i] = 0.0;
}

void matrix::clear()
{
  if(_m != 0) {
    for(int i=0; i<(_rsz*_csz); i++) _m[i] = 0.0; }
}

// arithmetic operators
matrix& matrix::operator+=(const matrix& m)
{
  if(_rsz != m._rsz || _csz != m._csz) {
    std::cerr << "matrix::+=(matrix): dimension conflict." << std::endl;
    exit(1); }

  for(int i=0; i<(_rsz*_csz); i++) _m[i] += m._m[i];

  return *this;
}

matrix& matrix::operator-=(const matrix& m)
{
  if(_rsz != m._rsz || _csz != m._csz) {
    std::cerr << "matrix::-=(matrix): dimension conflict." << std::endl;
    exit(1); }

  for(int i=0; i<(_rsz*_csz); i++) _m[i] -= m._m[i];

  return *this;
}

matrix& matrix::operator*=(double a)
{
  for(int i=0; i<(_rsz*_csz); i++) _m[i] *= a;

  return *this;
}

matrix& matrix::operator/=(double a)
{
  for(int i=0; i<(_csz*_rsz); i++) _m[i] /= a;

  return *this;
}

//
// member functions of class vector
//

// constructors
vector::vector() : matrix() {}

vector::vector(int sz) : matrix(sz,1) {}

vector::vector(const matrix& m) : matrix()
{
  if(m._csz != 1) {
    std::cerr << "vector::vector(matrix): convert invalid matrix." << std::endl;
    exit(1); }

  matrix::init(m._rsz,1);

  for(int i=0; i<m._rsz; i++) _m[i] = m._m[i];
}

// initializer
void vector::init(int rsz, int csz)
{
  if(csz != 1) {
    std::cerr << "vector::init(int,int): invalid dimension." << std::endl;
    exit(1); }

  matrix::init(rsz,1);
}

// arithmetic operators
vector& vector::operator+=(const vector& v)
{
  matrix::operator+=(v);
  return *this;
}

vector& vector::operator-=(const vector& v)
{
  matrix::operator-=(v);
  return *this;
}

vector& vector::operator*=(double a)
{
  matrix::operator*=(a);
  return *this;
}

vector& vector::operator/=(double a)
{
  matrix::operator/=(a);
  return *this;
}

//
// friend functions of class matrix
//

matrix operator+(const matrix& m1, const matrix& m2)
{
  if(m1._rsz != m2._rsz || m1._csz != m2._csz) {
    std::cerr << "+(matrix,matrix): dimension conflict." << std::endl;
    exit(1); }

  matrix n(m1._rsz,m1._csz);

  for(int i=0; i<(m1._rsz*m1._csz); i++) n._m[i] = m1._m[i] + m2._m[i];

  return n;
}

vector operator+(const vector& v1, const vector& v2)
{
  if(v1._rsz != v2._rsz) {
    std::cerr << "+(vector,vector): dimension conflict." << std::endl;
    exit(1); }

  vector u(v1._rsz);

  for(int i=0; i<v1._rsz; i++) u._m[i] = v1._m[i] + v2._m[i];

  return u;
}

matrix operator-(const matrix& m1, const matrix& m2)
{
  if(m1._rsz != m2._rsz || m1._csz != m2._csz) {
    std::cerr << "-(matrix,matrix): dimension conflict." << std::endl;
    exit(1); }

  matrix n(m1._rsz,m1._csz);

  for(int i=0; i<(m1._rsz*m1._csz); i++) n._m[i] = m1._m[i] - m2._m[i];

  return n;
}

vector operator-(const vector& v1, const vector& v2)
{
  if(v1._rsz != v2._rsz) {
    std::cerr << "-(vector,vector): dimension conflict." << std::endl;
    exit(1); }

  vector u(v1._rsz);

  for(int i=0; i<v1._rsz; i++) u._m[i] = v1._m[i] - v2._m[i];

  return u;
}

matrix operator-(const matrix& m)
{
  matrix n(m._rsz,m._csz);

  for(int i=0; i<(m._rsz*m._csz); i++) n._m[i] = -m._m[i];

  return n;
}

vector operator-(const vector& v)
{
  vector u(v._rsz);

  for(int i=0; i<v._rsz; i++) u._m[i] = -v._m[i];

  return u;
}

matrix operator*(const matrix& m1, const matrix& m2)
{
  if(m1._csz != m2._rsz) {
    std::cerr << "*(matrix,matrix): dimension conflict." << std::endl;
    exit(1); }

  matrix n(m1._rsz,m2._csz);

  for(int j=0; j<m1._rsz; j++)
    for(int i=0; i<m2._csz; i++) {
      double sum = 0.0;
      for(int k=0; k<m1._csz; k++) {
	sum += m1._m[j*m1._csz+k] * m2._m[k*m2._csz+i]; }
      n._m[j*m2._csz+i] = sum;
    }

  return n;
}

vector operator*(const matrix& m, const vector& v)
{
  if(m._csz != v._rsz) {
    std::cerr << "*(matrix,vector): dimension conflict." << std::endl;
    exit(1); }

  vector u(m._rsz);

  for(int j=0; j<m._rsz; j++) {
    double sum = 0.0;
    for(int i=0; i<m._csz; i++) sum += m._m[j*m._csz+i] * v._m[i];
    u._m[j] = sum;
  }

  return u;
}

matrix operator*(double a, const matrix& m)
{
  matrix n(m._rsz,m._csz);

  for(int i=0; i<(m._rsz*m._csz); i++) n._m[i] = a*m._m[i];

  return n;
}

vector operator*(double a, const vector& v)
{
  vector u(v.sz());

  for(int i=0; i<v._rsz; i++) u._m[i] = a*v._m[i];

  return u;
}

matrix operator*(const matrix& m, double a)
{
  matrix n(m._rsz,m._csz);

  for(int i=0; i<(m._rsz*m._csz); i++) n._m[i] = a*m._m[i];

  return n;
}

vector operator*(const vector& v, double a)
{
  vector u(v._rsz);

  for(int i=0; i<v._rsz; i++) u._m[i] = a*v._m[i];

  return u;
}

matrix operator/(const matrix& m, double a)
{
  matrix n(m._rsz,m._csz);

  for(int i=0; i<(m._rsz*m._csz); i++) n._m[i] = m._m[i]/a;

  return n;
}

vector operator/(const vector& v, double a)
{
  vector u(v._rsz);

  for(int i=0; i<v._rsz; i++) u._m[i] = v._m[i]/a;

  return u;
}
