CC  = g++
FRG = -O

libmatrix.a: matrix_base.h matrix_base.c \
             util_gen.h util_gen.c util_kk.h util_kk.c util_io.h util_io.c \
             util_num.h util_num.c tensor3333.h tensor3333.c \
	     alg_matutil.h alg_matutil.c alg_eigen.c alg_lu.c
	$(CC) $(FRG) -c matrix_base.c
	$(CC) $(FRG) -c util_gen.c
	$(CC) $(FRG) -c util_kk.c
	$(CC) $(FRG) -c util_io.c
	$(CC) $(FRG) -c util_num.c
	$(CC) $(FRG) -c alg_matutil.c
	$(CC) $(FRG) -c alg_eigen.c
	$(CC) $(FRG) -c alg_lu.c
	$(CC) $(FRG) -c tensor3333.c
	rm -f libmatrix.a
	ar rc libmatrix.a \
	      matrix_base.o util_gen.o util_kk.o util_io.o util_num.o \
	      alg_matutil.o alg_eigen.o alg_lu.o tensor3333.o
	ranlib libmatrix.a
	rm    matrix_base.o util_gen.o util_kk.o util_io.o util_num.o \
	      alg_matutil.o alg_eigen.o alg_lu.o tensor3333.o
