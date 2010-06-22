#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <gmp.h>
#include <mpfr.h>

#include "gen.h"
#include "subr_random.h"
#include "ulp.h"

#define	NITERATIONS	50

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

int
getfunctionulp(const char *fname, struct ulp *u)
{
	mpfr_t mp_rop, mp_x, mp_y;
	double x, y, computed, exact, _u;
	size_t i;
	const mpfr_rnd_t tonearest = GMP_RNDN;
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
		if (f->f_narg == 1) {
			do {
				x = random_double(FP_NORMAL);
			} while (!f->f_u.fp1(x));
		}
		if (f->f_narg == 2) {
			do {
				x = random_double(FP_NORMAL);
				y = random_double(FP_NORMAL);
			} while (!f->f_u.fp2(x, y));
		}

		/* Hack, yikes */
		if (!strcmp(f->f_name, "yn")) {
			x = 1.0;
		}

		/* Copy arguments to mpfr variables */
		mpfr_set_d(mp_x, x, tonearest);
		if (f->f_narg == 2){
			mpfr_set_d(mp_y, y, tonearest);
		}

		/* Ready to call the mpfr*() */
		if(f->f_narg == 1) {
			f->f_mpfr(mp_rop, mp_x, tonearest);
		} else {
			f->f_mpfr(mp_rop, mp_x, mp_y, tonearest);
		}
		exact = mpfr_get_d(mp_rop, tonearest);
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

