#ifndef	__BASIC_COMPLEX_H__
#define	__BASIC_COMPLEX_H__

#include <complex.h>

double complex cadd(double complex z, double complex w);
double complex csub(double complex z, double complex w);
double complex cmul(double complex z, double complex w);
double complex cdiv(double complex z, double complex w);
double complex cneg(double complex z);

long double complex caddl(long double complex z, long double complex w);
long double complex csubl(long double complex z, long double complex w);
long double complex cmull(long double complex z, long double complex w);
long double complex cdivl(long double complex z, long double complex w);
long double complex cnegl(long double complex z);

#endif	/* __BASIC_COMPLEX_H__ */
