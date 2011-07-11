#ifndef _util_num_h_
#define _util_num_h_

// inverse matrix by LU decomposition
matrix inv(const matrix&);

// determinant by LU decomposition
double det(const matrix&);

// spectral decomposition for symmetric matrix
void eigens(const matrix&, matrix&, vector&);

// inverse for symmetric matrix by spectral decomposition
matrix invs(const matrix&, double = 1.0e10);

// generalized inverse for symmetric matrix by spectral decomposition
matrix ginvs(const matrix&, int, double = 1.0e10);

// determinant for symmetric matrix by spectral decomposition
double dets(const matrix&);

#endif // _util_num_h_
