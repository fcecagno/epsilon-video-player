/* matinv.cpp -- inverse of matrix using Gauss-Jordan elimination.
 * (C) 2001, C. Bond. All rights reserved.
 */
#include <math.h>
#include <RX/mat3d.h>
using namespace RX;
/* Bare-bones Gauss-Jordan algorithm. Returns '0' on success, '1' if
 * matrix is singular (zero diagonal). No pivoting used.
 *
 * Replaces input matrix with its inverse.
 */

int matinv(mat3d a, mat3d &inv)
{
	inv.setIdentity();
    for (int k=0;k<3;k++) {
		float tmp = a.at(k,k);
		if(tmp == 0.0) return 1;

        for (int j=0;j<3;j++) {
            if (j>k) a.set(k,j, a.at(k, j)/tmp);
            inv.set(k,j, inv.at(k, j)/tmp);
        }
        for (int i=0;i<3;i++) {             // Loop over rows
            if (i == k) continue;
            tmp = a.at(i,k);
            for (int j=0;j<3;j++) {
                if (j>k) a.set(i, j, a.at(i, j) - a.at(k,j)*tmp);
                inv.set(i, j, inv.at(i, j) - inv.at(k,j)*tmp);
            }
        }
    }

    return 0;
}
