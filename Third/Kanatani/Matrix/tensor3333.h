//---------------------------------------------------------------------//
//  tensor3333.h: library for 3333-tensor computation                  //
//                                                                     //
//  Programmed by Naoya Ohta (1998/11/11)                              //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//

#ifndef _tensor3333_h_
#define _tensor3333_h_

class matrix;

class tensor3333
{
public:
  // constructors
  tensor3333();
  tensor3333(const tensor3333&);

  // initializer
  void clear();

  // operators
  tensor3333& operator=(const tensor3333&);
  double& operator()(int, int, int, int);
  const double& operator()(int, int, int, int) const;

  // arithmetic operators
  tensor3333& operator+=(const tensor3333&);
  tensor3333& operator-=(const tensor3333&);
  tensor3333& operator*=(double);
  tensor3333& operator/=(double);

  friend tensor3333 operator+(const tensor3333&, const tensor3333&);
  friend tensor3333 operator-(const tensor3333&, const tensor3333&);
  friend tensor3333 operator-(const tensor3333&);
  friend matrix     operator*(const tensor3333&, const matrix&);
  friend tensor3333 operator*(double, const tensor3333&);
  friend tensor3333 operator*(const tensor3333&, double);
  friend tensor3333 operator/(const tensor3333&, double);

private:
  double _t[81];
};

matrix     type99(const tensor3333&); // 3333-tensor -----> 99-martix
tensor3333 type3333(const matrix&);   //             <-----
matrix     type66(const tensor3333&); // (33)(33)-tensor -> 66-martix
tensor3333 type3333s(const matrix&);  //                 <-
vector     type9(const matrix&);      // 33-matrix -------> 9-vector
matrix     type33(const vector&);     //           <-------
vector     type6(const matrix&);      // (33)-matrix -----> 6-vector
matrix     type33s(const vector&);    //             <-----
vector     type3(const matrix&);      // [33]-matrix -----> 3-vector
matrix     type33a(const vector&);    //             <-----

// tensor product of 33matrix and 33matrix
tensor3333 tensprod(const matrix&, const matrix&);

// eigenvalues and eigenmatrices
void eigens(const tensor3333&, matrix*, vector&);
                                                 
// generalized inverse
tensor3333 ginvs(const tensor3333&, int);

#endif // _tensor3333_h_
