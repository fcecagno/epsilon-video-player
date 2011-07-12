CC     = g++
CFLAGS = -O
LIBDIR = ../Matrix

mhomoest : mhomoest.c homoest.c $(LIBDIR)/libmatrix.a
	$(CC) $(CFLAGS) -I$(LIBDIR) mhomoest.c homoest.c \
	      -lm -L$(LIBDIR) -lmatrix -o $@
