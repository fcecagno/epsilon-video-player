/***********************************************************
	alg_matutil.c -- π‘ŒÛ
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "alg_matutil.h"

void alg_error(char *message)
{
	fprintf(stderr, "\n%s\n", message);  exit(EXIT_FAILURE);
}
vector alg_newvec(int n)
{
	return (SCALAR *)malloc(sizeof(SCALAR) * n);
}
vector alg_new_vector(int n)
{
	vector v;
	v = alg_newvec(n);
	if (v == NULL) alg_error("µ≠≤±ŒŒ∞Ë…‘¬≠.");
	return v;
}
void alg_free_vector(vector v)
{
	free(v);
}
double alg_innerproduct(int n, vector u, vector v)
{
	int i, n5;
	double s;
	s = 0;  n5 = n % 5;
	for (i = 0; i < n5; i++) s += u[i]*v[i];
	for (i = n5; i < n; i += 5)
		s += u[i]*v[i] + u[i+1]*v[i+1] + u[i+2]*v[i+2]
		               + u[i+3]*v[i+3] + u[i+4]*v[i+4];
	return s;
}
