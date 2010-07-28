#define _XOPEN_SOURCE 600

#include <assert.h>
#include <complex.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <gmp.h>
#include <mpc.h>
#include <mpfr.h>

#include "gen.h"
#include "subr_random.h"
#include "ulp.h"

static double
calculp_double_complex(double complex computed, double complex exact)
{
	double ulp_real;
	double ulp_imag;

	ulp_real = calculp_double(creal(computed), creal(exact));
	ulp_imag = calculp_double(cimag(computed), cimag(exact));

	return (fabs(ulp_real) + fabs(ulp_imag));
}

static long double
calculp_long_double(long double computedl, long double exactl)
{
        long double ulp_reall;
        long double ulp_imagl;

        ulp_reall = calculp_long_double(creall(computedl), creall(exactl));
        ulp_imagl = calculp_long_double(cimagl(computedl), cimagl(exactl));

        return (fabsl(ulp_reall) + fabsl(ulp_imagl));
}

static void
populate_complex_vars(const struct fentry *f,
    double complex *x, double complex *y,
    long double complex *xl, long double coplex *yl,
    mpc_t mp_x, mpc_t mp_y, mpc_t mp_xl, mpc_t mp_yl)
{
	const mpc_rnd_t tonearest = MPC_RNDNN;
	long double complex txl, tyl;
	double complex tx, ty;

	assert(f && x && y && xl && yl);
	txl = tx = 0.0;
	tyl = ty = 0.0;

	/* Generate random arguments */
	if (f->f_narg == 1) {
		do {
			tx  = random_double_complex(FP_NORMAL);
			txl = random_long_double_complex(FP_NORMAL);
		} while (!f->f_uc.fp1(tx) || !f->f_uc.fp1(txl));
	}
	if (f->f_narg == 2) {
		do {
			tx  = random_double_complex(FP_NORMAL);
			ty  = random_double_complex(FP_NORMAL);
			txl = random_long_double_complex(FP_NORMAL);
			tyl = random_long_double_complex(FP_NORMAL);
		} while (!f->f_uc.fp2(tx, ty) || !f->f_uc.fp2(txl, tyl));
	}

	*x  = tx;
	*y  = ty;
	*xl = txl;
	*yl = tyl;

	/* Copy arguments to mpc variables */
	mpc_set_d_d  (mp_x,  creal (tx),  cimag (tx),  tonearest);
	mpc_set_d_d  (mp_y,  creal (ty),  cimag (ty),  tonearest);
	mpc_set_ld_ld(mp_xl, creall(txl), cimagl(txl), tonearest);
	mpc_set_ld_ld(mp_yl, creall(tyl), cimagl(tyl), tonearest);
}

int
getfunctionulp(const char *fname, struct ulp *u)
{
	const struct fentry *f;
	const mpc_rnd_t tonearest = GMP_RNDNN;
	mpc_t mp_exactl, mp_xl, mp_yl;
	mpc_t mp_exact, mp_x, mp_y;
	long double xl, yl, computedl, exactl, myulpl;
	double x, y, computed, exact, myulp;
	size_t i;

	f = getfunctionbyname(fname);
	if (f == NULL)
		return (-1);

	/* Initialize high precision variables */
	mpc_init2(mp_exact, 200);
        mpc_init2(mp_x,     200);
        mpc_init2(mp_y,     200);
	mpc_init2(mp_exactl,200);
	mpc_init2(mp_xl,    200);
	mpc_init2(mp_yl,    200);

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
			ULP_UPDATE(u, myulp);
		} else {
			u->ulp_skipped++;
		}

		if (f->f_libml) {
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

	/* Success */
	return 0;
}

void
printulps_double_complex(struct ulp_complex u)
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
printulps_long_double_complex(struct ulp_complex u)
{
	if (u.ulp_maxl > 9.9 || u.ulp_minl > 9.9) {
		printf("%-10.4e %-10.4e %-10.4e   ",
		    (double)u.ulp_maxl, (double)u.ulp_minl, (double)u.ulp_avgl);
	} else {
		printf("%-10.4f %-10.4f %-10.4f   ",
		    (double)u.ulp_maxl, (double)u.ulp_minl, (double)u.ulp_avgl);
	}
	printf("%5u\n", u.ulp_skippedl);
}
