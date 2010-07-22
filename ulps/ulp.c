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

#define	NITERATIONS	(5*10000)

static double
calculp_double(double computed, double exact)
{
	double xbefore, xafter;
	double ulps;

	xbefore = nextafter(exact, -INFINITY);
	xafter = nextafter(exact, +INFINITY);
	assert(xafter != xbefore);

	ulps = fabs( (exact - computed) / (xafter - xbefore) );

	return (ulps);
}

static long double
calculp_long_double(long double computedl, long double exactl)
{
	long double xbeforel, xafterl;
	long double ulpsl;

	xbeforel = nextafterl(exactl, -INFINITY);
	xafterl = nextafterl(exactl, +INFINITY);
	assert(xafterl != xbeforel);

	ulpsl = fabsl( (exactl - computedl) / (xafterl - xbeforel) );

	return (ulpsl);
}

int
getfunctionulp(const char *fname, struct ulp *u)
{
	const struct fentry *f;
	const mpfr_rnd_t tonearest = GMP_RNDN;
	mpfr_t mp_exactl, mp_xl, mp_yl;
	mpfr_t mp_exact, mp_x, mp_y;
	long double xl, yl, computedl, exactl, myulpl;
	double x, y, computed, exact, myulp;
	size_t i;

	f = getfunctionbyname(fname);
	if (f == NULL)
		return (-1);

	/* Initialize high precision variables */
	mpfr_inits2(100, mp_exact,  mp_x,  mp_y,  (mpfr_ptr)NULL);
	mpfr_inits2(200, mp_exactl, mp_xl, mp_yl, (mpfr_ptr)NULL);

	x = xl = 0.0;
	y = yl = 0.0;

	ULP_INIT(u);

	for (i = 0; i < NITERATIONS; i++) {
		if (i % 100 == 0)
			printf("Percentage complete: %2.2f\r",
			    (100.0 * i)/NITERATIONS);

		assert(f->f_libml);
		/* Generate random arguments */
		if (f->f_narg == 1) {
			do {
				x  = random_double(FP_NORMAL);
				xl = random_long_double(FP_NORMAL);
			} while (!f->f_u.fp1(x));
		}
		if (f->f_narg == 2) {
			do {
				x  = random_double(FP_NORMAL);
				y  = random_double(FP_NORMAL);
				xl = random_long_double(FP_NORMAL);
				yl = random_long_double(FP_NORMAL);
			} while (!f->f_u.fp2(x, y));
		}

		/* Hack, yikes */
		if (!strcmp(f->f_name, "yn")) {
			x = 1.0;
		}

		/* Copy arguments to mpfr variables */
		mpfr_set_d (mp_x,  x,  tonearest);
		mpfr_set_ld(mp_xl, xl, tonearest);
		if (f->f_narg == 2){
			mpfr_set_d (mp_y, y,  tonearest);
			mpfr_set_ld(mp_y, yl, tonearest);
		}

		/*
		 * Ready to call the mpfr*()
		 * The double version ALWAYS exist!
		 */
		if(f->f_narg == 1) {
			f->f_mpfr(mp_exact, mp_x, tonearest);
		} else {
			f->f_mpfr(mp_exact, mp_x, mp_y, tonearest);
		}
		exact = mpfr_get_d(mp_exact,  tonearest);

		/* We can't tell the same for long double functions though */
		if (f->f_libml) {
			if(f->f_narg == 1) {
				f->f_mpfr(mp_exactl, mp_xl, tonearest);
			} else {
				f->f_mpfr(mp_exactl, mp_xl, mp_yl, tonearest);
			}
			exactl = mpfr_get_ld(mp_exactl, tonearest);
		}

		/* Ready to call the libm*() */
		if (f->f_narg == 1) {
			computed = f->f_libm(x);
		} else {
			computed = f->f_libm(x, y);
		}

                if (f->f_libml) {
			if (f->f_narg == 1) {
				computedl = f->f_libml(xl);
			} else {
				computedl = f->f_libml(xl, yl);
			}
		}

		/* Skip bogus results */
		if (fpclassify(computed) == FP_NORMAL &&
		    fpclassify(exact) == FP_NORMAL) {
			myulp = calculp_double(computed, exact);
			if (myulp > 1E100) {
				printf("----------->\nx=%.16e  y=%.16e\n"
				    "computed = %.16e  exact = %.16e  ULP = %.16e\n\n",
				    x, y, computed, exact, myulp);
			}
			ULP_UPDATE(u, myulp);
		} else {
			u->ulp_skipped++;
		}

                if (f->f_libml) {
			if (fpclassify(computedl) == FP_NORMAL &&
			    fpclassify(exactl) == FP_NORMAL) {
				myulpl = calculp_long_double(computedl, exactl);
				ULP_UPDATE(u, myulpl);
			}
		} else {
			u->ulp_skippedl++;
		}
	}

	u->ulp_avg  /= (i - u->ulp_skipped);
	u->ulp_avgl /= (i - u->ulp_skippedl);

	/* Free resources */
	mpfr_clears(mp_exact,  mp_x,  mp_y,  NULL);
	mpfr_clears(mp_exactl, mp_xl, mp_yl, NULL);

	/* Success */
	return 0;
}
