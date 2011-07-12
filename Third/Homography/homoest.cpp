#include <cmath>
#include "Matrix/matrix.h"

//---------------------------------------------------------------------//
//  homoest: Homography estimation from correcpondence between two     //
//           sets of image points.                                     //
//  Programmed by Naoya Ohta and Shimizu Yoshiyuki (1999/2/25)         //
//  Computer Science Department, Gunma University                      //
//---------------------------------------------------------------------//
int homoest(const vector *x1, const matrix *Vx1,
            const vector *x2, const matrix *Vx2, int pn,
            matrix& Hhat, matrix& Hp, matrix& Hm, 
            double& eps2, double epslmd, int itmax)

//  x1     : Array of image points in set 1.                    (input)
//  Vx1    : Array of covariance matrix of x1.                  (input)
//  x2     : Array of image points in set 2.                    (input)
//  Vx2    : Array of covariance matrix of x2.                  (input)
//  pn     : Number of image points in a set. (Size of arrays)  (input)
//  Hhat   : Estimated homography matrix.                       (output)
//  Hp     : Standard deviation of Hhat. (+)                    (output)
//  Hm     : Standard deviation of Hhat. (-)                    (output)
//  eps2   : Squared noise level                                (output)
//  epslmd : Constant for convergence test.                     (input)
//  itmax  : Maximum iteration number. (>=1)                    (input)
//
//  Return value : iteration number
//               : -1 (if iteration number exceeds itmax)

{
  tensor3333  M, N;
  matrix      W(3,3), A[3], H[9], dH(3,3);
  vector      e1(3), e2(3), e3(3), lmd(9);
  double      c;
  int         a, i, j, k, l, it;

  // initialization
  for(i=0; i<3; i++) { A[i].init(3,3); }
  for(i=0; i<9; i++) { H[i].init(3,3); }
  e1.clear(); e2.clear(); e3.clear();
  e1[0] = e2[1] = e3[2] = 1.0;
  c = 0.0;
  W = unit(3);

  for(it=1; ; it++) { // renormalization loop

    M.clear(); N.clear();
    for(a=0; a<pn; a++) {
      int n, m, p, q;

      // computation of weight matrix W for current point a
      if(it != 1) {
        matrix m1 = x2[a]%(H[8]*Vx1[a]*tp(H[8]))%x2[a];
        matrix m2 = (H[8]*x1[a])%Vx2[a]%(H[8]*x1[a]);
        W = ginvs(S(m1+m2),2);
      }

      // accumulation for a half part of tensor N
      for(i=0; i<3; i++)
      for(j=0; j<3; j++)
      for(k=0; k<3; k++)
      for(l=0; l<3; l++)
        if((3*(i-1)+j) <= (3*(k-1)+l)) {
	  for(m=0; m<3; m++)
          for(n=0; n<3; n++)
          for(p=0; p<3; p++)
          for(q=0; q<3; q++) {
	    int s = eeps(i,m,p)*eeps(k,n,q);
            if(s != 0)
              N(i,j,k,l) += double(s)*W[m][n]*
                            (Vx1[a][j][l]*x2[a][p]*x2[a][q] +
                             Vx2[a][p][q]*x1[a][j]*x1[a][l]);
	  }
	}

      // accumulation for tensor M
      A[0] = e1%(x2[a]*tp(x1[a]));
      A[1] = e2%(x2[a]*tp(x1[a]));
      A[2] = e3%(x2[a]*tp(x1[a]));
      for (k=0; k<3; k++)
        for (l=0; l<3; l++)
          M += W[k][l]*tensprod(A[k],A[l]);

    } // loop end of a

    // fill the other half of tensor N
    for(i=0; i<3; i++)
    for(j=0; j<3; j++)
    for(k=0; k<3; k++)
    for(l=0; l<3; l++)
      if((3*(i-1)+j) > (3*(k-1)+l))
        N(i,j,k,l) = N(k,l,i,j);

    M /= double(pn);
    N /= double(pn);

    // computation of eigenmatrices
    if(it == 1)
      eigens(M,H,lmd);
    else
      eigens(M-c*N,H,lmd);

    // condition to quit the renormalization loop
    if(fabs(lmd[8]) < epslmd) break;
    if(it == itmax) { it = -1; break; }

    // update c
    c += lmd[8]/(H[8],N*H[8]);

  } // renormalization loop end

  Hhat = H[8];
  if(det(Hhat) < 0.0) Hhat *= -1; // just eliminate the sign ambiguity.
  eps2 = c/(1.0 - 4.0/double(pn));
  dH = sqrt(eps2/(lmd[7]*double(pn)))*H[7];
  Hp = normal(Hhat + dH);
  Hm = normal(Hhat - dH);

  return it;
}

//---------------------------------------------------------------------//
//  Utility functions                                                  //
//---------------------------------------------------------------------//
int scale(vector *i1, vector *i2, vector *x1, vector *x2,
          vector& ic1, vector& ic2, double& f0, int pn)
{
  // compute centroids
  ic1.clear();
  ic2.clear();

  for(int a=0; a<pn; a++) {
    ic1 += i1[a];
    ic2 += i2[a]; }

  ic1 /= double(pn);
  ic2 /= double(pn);

  // search min and max values
  double i1xmin, i1xmax, i1ymin, i1ymax, i2xmin, i2xmax, i2ymin, i2ymax;
  i1xmin = i1xmax = i1[0][0]; i1ymin = i1ymax = i1[0][1];
  i2xmin = i2xmax = i2[0][0]; i2ymin = i2ymax = i2[0][1];

  for (int a=1; a<pn; a++) {
    if(i1xmin > i1[a][0]) i1xmin = i1[a][0];
    if(i1xmax < i1[a][0]) i1xmax = i1[a][0];
    if(i1ymin > i1[a][1]) i1ymin = i1[a][1];
    if(i1ymax < i1[a][1]) i1ymax = i1[a][1];
    if(i2xmin > i2[a][0]) i2xmin = i2[a][0];
    if(i2xmax < i2[a][0]) i2xmax = i2[a][0];
    if(i2ymin > i2[a][1]) i2ymin = i2[a][1];
    if(i2ymax < i2[a][1]) i2ymax = i2[a][1];
  }

  f0 = i1xmax - i1xmin;
  if(f0 < (i1ymax-i1ymin)) f0 = i1ymax - i1ymin;
  if(f0 < (i2xmax-i2xmin)) f0 = i2xmax - i2xmin;
  if(f0 < (i2ymax-i2ymin)) f0 = i2ymax - i2ymin;

  if(f0 == 0.0) return 1;

  f0 *= 2.0;

  // compute scaled data
  for(int a=0; a<pn; a++) {
    x1[a][0] = (ic1[0] - i1[a][0])/f0;
    x1[a][1] = (i1[a][1] - ic1[1])/f0;
    x2[a][0] = (ic2[0] - i2[a][0])/f0;
    x2[a][1] = (i2[a][1] - ic2[1])/f0;
    x1[a][2] = x2[a][2] = 1.0;
  }

  return 0;
}

matrix crdcnv(const matrix& H, const vector& ic1, const vector& ic2, double f0)
{
  matrix A(3,3), B(3,3);

  A.clear(); B.clear();

  A[0][0] = -1.0/f0;
  A[1][1] = 1.0/f0;
  A[0][2] = ic1[0]/f0;
  A[1][2] = -ic1[1]/f0;
  A[2][2] = 1.0;
  B[0][0] = -f0;
  B[1][1] = f0;
  B[0][2] = ic2[0];
  B[1][2] = ic2[1];
  B[2][2] = 1.0;

  return normal(B*H*A);
}
