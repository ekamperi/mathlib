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
dom_acos(long double x)
{
	return (x >= -1.0 && x <= 1.0);
}

static int
dom_acosh(long double x)
{
	return (x >= 1.0);
}

static int
dom_asin(long double x)
{
	return (x >= -1.0 && x <= 1.0);
}

static int
dom_asinh(long double x)
{
	return 1;
}

static int
dom_atan(long double x)
{
	return 1;
}

static int
dom_atan2(long double y, long double x)
{
	return (fpclassify(x) != FP_ZERO);
}

static int
dom_atanh(long double x)
{
	return (x > -1.0 && x < 1.0);
}

static int
dom_cbrt(long double x)
{
	return 1;
}

static int
dom_ceil(long double x)
{
	return 1;
}

static int
dom_cos(long double x)
{
	return 1;
}

static int
dom_cosh(long double x)
{
	return 1;
}

static int
dom_erf(long double x)
{
	return 1;
}

static int
dom_erfc(long double x)
{
	return 1;
}

static int
dom_exp(long double x)
{
	return 1;
}

static int
dom_expm1(long double x)
{
	return 1;
}

static int
dom_exp2(long double x)
{
	return 1;
}

static int
dom_fabs(long double x)
{
	return 1;
}

static int
dom_floor(long double x)
{
	return 1;
}

static int
dom_hypot(long double x, long double y)
{
	return 1;
}

static int
dom_log(long double x)
{
	return (x > 0.0);
}

static int
dom_log1p(long double x)
{
	return (x > -1.0);
}

static int
dom_log10(long double x)
{
	return (x > 0.0);
}

static int
dom_log2(long double x)
{
	return (x > 0.0);
}

static int
dom_remainder(long double x, long double y)
{
	return (fpclassify(y) != FP_ZERO);
}

static int
dom_rint(long double x)
{
	return 1;
}

static int
dom_sin(long double x)
{
	return 1;
}

static int
dom_sinh(long double x)
{
	return 1;
}

static int
dom_sqrt(long double x)
{
	return (x >= 0.0);
}

static int
dom_tan(long double x)
{
	return (fabs(x - M_PI_2) > 1E-10);
}

static int
dom_tanh(long double x)
{
	return 1;
}

static int
dom_pow(long double x, long double y)
{
	return (x >= 0.0 || (floor(y) == y));
}

static int
dom_tgamma(long double x)
{
	return (x > 0.0 || (floor(x) != x));
}

static int
dom_y0(long double x)
{
	return (x > 0.0);
}

static int
dom_y1(long double x)
{
	return (x > 1.0 && x < 1E5);
}

static int
dom_yn(long double x, long double y)
{
	return (y > 0.0 && y < 1E10);
}

static const struct fentry
ftable[] = {
	/* acos() */
#ifdef	HAVE_ACOS
	{
		.f_narg = 1,
		.f_name = "acos",
		.f_libm = acos,
#ifdef	HAVE_ACOSL
		.f_namel = "acosl",
		.f_libml = acosl,
#endif
		.f_mpfr = mpfr_acos,
		.f_u.fp1 = dom_acos
	},
#endif

	/* acosh() */
#ifdef	HAVE_ACOSH
	{
		.f_narg = 1,
		.f_name = "acosh",
		.f_libm = acosh,
#ifdef  HAVE_ACOSHL
		.f_namel = "acoshl",
		.f_libml = acoshl,
#endif
		.f_mpfr = mpfr_acosh,
		.f_u.fp1 = dom_acosh
	},
#endif

	/* asin() */
#ifdef	HAVE_ASIN
	{
		.f_narg = 1,
		.f_name = "asin",
		.f_libm = asin,
#ifdef	HAVE_ASINL
		.f_namel = "asinl",
		.f_libml = asinl,
#endif
		.f_mpfr = mpfr_asin,
		.f_u.fp1 = dom_asin
	},
#endif

	/* asinh() */
#ifdef	HAVE_ASINH
	{
		.f_narg = 1,
		.f_name = "asinh",
		.f_libm = asinh,
#ifdef	HAVE_ASINHL
		.f_namel = "asinhl",
		.f_libml = asinhl,
#endif
		.f_mpfr = mpfr_asinh,
		.f_u.fp1 = dom_asinh
	},
#endif

	/* atan() */
#ifdef	HAVE_ATAN
	{
		.f_narg = 1,
		.f_name = "atan",
		.f_libm = atan,
#ifdef	HAVE_ATANL
		.f_namel = "atanl",
		.f_libml = atanl,
#endif
		.f_mpfr = mpfr_atan,
		.f_u.fp1 = dom_atan
	},
#endif

	/* atan2() */
#ifdef	HAVE_ATAN2
	{
		.f_narg = 2,
		.f_name = "atan2",
		.f_libm = atan2,
#ifdef	HAVE_ATAN2L
		.f_namel = "atan2l",
		.f_libml = atan2l,
#endif
		.f_mpfr = mpfr_atan2,
		.f_u.fp2 = dom_atan2
	},
#endif

	/* atanh() */
#ifdef	HAVE_ATANH
	{
		.f_narg = 1,
		.f_name = "atanh",
		.f_libm = atanh,
#ifdef	HAVE_ATANHL
		.f_namel = "atanhl",
		.f_libml = atanhl,
#endif
		.f_mpfr = mpfr_atanh,
		.f_u.fp1 = dom_atanh
	},
#endif

	/* cbrt() */
#ifdef	HAVE_CBRT
	{
		.f_narg = 1,
		.f_name = "cbrt",
		.f_libm = cbrt,
#ifdef	HAVE_CBRTL
		.f_namel = "cbrtl",
		.f_libml = cbrtl,
#endif
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
#ifdef	HAVE_CEILL
		.f_namel = "ceill",
		.f_libml = ceill,
#endif
		.f_mpfr = mpfr_ceil,
		.f_u.fp1 = dom_ceil
	},
#endif

	/* cos() */
#ifdef	HAVE_COS
	{
		.f_narg = 1,
		.f_name = "cos",
		.f_libm = cos,
#ifdef	HAVE_COSL
		.f_namel = "cosl",
		.f_libml = cosl,
#endif
		.f_mpfr = mpfr_cos,
		.f_u.fp1 = dom_cos
	},
#endif

	/* cosh() */
#ifdef	HAVE_COSH
	{
		.f_narg = 1,
		.f_name = "cosh",
		.f_libm = cosh,
#ifdef	HAVE_COSHL
		.f_namel = "coshl",
		.f_libml = coshl,
#endif
		.f_mpfr = mpfr_cosh,
		.f_u.fp1 = dom_cosh
	},
#endif

	/* exp() */
#ifdef	HAVE_EXP
	{
		.f_narg = 1,
		.f_name = "exp",
		.f_libm = exp,
#ifdef	HAVE_EXPL
		.f_namel = "expl",
		.f_libml = expl,
#endif
		.f_mpfr = mpfr_exp,
		.f_u.fp1 = dom_exp
	},
#endif

	/* expm1() */
#ifdef	HAVE_EXPM1
	{
		.f_narg = 1,
		.f_name = "expm1",
		.f_libm = expm1,
#ifdef	HAVE_EXPM1L
		.f_namel = "expm1l",
		.f_libml = expm1l,
#endif
		.f_mpfr = mpfr_expm1,
		.f_u.fp1 = dom_expm1
	},
#endif

	/* exp2() */
#ifdef	HAVE_EXP2
	{
		.f_narg = 1,
		.f_name = "exp2",
		.f_libm = exp2,
#ifdef	HAVE_EXP2L
		.f_namel = "exp2l",
		.f_libml = exp2l,
#endif
		.f_mpfr = mpfr_exp2,
		.f_u.fp1 = dom_exp2
	},
#endif

	/* erf() */
#ifdef	HAVE_ERF
	{
		.f_narg = 1,
		.f_name = "erf",
		.f_libm = erf,
#ifdef	HAVE_ERFL
		.f_namel = "erfl",
		.f_libml = erfl,
#endif
		.f_mpfr = mpfr_erf,
		.f_u.fp1 = dom_erf
	},
#endif

	/* erfc() */
#ifdef	HAVE_ERFC
	{
		.f_narg = 1,
		.f_name = "erfc",
		.f_libm = erfc,
#ifdef	HAVE_ERFCL
		.f_namel = "erfcl",
		.f_libml = erfcl,
#endif
		.f_mpfr = mpfr_erfc,
		.f_u.fp1 = dom_erfc
	},
#endif

	/* fabs() */
#ifdef	HAVE_FABS
	{
		.f_narg = 1,
		.f_name = "fabs",
		.f_libm = fabs,
#ifdef	HAVE_FABSL
		.f_namel = "fabsl",
		.f_libml = fabsl,
#endif
		.f_mpfr = mpfr_abs,
		.f_u.fp1 = dom_fabs
	},
#endif

	/* floor() */
#ifdef	HAVE_FLOOR
	{
		.f_narg = 1,
		.f_name = "floor",
		.f_libm = floor,
#ifdef	HAVE_FLOORL
		.f_namel = "floorl",
		.f_libml = floorl,
#endif
		.f_mpfr = mpfr_floor,
		.f_u.fp1 = dom_floor
	},
#endif

	/* hypot() */
#ifdef	HAVE_HYPOT
	{
		.f_narg = 2,
		.f_name = "hypot",
		.f_libm = hypot,
#ifdef	HAVE_HYPOTL
		.f_namel = "hypotl",
		.f_libml = hypotl,
#endif
		.f_mpfr = mpfr_hypot,
		.f_u.fp2 = dom_hypot
	},
#endif

	/* log()*/
#ifdef	HAVE_LOG
	{
		.f_narg = 1,
		.f_name = "log",
		.f_libm = log,
#ifdef	HAVE_LOGL
		.f_namel = "logl",
		.f_libml = logl,
#endif
		.f_mpfr = mpfr_log,
		.f_u.fp1 = dom_log
	},
#endif

	/* log1p() */
#ifdef	HAVE_LOG1P
	{
		.f_narg = 1,
		.f_name = "log1p",
		.f_libm = log1p,
#ifdef	HAVE_LOG1PL
		.f_namel = "log1pl",
		.f_libml = log1pl,
#endif
		.f_mpfr = mpfr_log1p,
		.f_u.fp1 = dom_log1p
	},
#endif

	/* log10() */
#ifdef	HAVE_LOG10
	{
		.f_narg = 1,
		.f_name = "log10",
		.f_libm = log10,
#ifdef	HAVE_LOG10L
		.f_namel = "log10l",
		.f_libml = log10l,
#endif
		.f_mpfr = mpfr_log10,
		.f_u.fp1 = dom_log10
	},
#endif

	/* log2() */
#ifdef	HAVE_LOG2
	{
		.f_narg = 1,
		.f_name = "log2",
		.f_libm = log2,
#ifdef	HAVE_LOG2L
		.f_namel = "log2l",
		.f_libml = log2l,
#endif
		.f_mpfr = mpfr_log2,
		.f_u.fp1 = dom_log2
	},
#endif

	/* remainder() */
#ifdef  HAVE_REMAINDER
	{
		.f_narg = 2,
		.f_name = "remainder",
		.f_libm = remainder,
#ifdef	HAVE_REMAINDERL
		.f_namel = "remainderl",
		.f_libml = remainderl,
#endif
		.f_mpfr = mpfr_remainder,
		.f_u.fp2 = dom_remainder
	},
#endif

	/* rint() */
#ifdef	HAVE_RINT
	{
		.f_narg = 1,
		.f_name = "rint",
		.f_libm = rint,
#ifdef	HAVE_RINTL
		.f_namel = "rintl",
		.f_libml = rintl,
#endif
		.f_mpfr = mpfr_rint,
		.f_u.fp1 = dom_rint
	},
#endif

	/* sin() */
#ifdef  HAVE_SIN
	{
		.f_narg = 1,
		.f_name = "sin",
		.f_libm = sin,
#ifdef  HAVE_SINL
		.f_namel = "sinl",
		.f_libml = sinl,
#endif
		.f_mpfr = mpfr_sin,
		.f_u.fp1 = dom_sin
	},
#endif

	/* sinh() */
#ifdef	HAVE_SINH
	{
		.f_narg = 1,
		.f_name = "sinh",
		.f_libm = sinh,
#ifdef	HAVE_SINHL
		.f_namel = "sinhl",
		.f_libml = sinhl,
#endif
		.f_mpfr = mpfr_sinh,
		.f_u.fp1 = dom_sinh
	},
#endif

	/* sqrt() */
#ifdef	HAVE_SQRT
	{
		.f_narg = 1,
		.f_name = "sqrt",
		.f_libm = sqrt,
#ifdef	HAVE_SQRTL
		.f_namel = "sqrtl",
		.f_libml = sqrtl,
#endif
		.f_mpfr = mpfr_sqrt,
		.f_u.fp1 = dom_sqrt
	},
#endif

	/* pow() */
#ifdef	HAVE_POW
	{
		.f_narg = 2,
		.f_name = "pow",
		.f_libm = pow,
#ifdef	HAVE_POWL
		.f_namel = "powl",
		.f_libml = powl,
#endif
		.f_mpfr = mpfr_pow,
		.f_u.fp2 = dom_pow
	},
#endif

	/* tan() */
#ifdef	HAVE_TAN
	{
		.f_narg = 1,
		.f_name = "tan",
		.f_libm = tan,
#ifdef	HAVE_TANL
		.f_namel = "tanl",
		.f_libml = tanl,
#endif
		.f_mpfr = mpfr_tan,
		.f_u.fp1 = dom_tan
	},
#endif

	/* tanh() */
#ifdef  HAVE_TANH
	{
		.f_narg = 1,
		.f_name = "tanh",
		.f_libm = tanh,
#ifdef	HAVE_TANHL
		.f_namel = "tanhl",
		.f_libml = tanhl,
#endif
		.f_mpfr = mpfr_tanh,
		.f_u.fp1 = dom_tanh
	},
#endif

	/* tgamma() */
#ifdef	HAVE_TGAMMA
	{
		.f_narg = 1,
		.f_name = "tgamma",
		.f_libm = tgamma,
#ifdef	HAVE_TGAMMAL
		.f_namel = "tgammal",
		.f_libml = tgammal,
#endif
		.f_mpfr = mpfr_gamma,
		.f_u.fp1 = dom_tgamma
	},
#endif

	/* y0() */
#ifdef	HAVE_Y0
	{
		.f_narg = 1,
		.f_name = "y0",
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
getfunctionbyidx(size_t idx)
{
	assert(idx < fsize);

	return (&ftable[idx]);
}

const struct fentry *
getfunctionbyname(const char *fname)
{
	const struct fentry *f;
	int i;

	assert(fname);

	for (i = 0; i < fsize; i++) {
		f = &ftable[i];
		if ((strcmp(fname, f->f_name ) == 0) ||
		    (f->f_namel && strcmp(fname, f->f_namel) == 0)) {
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
