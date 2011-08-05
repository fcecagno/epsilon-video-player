#include <iostream>
#include "matrix_base.h"
#include "util_gen.h"
#include "util_num.h"

// inverse matrix by LU decomposition
matrix inv(const matrix& m)
{
  double **mm, **imidx, det;
  int    i, j, n = m.rsz();
  double alg_matinv(int, double**, double**);

  // check dimension.
  if(n < 1 || m.csz() != n) {
    std::cerr << "inv: invalid dimension." << std::endl;
    exit(1); }

  // memory allocation for buffers.
  if((mm = new double*[n]) == 0 || (imidx = new double*[n]) == 0 ||
     (mm[0] = new double[n*n]) == 0 ) {
    std::cerr << "inv: memory allocation error." << std::endl;
    exit(1); }

  matrix im(n,n);

  // setup of index memories.
  imidx[0] = &(im[0][0]);
  for(i=1; i<n; i++) {
    mm[i] = mm[0] + n*i;
    imidx[i] = imidx[0] + n*i; }

  // copy the input matrix.
  for(i=0; i<n; i++)
    for(j=0; j<n; j++)
      mm[i][j] = m[i][j];

  det = alg_matinv(n,mm,imidx);

  // check determinant.
  if(det == 0.0) {
    std::cerr << "inv: singular matrix." << std::endl;
    exit(1); }

  delete [] mm[0], mm, imidx;
  return im;
}

// determinant by LU decomposition
double det(const matrix& m)
{
  double **mm, det;
  int    i, j, n = m.rsz();
  double alg_det(int, double**);

  // check dimension.
  if(n < 1 || m.csz() != n) {
    std::cerr << "det: invalid dimension." << std::endl;
    exit(1); }

  // memory allocation for buffers.
  if((mm = new double*[n]) == 0 || (mm[0] = new double[n*n]) == 0 ) {
    std::cerr << "det: memory allocation error." << std::endl;
    exit(1); }

  // setup of index memories.
  for(i=1; i<n; i++) mm[i] = mm[0] + n*i;

  // copy the input matrix.
  for(i=0; i<n; i++)
    for(j=0; j<n; j++)
      mm[i][j] = m[i][j];

  det = alg_det(n,mm);

  delete [] mm[0], mm;
  return det;
}

// spectral decomposition for symmetric matrix
void eigens(const matrix& m, matrix& u, vector& v)
{
  double  **uptr, *w;
  int     i, j;
  int     n = m.rsz();

  int alg_eigen(int, double**, double*);

  // check dimensions.
  if(m.csz() != n || u.rsz() != n || u.csz() != n || v.sz() != n) {
    std::cerr << "eigens: invalid dimension." << std::endl;
    exit(1); }

  // check being symmetric.
  for(j=1; j<n; j++)
    for(i=0; i<j; i++)
      if(m[j][i] != m[i][j]) {
        std::cerr << "eigens: not symmetric matrix." << std::endl;
        exit(1); }

  // allocate memory for work vector.
  if((w = new double[n]) == 0) {
    std::cerr << "eigens: memory allocation error." << std::endl;
    exit(1); }

  // copy matrix m to matrix u.
  u = m;

  // preparation for matrix u.
  if((uptr = new double*[n]) == 0) {
    std::cerr << "eigens: memory allocation error." << std::endl;
    exit(1); }
  for(i=0; i<n; i++) uptr[i] = &(u[0][0]) + n*i;

  // calculate eigenvalues and eigenvectors
  if(alg_eigen(n,uptr,&(v[0])) != 0) {
    std::cerr << "eigens: computation error." << std::endl;
    exit(1); }

  // rearrangement of elements of matrix
  for(i=0; i<n; i++) {
    for(j=0; j<n; j++) w[j] = uptr[j][i];
    for(j=0; j<n; j++) u[j][i] = w[j];
  }

  // transpose matrix u
  for(j=0; j<n-1; j++)
    for(i=j+1; i<n; i++) {
      double x = u[i][j]; u[i][j] = u[j][i]; u[j][i] = x;
    }

  // free memories
  delete [] uptr, w;
}

// inverse for symmetric matrix by spectral decomposition
matrix invs(const matrix& m, double cn)
{
  int  n = m.rsz();

  // check dimensions.
  if(n < 1 || m.csz() != n) {
    std::cerr << "invs: invalid dimension." << std::endl;
    exit(1); }

  // check designated condition number
  if(cn < 1.0) {
    std::cerr << "invs: invalid condition number." << std::endl;
    exit(1); }

  // spectral decomposition
  matrix u(n,n);
  vector v(n);
  eigens(m,u,v);

  // search minimum and maximum absolute values of eigenvalues
  double min = (v[0]>=0.0 ? v[0] : -v[0]);
  double max = min;
  for(int i=1; i<n; i++) {
    double x = (v[i]>=0.0 ? v[i] : -v[i]);
    if(x < min) {
      min = x; }
    else if(x > max) {
      max = x; }
  }

  // check condition number
  if(max >= cn*min) {
    std::cerr << "invs: ill condition." << std::endl;
    exit(1); }

  // compute inverse
  for(int i=0; i<n; i++) v[i] = 1.0/v[i];
  return u*diag(v)*tp(u);
}

// generalized inverse for symmetric matrix by spectral decomposition
matrix ginvs(const matrix& m, int rank, double cn)
{
  int  n = m.rsz();
  int  i;

  // check dimensions.
  if(n < 1 || m.csz() != n) {
    std::cerr << "ginvs: invalid dimension." << std::endl;
    exit(1); }

  // check designated rank
  if(rank < 1 || rank > n) {
    std::cerr << "ginvs: invalid rank." << std::endl;
    exit(1); }

  // check designated condition number
  if(cn < 1.0) {
    std::cerr << "ginvs: invalid condition number." << std::endl;
    exit(1); }

  // memory allocation for work buffers
  matrix u(n,n);
  vector v(n);
  int idx[100];

  // spectral decomposition
  eigens(m,u,v);

  // sort by absolute value of eigenvalues
  for(i=0; i<n; i++) idx[i] = i;
  for(i=0; i<n-1; i++)
    for(int j=i+1; j<n; j++) {
      double absi = v[idx[i]], absj = v[idx[j]];
      if(absi < 0.0) absi *= -1.0;
      if(absj < 0.0) absj *= -1.0;
      if(absj > absi) { int k = idx[i]; idx[i] = idx[j]; idx[j] = k; }
    }

  // check condition number
  double emax = v[idx[0]], emin = v[idx[rank-1]];
  if(emax < 0.0) emax *= -1.0;
  if(emin < 0.0) emin *= -1.0;
  if(emax >= cn*emin) {
    std::cerr << "ginvs: ill condition." << std::endl;
    exit(1); }

  // compute generalized inverse
  for(i=0; i<rank; i++) v[idx[i]] = 1.0/v[idx[i]];
  for(i=rank; i<n; i++) v[idx[i]] = 0.0;
  return u*diag(v)*tp(u);
}

// determinant for symmetric matrix by spectral decomposition
double dets(const matrix& m)
{
  int  n = m.rsz();

  // check dimension.
  if(n < 1 || m.csz() != n) {
    std::cerr << "dets: invalid dimension." << std::endl;
    exit(1); }

  // spectral decomposition
  matrix u(n,n);
  vector v(n);
  eigens(m,u,v);

  // compute determinant
  double det = v[0];
  for(int i=1; i<n; i++) det *= v[i];

  return det;
}
