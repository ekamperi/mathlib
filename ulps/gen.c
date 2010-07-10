#define _XOPEN_SOURCE 600

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <gmp.h>
#include <mpfr.h>

#include "config.h"
#include "gen.h"

static int
dom_acos(double x)
{
	return (x >= -1.0 && x <= 1.0);
}

static int
dom_acosh(double x)
{
	return (x >= 1.0);
}

static int
dom_asin(double x)
{
	return (x >= -1.0 && x <= 1.0);
}

static int
dom_asinh(double x)
{
	return 1;
}

static int
dom_atan(double x)
{
	return 1;
}

static int
dom_atanh(double x)
{
	return (x > -1.0 && x < 1.0);
}

static int
dom_cbrt(double x)
{
	return 1;
}

static int
dom_ceil(double x)
{
	return 1;
}

static int
dom_cosh(double x)
{
	return 1;
}

static int
dom_erf(double x)
{
	return 1;
}

static int
dom_erfc(double x)
{
	return 1;
}

static int
dom_floor(double x)
{
	return 1;
}

static int
dom_hypot(double x, double y)
{
	return 1;
}

static int
dom_sin(double x)
{
	return 1;
}

static int
dom_cos(double x)
{
	return 1;
}

static int
dom_exp(double x)
{
	return 1;
}

static int
dom_expm1(double x)
{
	return 1;
}

static int
dom_exp2(double x)
{
	return 1;
}

static int
dom_fabs(double x)
{
	return 1;
}

static int
dom_log(double x)
{
	return (x > 0.0);
}

static int
dom_log1p(double x)
{
	return (x > -1.0);
}

static int
dom_log10(double x)
{
	return (x > 0.0);
}

static int
dom_log2(double x)
{
	return (x > 0.0);
}

static int
dom_remainder(double x, double y)
{
	return (fpclassify(y) != FP_ZERO);
}

static int
dom_rint(double x)
{
	return 1;
}

static int
dom_sinh(double x)
{
	return 1;
}

static int
dom_sqrt(double x)
{
	return (x >= 0.0);
}

static int
dom_tan(double x)
{
	return (fabs(x - M_PI_2) > 1E-10);
}

static int
dom_tanh(double x)
{
	return 1;
}

static int
dom_pow(double x, double y)
{
	return (x >= 0.0 || (floor(y) == y));
}

static int
dom_tgamma(double x)
{
	return (x > 0.0 || (floor(x) != x));
}

static int
dom_y0(double x)
{
	return (x > 0.0);
}

static int
dom_y1(double x)
{
	return (x > 1.0 && x < 1E5);
}

#if 0
static int
dom_yn(double x, double y)
{
	return (y > 0.0 && y < 1E10);
}
#endif

const struct fentry
ftable[] = {
	/* acos() */
#ifdef	HAVE_ACOS
	{
		.f_name = "acos",
		.f_narg = 1,
		.f_libm = acos,
		.f_mpfr = mpfr_acos,
		.f_u.fp1 = dom_acos
	},
#endif

	/* acosh() */
#ifdef	HAVE_ACOSH
	{
		.f_name = "acosh",
		.f_narg = 1,
		.f_libm = acosh,
		.f_mpfr = mpfr_acosh,
		.f_u.fp1 = dom_acosh
	},
#endif

	/* asin() */
#ifdef	HAVE_ASIN
	{
		.f_name = "asin",
		.f_narg = 1,
		.f_libm = asin,
		.f_mpfr = mpfr_asin,
		.f_u.fp1 = dom_asin
	},
#endif

	/* asinh() */
#ifdef	HAVE_ASINH
	{
		.f_name = "asinh",
		.f_narg = 1,
		.f_libm = asinh,
		.f_mpfr = mpfr_asinh,
		.f_u.fp1 = dom_asinh
	},
#endif

	/* atan() */
#ifdef	HAVE_ATAN
	{
		.f_name = "atan",
		.f_narg = 1,
		.f_libm = atan,
		.f_mpfr = mpfr_atan,
		.f_u.fp1 = dom_atan
	},
#endif

	/* atan2() */
#ifdef	HAVE_ATAN2
	{
		.f_name = "atan2",
		.f_narg = 2,
		.f_libm = atan2,
		.f_mpfr = mpfr_atan2,
		.f_u.fp1 = dom_atan
	},
#endif

	/* atanh() */
#ifdef	HAVE_ATANH
	{
		.f_name = "atanh",
		.f_narg = 1,
		.f_libm = atanh,
		.f_mpfr = mpfr_atanh,
		.f_u.fp1 = dom_atanh
	},
#endif

	/* sin() */
#ifdef	HAVE_SIN
	{
		.f_name = "sin",
		.f_narg = 1,
		.f_libm = sin,
		.f_mpfr = mpfr_sin,
		.f_u.fp1 = dom_sin
	},
#endif

	/* cbrt() */
#ifdef	HAVE_CBRT
	{
		.f_name = "cbrt",
		.f_narg = 1,
		.f_libm = cbrt,
		.f_mpfr = mpfr_cbrt,
		.f_u.fp1 = dom_cbrt
	},
#endif

	/* ceil() */
#ifdef	HAVE_CEIL
	{
		.f_name = "ceil",
		.f_narg = 1,
		.f_libm = ceil,
		.f_mpfr = mpfr_ceil,
		.f_u.fp1 = dom_ceil
	},
#endif

	/* cos() */
#ifdef	HAVE_COS
	{
		.f_name = "cos",
		.f_narg = 1,
		.f_libm = cos,
		.f_mpfr = mpfr_cos,
		.f_u.fp1 = dom_cos
	},
#endif

	/* cosh() */
#ifdef	HAVE_COSH
	{
		.f_name = "cosh",
		.f_narg = 1,
		.f_libm = cosh,
		.f_mpfr = mpfr_cosh,
		.f_u.fp1 = dom_cosh
	},
#endif

	/* exp() */
#ifdef	HAVE_EXP
	{
		.f_name = "exp",
		.f_narg = 1,
		.f_libm = exp,
		.f_mpfr = mpfr_exp,
		.f_u.fp1 = dom_exp
	},
#endif

	/* expm1() */
#ifdef	HAVE_EXPM1
	{
		.f_name = "expm1",
		.f_narg = 1,
		.f_libm = expm1,
		.f_mpfr = mpfr_expm1,
		.f_u.fp1 = dom_expm1
	},
#endif

	/* exp2() */
#ifdef	HAVE_EXP2
	{
		.f_name = "exp2",
		.f_narg = 1,
		.f_libm = exp2,
		.f_mpfr = mpfr_exp2,
		.f_u.fp1 = dom_exp2
	},
#endif

	/* erf() */
#ifdef	HAVE_ERF
	{
		.f_name = "erf",
		.f_narg = 1,
		.f_libm = erf,
		.f_mpfr = mpfr_erf,
		.f_u.fp1 = dom_erf
	},
#endif

	/* erfc() */
#ifdef	HAVE_ERFC
	{
		.f_name = "erfc",
		.f_narg = 1,
		.f_libm = erfc,
		.f_mpfr = mpfr_erfc,
		.f_u.fp1 = dom_erfc
	},
#endif

	/* fabs() */
#ifdef	HAVE_FABS
	{
		.f_name = "fabs",
		.f_narg = 1,
		.f_libm = fabs,
		.f_mpfr = mpfr_abs,
		.f_u.fp1 = dom_fabs
	},
#endif

	/* floor() */
#ifdef	HAVE_FLOOR
	{
		.f_name = "floor",
		.f_narg = 1,
		.f_libm = floor,
		.f_mpfr = mpfr_floor,
		.f_u.fp1 = dom_floor
	},
#endif

	/* hypot() */
#ifdef	HAVE_HYPOT
	{
		.f_name = "hypot",
		.f_narg = 2,
		.f_libm = hypot,
		.f_mpfr = mpfr_hypot,
		.f_u.fp2 = dom_hypot
	},
#endif

	/* log()*/
#ifdef	HAVE_LOG
	{
		.f_name = "log",
		.f_narg = 1,
		.f_libm = log,
		.f_mpfr = mpfr_log,
		.f_u.fp1 = dom_log
	},
#endif

	/* log1p() */
#ifdef	HAVE_LOG1P
	{
		.f_name = "log1p",
		.f_narg = 1,
		.f_libm = log1p,
		.f_mpfr = mpfr_log1p,
		.f_u.fp1 = dom_log1p
	},
#endif

	/* log10() */
#ifdef	HAVE_LOG10
	{
		.f_name = "log10",
		.f_narg = 1,
		.f_libm = log10,
		.f_mpfr = mpfr_log10,
		.f_u.fp1 = dom_log10
	},
#endif

	/* log2() */
#ifdef	HAVE_LOG2
	{
		.f_name = "log2",
		.f_narg = 1,
		.f_libm = log2,
		.f_mpfr = mpfr_log2,
		.f_u.fp1 = dom_log2
	},
#endif

	/* remainder() */
#ifdef  HAVE_REMAINDER
	{
		.f_name = "remainder",
		.f_narg = 2,
		.f_libm = remainder,
		.f_mpfr = mpfr_remainder,
		.f_u.fp2 = dom_remainder
	},
#endif

	/* rint() */
#ifdef	HAVE_RINT
	{
		.f_name = "rint",
		.f_narg = 1,
		.f_libm = rint,
		.f_mpfr = mpfr_rint,
		.f_u.fp1 = dom_rint
	},
#endif

	/* sinh() */
#ifdef	HAVE_SINH
	{
		.f_name = "sinh",
		.f_narg = 1,
		.f_libm = sinh,
		.f_mpfr = mpfr_sinh,
		.f_u.fp1 = dom_sinh
	},
#endif

	/* sqrt() */
#ifdef	HAVE_SQRT
	{
		.f_name = "sqrt",
		.f_narg = 1,
		.f_libm = sqrt,
		.f_mpfr = mpfr_sqrt,
		.f_u.fp1 = dom_sqrt
	},
#endif

	/* pow() */
#ifdef	HAVE_POW
	{
		.f_name = "pow",
		.f_narg = 2,
		.f_libm = pow,
		.f_mpfr = mpfr_pow,
		.f_u.fp2 = dom_pow
	},
#endif

	/* tan() */
#ifdef	HAVE_TAN
	{
		.f_name = "tan",
		.f_narg = 1,
		.f_libm = tan,
		.f_mpfr = mpfr_tan,
		.f_u.fp1 = dom_tan
	},
#endif

	/* tanh() */
#ifdef  HAVE_TANH
	{
	  .f_name = "tanh",
	  .f_narg = 1,
	  .f_libm = tanh,
	  .f_mpfr = mpfr_tanh,
	  .f_u.fp1 = dom_tanh
	},
#endif

	/* tgamma() */
#ifdef	HAVE_TGAMMA
	{
		.f_name = "tgamma",
		.f_narg = 1,
		.f_libm = tgamma,
		.f_mpfr = mpfr_gamma,
		.f_u.fp1 = dom_tgamma
	},
#endif

	/* y0() */
#ifdef	HAVE_Y0
	{
		.f_name = "y0",
		.f_narg = 1,
		.f_libm = y0,
		.f_mpfr = mpfr_y0,
		.f_u.fp1 = dom_y0
	},
#endif

	/* y1() */
#ifdef	HAVE_Y1
	{
		.f_name = "y1",
		.f_narg = 1,
		.f_libm = y1,
		.f_mpfr = mpfr_y1,
		.f_u.fp1 = dom_y1
	},
#endif

#if 0
	/* yn() */
#ifdef	HAVE_YN
	{
		.f_name = "yn",
		.f_narg = 2,
		.f_libm = yn,
		.f_mpfr = mpfr_yn,
		.f_u.fp2 = dom_yn
	}
#endif
#endif
};

const int fsize = sizeof(ftable) / sizeof(ftable[0]);

const struct fentry *
getfunctionbyname(const char *fname)
{
	const struct fentry *f;
	int i;

	assert(fname);

	for (i = 0; i < fsize; i++) {
		f = &ftable[i];
		if (strcmp(fname, f->f_name) == 0) {
			assert(f->f_narg == 1 || f->f_narg == 2);
			assert(f->f_u.fp1 || f->f_u.fp2);
			return (f);
		}
	}

	return (NULL);
}

void
printfunctions(void)
{
	int i;

	for (i = 0; i < fsize; i++)
		printf("%s ", ftable[i].f_name);
	printf("\n");
}
