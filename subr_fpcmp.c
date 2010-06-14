#include <math.h>
#include "subr_fpcmp.h"

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
		return ((iszero(x) && iszero(y)));	\
} while(0)

int
fpcmp_equalf(float x, float y)
{
	__FPCMP_COMMON(x, y);

	return ((fabsf(x - y) < (1E-5 * fmaxf(fabsf(x), fabsf(y)))));
}

int
fpcmp_equal(double x, double y)
{
	__FPCMP_COMMON(x, y);

	return ((fabs(x - y) < (1E-10 * fmax(fabs(x), fabs(y)))));
}

int
fpcmp_equall(long double x, long double y)
{
	__FPCMP_COMMON(x, y);

	return ((fabsl(x - y) < (1E-20 * fmaxl(fabsl(x), fabsl(y)))));
}
