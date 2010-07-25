#include <complex.h>
#include <math.h>
#include "subr_fpcmp.h"

/*******************************************************************************
 *			Real arithmetic
 ******************************************************************************/
#define __FPCMP_COMMON(x, y)				\
do {							\
	if (isnan(x) || isnan(y))			\
		return (isnan(x) && isnan(y));		\
							\
	if (isinf(x) || isinf(y))			\
		return ((isinf(x) && isinf(y))		\
		    && (signbit(x) == signbit(y)));	\
							\
	if (iszero(x) || iszero(y))			\
		return ((iszero(x) && iszero(y))	\
		    && (signbit(x) == signbit(y)));	\
} while(0)

int
fpcmp_equalf(float x, float y)
{
	__FPCMP_COMMON(x, y);

	return ((fabsf(x - y) < (1E-6 * fmaxf(fabsf(x), fabsf(y)))));
}

int
fpcmp_equal(double x, double y)
{
	__FPCMP_COMMON(x, y);

	return ((fabs(x - y) < (1E-8 * fmax(fabs(x), fabs(y)))));
}

int
fpcmp_equall(long double x, long double y)
{
	__FPCMP_COMMON(x, y);

	return ((fabsl(x - y) < (1E-10 * fmaxl(fabsl(x), fabsl(y)))));
}

/********************************************************************************
 *				Complex arithmetic
 ******************************************************************************/
/*
 * Quoting from ISO/IEC 9899:TC2:
 *
 * 6.2.5.13 Types
 * Each complex type has the same representation and alignment requirements as
 * an array type containing exactly two elements of the corresponding real type;
 * the first element is equal to the real part, and the second element to the
 * imaginary part, of the complex number.
 */
typedef union {
	float complex z;
	float parts[2];
} float_complex;

typedef union {
	double complex z;
	double parts[2];
} double_complex;

typedef union {
	long double complex z;
	long double parts[2];
} long_double_complex;

int
fpcomplex_equalf(float complex x, float complex y)
{
	float_complex fcx = { .z = x };
	float_complex fcy = { .z = y };
	int real, imag;

	real = fpcmp_equalf(fcx.parts[0], fcy.parts[0]);
	imag = fpcmp_equalf(fcx.parts[1], fcy.parts[1]);

	return (real && imag);
}

int
fpcomplex_equal(double complex x, double complex y)
{
	double_complex dcx = { .z = x };
	double_complex dcy = { .z = y };
	int real, imag;

	real = fpcmp_equal(dcx.parts[0], dcy.parts[0]);
	imag = fpcmp_equal(dcx.parts[1], dcy.parts[1]);

	return (real && imag);
}

int
fpcomplex_equall(long double complex x, long double complex y)
{
	long_double_complex ldcx = { .z = x };
	long_double_complex ldcy = { .z = y };
	int real, imag;

	real = fpcmp_equall(ldcx.parts[0], ldcy.parts[0]);
	imag = fpcmp_equall(ldcx.parts[1], ldcy.parts[1]);

	return (real && imag);
}
