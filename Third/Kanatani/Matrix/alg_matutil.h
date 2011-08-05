/***********************************************************
	alg_matutil.h -- 行列 (ヘッダファイル)
***********************************************************/
#ifndef MATUTIL
#define MATUTIL

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifndef NULL
#define NULL 0
#endif

#define SCALAR double
typedef SCALAR *vector, **matrix;

void   alg_error(char *message);
vector alg_newvec(int n);
vector alg_new_vector(int n);
void   alg_free_vector(vector v);
double alg_innerproduct(int n, vector u, vector v);

#endif  /* MATUTIL */
