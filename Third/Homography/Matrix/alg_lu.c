/***********************************************************
	alg_lu.c -- LU分解
***********************************************************/
#include <stdlib.h>
#include <math.h>
#include "alg_matutil.h"  /* 行列操作の小道具集 */
double alg_lu(int n, matrix a, int *ip)
{
	int i, j, k, ii, ik;
	double t, u, det;
	vector weight;
	weight = alg_new_vector(n);    /* weight[0..n-1] の記憶領域確保 */
	det = 0;                       /* 行列式 */
	for (k = 0; k < n; k++) {      /* 各行について */
		ip[k] = k;             /* 行交換情報の初期値 */
		u = 0;                 /* その行の絶対値最大の要素を求める */
		for (j = 0; j < n; j++) {
			t = fabs(a[k][j]);  if (t > u) u = t;
		}
		if (u == 0) goto EXIT; /* 0 なら行列はLU分解できない */
		weight[k] = 1 / u;     /* 最大絶対値の逆数 */
	}
	det = 1;                       /* 行列式の初期値 */
	for (k = 0; k < n; k++) {      /* 各行について */
		u = -1;
		for (i = k; i < n; i++) {  /* より下の各行について */
			ii = ip[i]; /* 重み×絶対値 が最大の行を見つける */
			t = fabs(a[ii][k]) * weight[ii];
			if (t > u) {  u = t;  j = i;  }
		}
		ik = ip[j];
		if (j != k) {
			ip[j] = ip[k];  ip[k] = ik;  /* 行番号を交換 */
			det = -det;  /* 行を交換すれば行列式の符号が変わる */
		}
		u = a[ik][k];  det *= u;       /* 対角成分 */
		if (u == 0) goto EXIT;         /* 0 なら行列はLU分解できない */
		for (i = k + 1; i < n; i++) {  /* Gauss消去法 */
			ii = ip[i];
			t = (a[ii][k] /= u);
			for (j = k + 1; j < n; j++)
				a[ii][j] -= t * a[ik][j];
		}
	}
EXIT:
	alg_free_vector(weight);   /* 記憶領域を解放 */
	return det;                /* 戻り値は行列式 */
}
void alg_solve(int n, matrix a, vector b, int *ip, vector x)
{
	int i, j, ii;
	double t;
	for (i = 0; i < n; i++) {       /* Gauss消去法の残り */
		ii = ip[i];  t = b[ii];
		for (j = 0; j < i; j++) t -= a[ii][j] * x[j];
		x[i] = t;
	}
	for (i = n - 1; i >= 0; i--) {  /* 後退代入 */
		t = x[i];  ii = ip[i];
		for (j = i + 1; j < n; j++) t -= a[ii][j] * x[j];
		x[i] = t / a[ii][i];
	}
}
double alg_matinv(int n, matrix a, matrix a_inv)
{
	int i, j, k, ii;
	double t, det;
	int *ip;   /* 行交換の情報 */
	ip = (int *)malloc(sizeof(int) * n);
	if (ip == NULL) alg_error("記憶領域不足");
	det = alg_lu(n, a, ip);
	if (det != 0)
		for (k = 0; k < n; k++) {
			for (i = 0; i < n; i++) {
				ii = ip[i];  t = (ii == k);
				for (j = 0; j < i; j++)
					t -= a[ii][j] * a_inv[j][k];
				a_inv[i][k] = t;
			}
			for (i = n - 1; i >= 0; i--) {
				t = a_inv[i][k];  ii = ip[i];
				for (j = i + 1; j < n; j++)
					t -= a[ii][j] * a_inv[j][k];
				a_inv[i][k] = t / a[ii][i];
			}
		}
	free(ip);
	return det;
}
double alg_gauss(int n, matrix a, vector b, vector x)
{
	double det;  /* 行列式 */
	int *ip;     /* 行交換の情報 */
	ip = (int *)malloc(sizeof(int) * n);      /* 記憶領域確保 */
	if (ip == NULL) alg_error("記憶領域不足");
	det = alg_lu(n, a, ip);                   /* LU分解 */
	if (det != 0) alg_solve(n, a, b, ip, x);  /* 連立方程式を解く */
	free(ip);                                 /* 記憶領域の解放 */
	return det;                               /* 戻り値は行列式 */
}
double alg_det(int n, matrix a)
{
	double det;  /* 行列式 */
	int *ip;     /* 行交換の情報 */
	ip = (int *)malloc(sizeof(int) * n);       /* 記憶領域確保 */
	if (ip == NULL) alg_error("記憶領域不足");
	det = alg_lu(n, a, ip);                    /* LU分解 */
	free(ip);                                  /* 記憶領域の解放 */
	return det;                                /* 行列式 */
}
