#ifndef	__SUBR_FPCMP_H__
#define	__SUBR_FPCMP_H__

#include "math.h"

#define	FPCMP_EQUAL(x, y)	\
	(fabs((x) - (y)) < (1E-5 * fmax(fabs((x)), fabs((y)))))

#endif	/* !__SUBR_FPCMP_H__ */
