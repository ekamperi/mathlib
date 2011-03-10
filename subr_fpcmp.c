#include <complex.h>
#include <math.h>
#include "mytypes.h"
#include "subr_fpcmp.h"

/*******************************************************************************
 *			Real arithmetic
 ******************************************************************************/
#define FPCMP_COMMON(x, y)				\
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
fpreal_equalf(float x, float y)
{
	FPCMP_COMMON(x, y);

	return ((fabsf(x - y) < (1E-6 * fmaxf(fabsf(x), fabsf(y)))));
}

int
fpreal_equal(double x, double y)
{
	FPCMP_COMMON(x, y);

	return ((fabs(x - y) < (1E-8 * fmax(fabs(x), fabs(y)))));
}

int
fpreal_equall(long double x, long double y)
{
	FPCMP_COMMON(x, y);

	return ((fabsl(x - y) < (1E-10 * fmaxl(fabsl(x), fabsl(y)))));
}

/*******************************************************************************
 *				Complex arithmetic
 ******************************************************************************/
int
fpcomplex_equalf(float complex x, float complex y)
{
	float_complex fcx = { .z = x };
	float_complex fcy = { .z = y };
	int real, imag;

	real = fpreal_equalf(fcx.parts[0], fcy.parts[0]);
	imag = fpreal_equalf(fcx.parts[1], fcy.parts[1]);

	return (real && imag);
}

int
fpcomplex_equal(double complex x, double complex y)
{
	double_complex dcx = { .z = x };
	double_complex dcy = { .z = y };
	int real, imag;

	real = fpreal_equal(dcx.parts[0], dcy.parts[0]);
	imag = fpreal_equal(dcx.parts[1], dcy.parts[1]);

	return (real && imag);
}

int
fpcomplex_equall(long double complex x, long double complex y)
{
	long_double_complex ldcx = { .z = x };
	long_double_complex ldcy = { .z = y };
	int real, imag;

	real = fpreal_equall(ldcx.parts[0], ldcy.parts[0]);
	imag = fpreal_equall(ldcx.parts[1], ldcy.parts[1]);

	return (real && imag);
}
