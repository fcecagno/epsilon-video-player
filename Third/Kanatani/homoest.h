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
            double& eps2, double epslmd = 1.0e-10, int itmax = 100);

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

//---------------------------------------------------------------------//
//  Utility functions                                                  //
//---------------------------------------------------------------------//
int scale(vector *i1, vector *i2, vector *x1, vector *x2,
          vector& ic1, vector& ic2, double& f0, const int pn);

matrix crdcnv(const matrix& H, const vector& ic1, const vector& ic2,
              double f0);
