AUTOMAKE_OPTIONS = gnu
LDADD = -lm $(MPFR_LIBS)
AM_CFLAGS = -std=c99 -Wall -W -ansi -I../include $(MPFR_CFLAGS)
bin_PROGRAMS = ulp
ulp_SOURCES = main.c gen.c ulp.c ../subr_random.c

noinst_HEADERS = gen.h ulp.h
