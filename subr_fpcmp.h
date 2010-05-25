#ifndef	__SUBR_FPCMP_H__
#define	__SUBR_FPCMP_H__

#include "math.h"

#define	FPCMP_EQUAL(x, y)	\
	(fabs((x) - (y)) < (1E-5 * fmax(fabs((x)), fabs((y)))))

#ifndef	iszero
#define	iszero(x)	(fpclassify((x) == FP_ZERO))
#endif

int
fpcmp_equal(double x, double y)
{
	if (isnan(x) || isnan(y))
		return (isnan(x) && isnan(y));

	if (isinf(x) || isinf(y))
		return ((isinf(x) && isinf(y))
		    && (signbit(x) == signbit(y)));

	if (iszero(x) || iszero(y))
		return ((iszero(x) && iszero(y))
		    && (signbit(x) == signbit(y)));

	return ((fabs(x) - y) < (1E-5 * fmax(fabs(x), fabs(y))));
}

#endif  /* !__SUBR_FPCMP_H__ */
