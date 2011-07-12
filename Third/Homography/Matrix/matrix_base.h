//---------------------------------------------------------------------//
//  matrix_base.h: library for matrix computation (basic part)         //
//                                                                     //
//  Programmed by Naoya Ohta (1998/11/11)                              //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//

#ifndef _matrix_base_h_
#define _matrix_base_h_

class vector;

class matrix
{
public:
  // constructors
  matrix();
  matrix(int, int);
  matrix(const matrix&);

  // destructor
  ~matrix();

  // initializer
  virtual void init(int, int);
  void clear();

  // operators
  matrix& operator=(const matrix&);
  inline double* operator[](int);
  inline const double* operator[](int) const;

  // arithmetic operators
  matrix& operator+=(const matrix&);
  matrix& operator-=(const matrix&);
  matrix& operator*=(double);
  matrix& operator/=(double);

  friend matrix operator+(const matrix&, const matrix&);
  friend vector operator+(const vector&, const vector&);
  friend matrix operator-(const matrix&, const matrix&);
  friend vector operator-(const vector&, const vector&);
  friend matrix operator-(const matrix&);
  friend vector operator-(const vector&);
  friend matrix operator*(const matrix&, const matrix&);
  friend vector operator*(const matrix&, const vector&);
  friend matrix operator*(double, const matrix&);
  friend vector operator*(double, const vector&);
  friend matrix operator*(const matrix&, double);
  friend vector operator*(const vector&, double);
  friend matrix operator/(const matrix&, double);
  friend vector operator/(const vector&, double);

  // other functions
  inline int rsz() const;
  inline int csz() const;

private:
  double  *_m;   // pointer to the memory.
  int     _rsz;  // row size (hight).
  int     _csz;  // column size (width).

friend class vector;
};

class vector : public matrix
{
public:
  // constructors
  vector();
  vector(int);
  vector(const matrix&);

  // initializer
  virtual void init(int, int = 1);

  // operators
  inline double& operator[](int);
  inline const double& operator[](int) const;

  // arithmetic operators
  vector& operator+=(const vector&);
  vector& operator-=(const vector&);
  vector& operator*=(double);
  vector& operator/=(double);

  // other functions
  inline int sz() const;
};

//
// inline functions in class matrix
//

double* matrix::operator[](int i)
{
  return (_m + _csz*i);
}

const double* matrix::operator[](int i) const
{
  return (_m + _csz*i);
}

int matrix::rsz() const
{
  return _rsz;
}

int matrix::csz() const
{
  return _csz;
}

//
// inline functions in class vector
//

double& vector::operator[](int i)
{
  return _m[i];
}

const double& vector::operator[](int i) const
{
  return _m[i];
}

int vector::sz() const
{
  return _rsz;
}

#endif // _matrix_base_h_
