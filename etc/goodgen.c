#define _XOPEN_SOURCE 600

#include <float.h>
#include <math.h>
#include <stdio.h>

#include <gmp.h>
#include <mpfr.h>

#include "../subr_random.h"

int
main(void)
{
	mpfr_t mp_x, mp_y;
	long double x, y, exact;
	size_t i;

	/* Initialize random number generator */
	init_randgen();

	/* Initialize high precision variables */
	mpfr_init2(mp_x, 500);
	mpfr_init2(mp_y, 500);

	for (i = 0; i < 10; i++) {
		do {
			x = random_long_double(FP_NORMAL);
		} while (x < 100.0 || x > 10000.0);

		mpfr_set_ld(mp_x,   x, GMP_RNDN);
		mpfr_set_ld(mp_y, 0.0, GMP_RNDN);

		mpfr_exp(mp_y, mp_x, GMP_RNDN);
		exact = mpfr_get_d(mp_y, GMP_RNDN);
		y = expl(x);

		printf("x      = %.32Le\n", x);
		printf("exp(x) = %.32Le\n", y);
		printf("exact  = %.32Le\n\n", exact);
	}

	/* Free resources */
	mpfr_clear(mp_x);
	mpfr_clear(mp_y);
}
