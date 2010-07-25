#ifndef	__SUBR_FPCMP_H__
#define	__SUBR_FPCMP_H__

#include <complex.h>

/*
 * Check whether argument is precisely zero.
 * Matches both signed and unsigned zeros.
 */
#undef	iszero
#define	iszero(x)	(fpclassify(x) == FP_ZERO)

int fpreal_equalf(float x, float y);
int fpreal_equal(double x, double y);
int fpreal_equall(long double x, long double y);

int fpcomplex_equalf(float complex x, float complex y);
int fpcomplex_equal(double complex x, double complex y);
int fpcomplex_equall(long double complex x, long double complex y);

#endif  /* !__SUBR_FPCMP_H__ */
