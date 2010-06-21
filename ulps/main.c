#define _XOPEN_SOURCE 600

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <gmp.h>
#include <mpfr.h>
#include "gen.h"

#include "subr_random.h"

#define NITERATIONS	100000

struct ulp {
	double ulp_max;
	double ulp_min;
	double ulp_avg;
	size_t ulp_skipped;
};

static double
calculp(double computed, double exact)
{
	double xbefore, xafter;
	double ulps;

	xbefore = nextafter(exact, -INFINITY);
	xafter = nextafter(exact, +INFINITY);
	assert(xafter != xbefore);

	ulps = fabs( (exact - computed) / (xafter - xbefore) );

	return (ulps);
}

static int
getfunctionulp(const char *fname, struct ulp *u)
{
	mpfr_t mp_rop, mp_x, mp_y;
	double x, y, computed, exact, _u;
	size_t i;
	const struct fentry *f;

	f = getfunctionbyname(fname);
	if (f == NULL)
		return -1;

	/* Some sanity checks */
	assert(f->f_narg == 1 || f->f_narg == 2);
	assert(u);

	/* Initialize high precision variables */
	mpfr_init2(mp_rop, 100);
	mpfr_init2(mp_x, 100);
	mpfr_init2(mp_y, 100);

	x = 0.0;
	y = 0.0;

	u->ulp_max = -DBL_MAX;
	u->ulp_min = DBL_MAX;
	u->ulp_avg = 0.0;
	u->ulp_skipped = 0;

	for (i = 0; i < NITERATIONS; i++) {
		/* Generate random arguments */
		do {
			x = random_double(FP_NORMAL);
		} while (!f->f_u.fp1(x));
		if (f->f_narg == 2) {
			do {
				y = random_double(FP_NORMAL);
			} while (!f->f_u.fp2(x, y));
		}

		/* Copy arguments to mpfr variables */
		mpfr_set_d(mp_x, x, GMP_RNDD);
		if (f->f_narg == 2){
			mpfr_set_d(mp_y, y, GMP_RNDD);
		}

		/* Ready to call the mpfr*() */
		if(f->f_narg == 1) {
			f->f_mpfr(mp_rop, mp_x, GMP_RNDD);
		} else {
			f->f_mpfr(mp_rop, mp_x, mp_y, GMP_RNDD);
		}
		exact = mpfr_get_d(mp_rop, GMP_RNDD);
		if (fpclassify(exact) == FP_NAN) {
			printf("%s\n", fname);
			if (f->f_narg == 1)
				printf("%f\n\n", x);
			else
				printf("%f %f\n\n", x, y);
		}

		/* Ready to call the libm*() */
		if (f->f_narg == 1) {
			computed = f->f_libm(x);
		} else {
			computed = f->f_libm(x, y);
		}

		if (fpclassify(computed) == FP_NAN) {
			printf("%s\n", fname);
			if (f->f_narg == 1)
				printf("%f\n\n", x);
			else
				printf("%f %f\n\n", x, y);
		}

		/* Skip bogus results */
		if (fpclassify(computed) != FP_NORMAL ||
		    fpclassify(exact) != FP_NORMAL) {
			u->ulp_skipped++;
			continue;
		}

		_u = calculp(computed, exact);

		if (_u > u->ulp_max)
			u->ulp_max = _u;
		if (_u < u->ulp_min)
			u->ulp_min = _u;
		u->ulp_avg += _u;
	}
	u->ulp_avg /= (i - u->ulp_skipped);

	/* Free resources */
	mpfr_clear(mp_rop);
	mpfr_clear(mp_x);
	mpfr_clear(mp_y);

	/* Success */
	return 0;
}

int
main(int argc, char *argv[])
{
	struct ulp u;
	int i, rv;

	/* Skip program name */
	argv++;
	argc--;

	/* Initialize random number generator */
	init_randgen();

	for (i = 0; i < argc; i++) {
		rv = getfunctionulp(argv[i], &u);
		if (rv != -1) {
			printf("function: %-8s "
			    "max ulp: %.4f  min ulp: %.4f  avg ulp: %.4f  "
			    "[skipped = %5u]\n",
			    argv[i], u.ulp_max, u.ulp_min, u.ulp_avg,
			    u.ulp_skipped);
		} else {
			fprintf(stderr, "function: %s not found\n", argv[i]);
			continue;
		}
	}

	return EXIT_SUCCESS;
}
