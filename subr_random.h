#ifndef	__SUBR_RANDOM_H__
#define	__SUBR_RANDOM_H__

#include <inttypes.h>

float random_float(int class);
double random_double(int class);
long double random_long_double(int class);

int
isvalidfp(const uint32_t *y);

#endif	/* ! __SUBR_RANDOM_H__ */
