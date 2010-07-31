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
#include "subr_mpc.h"
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
calculp_long_double_complex(long double computedl, long double exactl)
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
    long double complex *xl, long double complex *yl,
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
getfunctionulp_complex(const struct fentry *f, struct ulp_complex *uc)
{
	const mpc_rnd_t tonearest = MPC_RNDNN;
	mpc_t mp_exactl, mp_xl, mp_yl;
	mpc_t mp_exact, mp_x, mp_y;
	long double complex xl, yl, computedl, exactl, myulpl;
	double complex x, y, computed, exact, myulp;
	size_t i;

	/* Initialize high precision variables */
	mpc_init2(mp_exact, 200);
        mpc_init2(mp_x,     200);
        mpc_init2(mp_y,     200);
	mpc_init2(mp_exactl,200);
	mpc_init2(mp_xl,    200);
	mpc_init2(mp_yl,    200);

	ULP_COMPLEX_INIT(uc);

	for (i = 0; i < NITERATIONS; i++) {
		if (i % 100 == 0) {
			printf("Percentage complete: %2.2f\r",
			    (100.0 * i)/NITERATIONS);
			fflush(stdout);
		}

		/* Generate random arguments */
		populate_complex_vars(f, &x, &y, &xl, &yl, mp_x, mp_y, mp_xl, mp_yl);

		/*
		 * Ready to call the mpfr*()
		 * The double version ALWAYS exist!
		 */
		if(f->f_narg == 1) {
			f->f_mpc(mp_exact, mp_x, tonearest);
		} else {
			f->f_mpc(mp_exact, mp_x, mp_y, tonearest);
		}
		exact = mpc_get_dc(mp_exact,  tonearest);

		/* We can't tell the same for long double functions though */
		if (f->f_libml_complex) {
			if(f->f_narg == 1) {
				f->f_mpc(mp_exactl, mp_xl, tonearest);
			} else {
				f->f_mpc(mp_exactl, mp_xl, mp_yl, tonearest);
			}
			exactl = mpc_get_ldc(mp_exactl, tonearest);
		}

		/* Ready to call the libm*() */
		if (f->f_narg == 1) {
			computed = f->f_libm_complex(x);
		} else {
			computed = f->f_libm_complex(x, y);
		}

		if (f->f_libml_complex) {
			if (f->f_narg == 1) {
				computedl = f->f_libml_complex(xl);
			} else {
				computedl = f->f_libml_complex(xl, yl);
			}
		}

		/* Skip bogus results */
		if (fpclassify(computed) == FP_NORMAL &&
		    fpclassify(exact) == FP_NORMAL) {
			myulp = calculp_double_complex(computed, exact);
			ULP_COMPLEX_UPDATE(uc, myulp);
		} else {
			uc->ulp_real.ulp_skipped++;
			uc->ulp_imag.ulp_skipped++;
		}

		if (f->f_libml_complex) {
			if (fpclassify(computedl) == FP_NORMAL &&
			    fpclassify(exactl) == FP_NORMAL) {
				myulpl = calculp_long_double_complex(computedl, exactl);
				ULP_COMPLEX_UPDATEL(uc, myulpl);
			}
		} else {
			uc->ulp_real.ulp_skippedl++;
			uc->ulp_imag.ulp_skippedl++;
		}
	}

	uc->ulp_real.ulp_avg  /= (i - uc->ulp_real.ulp_skipped);
	uc->ulp_imag.ulp_avgl /= (i - uc->ulp_imag.ulp_skippedl);

	/* Free resources */
	mpc_clear(mp_exact);
	mpc_clear(mp_x);
	mpc_clear(mp_y);
	mpc_clear(mp_exactl);
	mpc_clear(mp_xl);
	mpc_clear(mp_yl);

	/* Success */
	return 0;
}

void
printulps_double_complex(struct ulp_complex uc)
{
	printf("%-10.4f %-10.4f %-10.4f   ",
	    uc.ulp_real.ulp_max, uc.ulp_real.ulp_min, uc.ulp_real.ulp_avg);
	printf("%5u\n", uc.ulp_real.ulp_skipped);
}

void
printulps_long_double_complex(struct ulp_complex uc)
{
	printf("%-10.4f %-10.4f %-10.4f   ",
	    (double)uc.ulp_real.ulp_maxl, (double)uc.ulp_real.ulp_minl, (double)uc.ulp_real.ulp_avgl);
	printf("%5u\n", uc.ulp_real.ulp_skippedl);
}