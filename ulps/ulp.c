#define _XOPEN_SOURCE 600

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

#define	NITERATIONS	(5*1000)

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
	const struct fentry *f;
	const mpfr_rnd_t tonearest = GMP_RNDN;
	mpfr_t mp_rop, mp_x, mp_y;
	double x, y, computed, exact, _u;
	size_t i;

	assert(u);

	f = getfunctionbyname(fname);
	if (f == NULL)
		return (-1);

	/* Initialize high precision variables */
	mpfr_inits2(100, mp_rop, mp_x, mp_y, NULL);

	x = 0.0;
	y = 0.0;

	u->ulp_max = -DBL_MAX;
	u->ulp_min = DBL_MAX;
	u->ulp_avg = 0.0;
	u->ulp_skipped = 0;

	for (i = 0; i < NITERATIONS; i++) {
		if (i %  100 == 0)
			printf("Percentage complete: %2.2f\r", (100.0 * i)/NITERATIONS);

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

		/* Ready to call the libm*() */
		if (f->f_narg == 1) {
			computed = f->f_libm(x);
		} else {
			computed = f->f_libm(x, y);
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
	mpfr_clears(mp_rop, mp_x, mp_y, NULL);

	/* Success */
	return 0;
}
