#ifndef	__GEN_H__
#define	__GEN_H__

#include <complex.h>

struct fentry {
	size_t f_narg;
	const char *f_name;
	const char *f_namel;

	/* Domain validator functions */
	union {
		int (*fp1)(long double x);
		int (*fp2)(long double x, long double y);
	} f_u;
	union {
		int (*fp1)(long double complex x);
		int (*fp2)(long double complex x, long double complex y);
	} f_uc;

	/* Computation functions */
	union {
		double (*f_ulibm_real)();
		double complex (*f_ulibm_complex)();
	} f_ulibm;

	union {
		long double (*f_ulibml_real)();
		long double complex (*f_ulibml_complex)();
	} f_ulibml;

	int (*f_mpfr)();
	int (*f_mpc)();
};

#define	f_libm_real	f_ulibm.f_ulibm_real
#define	f_libm_complex	f_ulibm.f_ulibm_complex
#define	f_libml_real	f_ulibml.f_ulibml_real
#define	f_libml_complex	f_ulibml.f_ulibml_complex

extern const int fsize;

const struct fentry *getfunctionbyidx(int idx);
const struct fentry *getfunctionbyname(const char *fname);
void printfunctions(void);

#ifdef	DEBUG
#define	DPRINTF(x)	printf x; fflush(stdout);
#else
#define	DPRINTF(x)
#endif

#endif	/* !__GEN_H__ */
