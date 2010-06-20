#define _XOPEN_SOURCE 600

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <gmp.h>
#include <mpfr.h>
#include "gen.h"

#include "subr_random.h"

#define NITERATIONS	100000

static double
ulp(double computed, double exact)
{
	double xbefore, xafter;
	double ulps;

	xbefore = nextafter(exact, -INFINITY);
	xafter = nextafter(exact, +INFINITY);
	assert(xafter != xbefore);

	ulps = fabs( (exact - computed) / (xafter - xbefore) );

	return (ulps);
}

static void
calculp(const char *fname)
{
	mpfr_t mp_rop, mp_x, mp_y;
	double x, y, computed, exact;
	double maxulp, minulp, avgulp, u;
	size_t i, cnt = 0;
	const struct fentry *f;

	f = getfuncbyname(fname);
	assert(f);
	assert(f->f_narg == 1 || f->f_narg == 2);

	/* Initialize high precision variables */
	mpfr_init2(mp_rop, 100);
	mpfr_init2(mp_x, 100);
	mpfr_init2(mp_y, 100);

	x = 0.0;
	y = 0.0;

	maxulp = -DBL_MAX;
	minulp =  DBL_MAX;
	avgulp = 0.0;
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

		/* Update statistics */
		if (fpclassify(computed) != FP_NORMAL
		    || fpclassify(exact) != FP_NORMAL) {
			cnt++;
			continue;
		}

		u = ulp(computed, exact);

		if (u > maxulp)
			maxulp = u;
		if (u < minulp)
			minulp = u;
		avgulp += u;
	}
	avgulp /= (i - cnt);

	printf("function: %-8s max ulp: %.4f  min ulp: %.4f  avg ulp: %.4f\n",
	    f->f_name, maxulp, minulp, avgulp);

	/* Free resources */
	mpfr_clear(mp_rop);
	mpfr_clear(mp_x);
	mpfr_clear(mp_y);
}

int
main (void)
{
	srand48(time(NULL));

	calculp("acos");
	calculp("acosh");
	calculp("asin");
	calculp("asinh");
	calculp("atan");
	calculp("atan2");
	calculp("sin");
	calculp("cos");
	calculp("cosh");
	calculp("exp");
	calculp("exp2");
	calculp("erfc");
	calculp("hypot");
	calculp("log");
	calculp("log1p");
	calculp("log10");
	calculp("log2");
	calculp("rint");
	calculp("sinh");
	calculp("sqrt");
	/* pow() */
	calculp("y0");
	calculp("y1");
	calculp("yn");

	return 0;
}
