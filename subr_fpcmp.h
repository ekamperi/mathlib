#ifndef	__SUBR_FPCMP_H__
#define	__SUBR_FPCMP_H__

/*
 * Check whether argument is precisely zero.
 * Matches both signed and unsigned zeros.
 */
#undef	iszero
#define	iszero(x)	(fpclassify(x) == FP_ZERO)

int fpcmp_equalf(float x, float y);
int fpcmp_equal(double x, double y);
int fpcmp_equall(long double x, long double y);

#endif  /* !__SUBR_FPCMP_H__ */
