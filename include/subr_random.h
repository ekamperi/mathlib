#ifndef	__SUBR_RANDOM_H__
#define	__SUBR_RANDOM_H__

#include <inttypes.h>
#include <complex.h>

void init_randgen(void);

/* Real numbers */
float random_float(int class);
double random_double(int class);
long double random_long_double(int class);

float random_float_range_in(int class, float min, float max);
double random_double_range_in(int class, double min, double max);
long double random_long_double_range_in(int class,
					long double min,
					long double max);

float random_float_range_ex(int class, float min, float max);
double random_double_range_ex(int class, double min, double max);
long double random_long_double_range_ex(int class,
				     long double min,
				     long double max);

/* Complex numbers */
float complex random_float_complex(int class);
double complex random_double_complex(int class);
long double complex random_long_double_complex(int class);

/* We export these functions, only to be used by self tests */
int isvalidfp_vax(const uint32_t *y);
int isvalidfp_intel80bit(const uint32_t *y);

#endif	/* ! __SUBR_RANDOM_H__ */
