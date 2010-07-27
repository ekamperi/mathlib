#ifndef	__GEN_H__
#define	__GEN_H__

#include <complex.h>

typedef enum { REAL, COMPLEX } domain_t;

struct fentry {
	domain_t f_domain;
	size_t f_narg;
	const char *f_name;
	const char *f_namel;
	union {
		int (*fp1)(long double x);
		int (*fp2)(long double x, long double y);
	} f_u;
	union {
		int (*fp1)(long double complex x);
		int (*fp2)(long double complex x, long double complex y);
	} f_uc;
	double (*f_libm)();
	long double (*f_libml)();
	int (*f_mpfr)();
	int (*f_mpc)();
};

extern const int fsize;

const struct fentry *getfunctionbyidx(size_t idx);
const struct fentry *getfunctionbyname(const char *fname);
void printfunctions(void);

#endif	/* !__GEN_H__ */
