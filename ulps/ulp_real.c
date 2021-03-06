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

double
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

long double
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



static void
populate_vars(const struct fentry *f,
    double *x, double *y, long double *xl, long double *yl,
    mpfr_ptr mp_x, mpfr_ptr mp_y, mpfr_ptr mp_xl, mpfr_ptr mp_yl)
{
	const mpfr_rnd_t tonearest = GMP_RNDN;
	long double txl, tyl;
	double tx, ty;

	assert(f && x && y && xl && yl);
	txl = tx = 0.0;
	tyl = ty = 0.0;

	/* Generate random arguments */
	if (f->f_narg == 1) {
		do {
			tx  = random_double(FP_NORMAL);
			txl = random_long_double(FP_NORMAL);
		} while (!f->f_u.fp1(tx) || !f->f_u.fp1(txl));
	}
	if (f->f_narg == 2) {
		do {
			tx  = random_double(FP_NORMAL);
			ty  = random_double(FP_NORMAL);
			txl = random_long_double(FP_NORMAL);
			tyl = random_long_double(FP_NORMAL);
		} while (!f->f_u.fp2(tx, ty) || !f->f_u.fp2(txl, tyl));
	}

	/* Hack, yikes */
	if (!strcmp(f->f_name, "yn")) {
		txl = tx = 1.0;
	}

	*x  = tx;
	*y  = ty;
	*xl = txl;
	*yl = tyl;

	/* Copy arguments to mpfr variables */
	mpfr_set_d (mp_x,  tx,  tonearest);
	mpfr_set_d (mp_y,  ty,  tonearest);
	mpfr_set_ld(mp_xl, txl, tonearest);
	mpfr_set_ld(mp_yl, tyl, tonearest);
}

void
getfunctionulp_real(const struct fentry *f, struct ulp *u)
{
	const mpfr_rnd_t tonearest = GMP_RNDN;
	mpfr_t mp_exactl, mp_xl, mp_yl;
	mpfr_t mp_exact, mp_x, mp_y;
	long double xl, yl, computedl, exactl, myulpl;
	double x, y, computed, exact, myulp;
	size_t i;

	/* Initialize high precision variables */
	mpfr_inits2(200, mp_exact,  mp_x,  mp_y,  (mpfr_ptr)NULL);
	mpfr_inits2(200, mp_exactl, mp_xl, mp_yl, (mpfr_ptr)NULL);

	ULP_INIT(u);

	for (i = 0; i < NITERATIONS; i++) {
		if (i % 100 == 0) {
			printf("Percentage complete: %2.2f\r",
			    (100.0 * i)/NITERATIONS);
			fflush(stdout);
		}

		/* Generate random arguments */
		populate_vars(f, &x, &y, &xl, &yl, mp_x, mp_y, mp_xl, mp_yl);

		/*
		 * Ready to call the mpfr*()
		 * The double version ALWAYS exist!
		 */
		if(f->f_narg == 1) {
			f->f_mpfr(mp_exact, mp_x, tonearest);
		} else {
			/* Hack, yikes */
			if (!strcmp(f->f_name, "yn"))
				f->f_mpfr(mp_exact, (long)x, mp_y, tonearest);
			else
				f->f_mpfr(mp_exact, mp_x, mp_y, tonearest);
		}
		exact = mpfr_get_d(mp_exact,  tonearest);

		/* We can't tell the same for long double functions though */
		if (f->f_libml_real) {
			if(f->f_narg == 1) {
				f->f_mpfr(mp_exactl, mp_xl, tonearest);
			} else {
				f->f_mpfr(mp_exactl, mp_xl, mp_yl, tonearest);
			}
			exactl = mpfr_get_ld(mp_exactl, tonearest);
		}

		/* Ready to call the libm*() */
		if (f->f_narg == 1) {
			computed = f->f_libm_real(x);
		} else {
			computed = f->f_libm_real(x, y);
		}

		if (f->f_libml_real) {
			if (f->f_narg == 1) {
				computedl = f->f_libml_real(xl);
			} else {
				computedl = f->f_libml_real(xl, yl);
			}
		}

		/* Skip bogus results */
		if (fpclassify(computed) == FP_NORMAL &&
		    fpclassify(exact) == FP_NORMAL) {
			myulp = calculp_double(computed, exact);
			ULP_UPDATE(u, myulp);
		} else {
			u->ulp_skipped++;
		}

		if (f->f_libml_real) {
			if (fpclassify(computedl) == FP_NORMAL &&
			    fpclassify(exactl) == FP_NORMAL) {
				myulpl = calculp_long_double(computedl, exactl);
				ULP_UPDATEL(u, myulpl);
			}
		} else {
			u->ulp_skippedl++;
		}
	}

	u->ulp_avg  /= (i - u->ulp_skipped);
	u->ulp_avgl /= (i - u->ulp_skippedl);

	/* Free resources */
	mpfr_clears(mp_exact,  mp_x,  mp_y,  (mpfr_ptr)NULL);
	mpfr_clears(mp_exactl, mp_xl, mp_yl, (mpfr_ptr)NULL);
}

void
printulps_double(struct ulp u)
{
	if (u.ulp_max > 9.9 || u.ulp_min > 9.9) {
		printf("%-10.4e %-10.e %-10.4e   ",
		    u.ulp_max, u.ulp_min, u.ulp_avg);
	} else {
		printf("%-10.4f %-10.4f %-10.4f   ",
		    u.ulp_max, u.ulp_min, u.ulp_avg);
	}
	printf("%5u\n", u.ulp_skipped);
}

void
printulps_long_double(struct ulp u)
{
	if (u.ulp_maxl > 9.9 || u.ulp_minl > 9.9) {
		printf("%-10.4e %-10.4e %-10.4e   ",
		    (double)u.ulp_maxl,
		    (double)u.ulp_minl,
		    (double)u.ulp_avgl);
	} else {
		printf("%-10.4f %-10.4f %-10.4f   ",
		    (double)u.ulp_maxl,
		    (double)u.ulp_minl,
		    (double)u.ulp_avgl);
	}
	printf("%5u\n", u.ulp_skippedl);
}
