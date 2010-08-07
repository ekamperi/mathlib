#define _XOPEN_SOURCE 600

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <gmp.h>
#include <mpc.h>
#include <mpfr.h>

#include "config.h"
#include "gen.h"
#include "mytypes.h"

/*******************************************************************************
 *				Real arithmetic
 ******************************************************************************/
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
dom_fmod(long double x, long double y)
{
	return (fpclassify(y) != FP_ZERO);
}

static int
dom_hypot(long double x, long double y)
{
	return 1;
}

static int
dom_j0(long double x)
{
	return 1;
}

static int
dom_j1(long double x)
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
dom_round(long double x)
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
dom_trunc(long double x)
{
	return 1;
}

static int
dom_y0(long double x)
{
	return (x > 0.0);
}

static int
dom_y1(long double x)
{
	return (x > 0.0);
}

static int
dom_yn(long double x, long double y)
{
	return (y > 0.0);
}

/*******************************************************************************
 *				Complex arithmetic
 ******************************************************************************/
static int
dom_cabs(long double complex z)
{
	return 1;
}

static int
dom_cacos(long double complex z)
{
	return 1;
}

static int
dom_cacosh(long double complex z)
{
	return 1;
}

static int
dom_carg(long double complex z)
{
	/* XXX */
	return 1;
}

static int
dom_casin(long double complex z)
{
	return 1;
}

static int
dom_casinh(long double complex z)
{
	return 1;
}

static int
dom_catan(long double complex z)
{
	return 1;
}

static int
dom_catanh(long double complex z)
{
	return 1;
}

static int
dom_ccos(long double complex z)
{
	return 1;
}

static int
dom_cexp(long double complex z)
{
	return 1;
}

static int
dom_clog(long double complex z)
{
	long_double_complex ldcz = { .z = z };

	return (fpclassify(ldcz.parts[0]) != FP_ZERO ||
		fpclassify(ldcz.parts[1]) != FP_ZERO);
}

static int
dom_cpow(long double complex z, long double complex w)
{
	/* XXX */
	return 1;
}

static int
dom_csin(long double complex z)
{
	return 1;
}

static int
dom_csinh(long double complex z)
{
	return 1;
}

static int
dom_csqrt(long double complex z)
{
	return 1;
}

static int
dom_ctan(long double complex z)
{
	/* XXX */
	return 1;
}

static int
dom_conj(long double complex z)
{
	return 1;
}

/*
 * ISO/IEC 9899:1999 - 6.7.8.10 - Initialization
 *
 * If an object that has static storage duration is not initialized explicitly,
 * then [...] if it has pointer type, it is initialized to a null pointer.
 *
 * Therefore, we can safely assume that f_namel, f_libml_real or f_liml_complex
 * is NULL, if autoconf didn't define the corresponding HAVE_XYZ symbol for us.
 */
static const struct fentry
ftable[] = {
/*******************************************************************************
 *				Real arithmetic
 ******************************************************************************/
	/* acos() */
#ifdef	HAVE_ACOS
	{
		.f_narg = 1,
		.f_name = "acos",
		.f_libm_real = acos,
#ifdef	HAVE_ACOSL
		.f_namel = "acosl",
		.f_libml_real = acosl,
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
		.f_libm_real = acosh,
#ifdef  HAVE_ACOSHL
		.f_namel = "acoshl",
		.f_libml_real = acoshl,
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
		.f_libm_real = asin,
#ifdef	HAVE_ASINL
		.f_namel = "asinl",
		.f_libml_real = asinl,
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
		.f_libm_real = asinh,
#ifdef	HAVE_ASINHL
		.f_namel = "asinhl",
		.f_libml_real = asinhl,
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
		.f_libm_real = atan,
#ifdef	HAVE_ATANL
		.f_namel = "atanl",
		.f_libml_real = atanl,
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
		.f_libm_real = atan2,
#ifdef	HAVE_ATAN2L
		.f_namel = "atan2l",
		.f_libml_real = atan2l,
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
		.f_libm_real = atanh,
#ifdef	HAVE_ATANHL
		.f_namel = "atanhl",
		.f_libml_real = atanhl,
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
		.f_libm_real = cbrt,
#ifdef	HAVE_CBRTL
		.f_namel = "cbrtl",
		.f_libml_real = cbrtl,
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
		.f_libm_real = ceil,
#ifdef	HAVE_CEILL
		.f_namel = "ceill",
		.f_libml_real = ceill,
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
		.f_libm_real = cos,
#ifdef	HAVE_COSL
		.f_namel = "cosl",
		.f_libml_real = cosl,
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
		.f_libm_real = cosh,
#ifdef	HAVE_COSHL
		.f_namel = "coshl",
		.f_libml_real = coshl,
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
		.f_libm_real = exp,
#ifdef	HAVE_EXPL
		.f_namel = "expl",
		.f_libml_real = expl,
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
		.f_libm_real = expm1,
#ifdef	HAVE_EXPM1L
		.f_namel = "expm1l",
		.f_libml_real = expm1l,
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
		.f_libm_real = exp2,
#ifdef	HAVE_EXP2L
		.f_namel = "exp2l",
		.f_libml_real = exp2l,
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
		.f_libm_real = erf,
#ifdef	HAVE_ERFL
		.f_namel = "erfl",
		.f_libml_real = erfl,
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
		.f_libm_real = erfc,
#ifdef	HAVE_ERFCL
		.f_namel = "erfcl",
		.f_libml_real = erfcl,
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
		.f_libm_real = fabs,
#ifdef	HAVE_FABSL
		.f_namel = "fabsl",
		.f_libml_real = fabsl,
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
		.f_libm_real = floor,
#ifdef	HAVE_FLOORL
		.f_namel = "floorl",
		.f_libml_real = floorl,
#endif
		.f_mpfr = mpfr_floor,
		.f_u.fp1 = dom_floor
	},
#endif

	/* fmod() */
#ifdef  HAVE_FMOD
	{
		.f_narg = 2,
		.f_name = "fmod",
		.f_libm_real = fmod,
#ifdef  HAVE_FMODL
		.f_namel = "fmodl",
		.f_libml_real = fmodl,
#endif
		.f_mpfr = mpfr_fmod,
		.f_u.fp1 = dom_fmod
	},
#endif

	/* hypot() */
#ifdef	HAVE_HYPOT
	{
		.f_narg = 2,
		.f_name = "hypot",
		.f_libm_real = hypot,
#ifdef	HAVE_HYPOTL
		.f_namel = "hypotl",
		.f_libml_real = hypotl,
#endif
		.f_mpfr = mpfr_hypot,
		.f_u.fp2 = dom_hypot
	},
#endif

	/* j0() */
#ifdef  HAVE_J0
	{
		.f_narg = 1,
		.f_name = "j0",
		.f_libm_real = j0,
#ifdef  HAVE_J0Lxxx
		.f_namel = "j0l",
		.f_libml_real = j0l,
#endif
		.f_mpfr = mpfr_j0,
		.f_u.fp2 = dom_j0
	},
#endif

	/* j1() */
#ifdef  HAVE_J1
	{
		.f_narg = 1,
		.f_name = "j1",
		.f_libm_real = j1,
#ifdef  HAVE_J1Lxxx
		.f_namel = "j1l",
		.f_libml_real = j1l,
#endif
		.f_mpfr = mpfr_j1,
		.f_u.fp2 = dom_j1
	},
#endif


	/* log()*/
#ifdef	HAVE_LOG
	{
		.f_narg = 1,
		.f_name = "log",
		.f_libm_real = log,
#ifdef	HAVE_LOGL
		.f_namel = "logl",
		.f_libml_real = logl,
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
		.f_libm_real = log1p,
#ifdef	HAVE_LOG1PL
		.f_namel = "log1pl",
		.f_libml_real = log1pl,
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
		.f_libm_real = log10,
#ifdef	HAVE_LOG10L
		.f_namel = "log10l",
		.f_libml_real = log10l,
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
		.f_libm_real = log2,
#ifdef	HAVE_LOG2L
		.f_namel = "log2l",
		.f_libml_real = log2l,
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
		.f_libm_real = remainder,
#ifdef	HAVE_REMAINDERL
		.f_namel = "remainderl",
		.f_libml_real = remainderl,
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
		.f_libm_real = rint,
#ifdef	HAVE_RINTL
		.f_namel = "rintl",
		.f_libml_real = rintl,
#endif
		.f_mpfr = mpfr_rint,
		.f_u.fp1 = dom_rint
	},
#endif

	/* round() */
#ifdef  HAVE_ROUND
	{
		.f_narg = 1,
		.f_name = "round",
		.f_libm_real = round,
#ifdef  HAVE_ROUNDL
		.f_namel = "roundl",
		.f_libml_real = roundl,
#endif
		.f_mpfr = mpfr_round,
		.f_u.fp1 = dom_round
	},
#endif

	/* sin() */
#ifdef  HAVE_SIN
	{
		.f_narg = 1,
		.f_name = "sin",
		.f_libm_real = sin,
#ifdef  HAVE_SINL
		.f_namel = "sinl",
		.f_libml_real = sinl,
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
		.f_libm_real = sinh,
#ifdef	HAVE_SINHL
		.f_namel = "sinhl",
		.f_libml_real = sinhl,
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
		.f_libm_real = sqrt,
#ifdef	HAVE_SQRTL
		.f_namel = "sqrtl",
		.f_libml_real = sqrtl,
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
		.f_libm_real = pow,
#ifdef	HAVE_POWL
		.f_namel = "powl",
		.f_libml_real = powl,
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
		.f_libm_real = tan,
#ifdef	HAVE_TANL
		.f_namel = "tanl",
		.f_libml_real = tanl,
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
		.f_libm_real = tanh,
#ifdef	HAVE_TANHL
		.f_namel = "tanhl",
		.f_libml_real = tanhl,
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
		.f_libm_real = tgamma,
#ifdef	HAVE_TGAMMAL
		.f_namel = "tgammal",
		.f_libml_real = tgammal,
#endif
		.f_mpfr = mpfr_gamma,
		.f_u.fp1 = dom_tgamma
	},
#endif

	/* trunc() */
#ifdef	HAVE_TRUNC
	{
		.f_narg = 1,
		.f_name = "trunc",
		.f_libm_real = trunc,
#ifdef  HAVE_TGAMMAL
		.f_namel = "truncl",
		.f_libml_real = truncl,
#endif
		.f_mpfr = mpfr_trunc,
		.f_u.fp1 = dom_trunc
	},
#endif

	/* y0() */
#ifdef	HAVE_Y0
	{
		.f_narg = 1,
		.f_name = "y0",
		.f_libm_real = y0,
		.f_mpfr = mpfr_y0,
		.f_u.fp1 = dom_y0
	},
#endif

	/* y1() */
#ifdef	HAVE_Y1
	{
		.f_narg = 1,
		.f_name = "y1",
		.f_libm_real = y1,
		.f_mpfr = mpfr_y1,
		.f_u.fp1 = dom_y1
	},
#endif

	/* yn() */
#ifdef	HAVE_YNxxx
	{
		.f_narg = 2,
		.f_name = "yn",
		.f_libm_real = yn,
		.f_mpfr = mpfr_yn,
		.f_u.fp2 = dom_yn
	},
#endif

/*******************************************************************************
 *				Complex arithmetic
 ******************************************************************************/
	/* cabs() */
#ifdef  HAVE_CABS
	{
		.f_narg = 1,
		.f_name = "cabs",
		.f_libm_complex = cabs,
#ifdef  HAVE_CABSL
		.f_namel = "cabsl",
		.f_libml_complex = cabsl,
#endif
		.f_mpc = mpc_abs,
		.f_uc.fp1 = dom_cabs
	},
#endif

	/* cacos() */
#ifdef  HAVE_CACOS
	{
		.f_narg = 1,
		.f_name = "cacos",
		.f_libm_complex = cacos,
#ifdef  HAVE_CACOSL
		.f_namel = "cacosl",
		.f_libml_complex = cacosl,
#endif
		.f_mpc = mpc_acos,
		.f_uc.fp1 = dom_cacos
	},
#endif

	/* cacosh() */
#ifdef  HAVE_CACOSH
	{
		.f_narg = 1,
		.f_name = "cacosh",
		.f_libm_complex = cacosh,
#ifdef  HAVE_CACOSHL
		.f_namel = "cacoshl",
		.f_libml_complex = cacoshl,
#endif
		.f_mpc = mpc_acosh,
		.f_uc.fp1 = dom_cacosh
	},
#endif

	/* carg() */
#ifdef  HAVE_CARG
	{
		.f_narg = 1,
		.f_name = "carg",
		.f_libm_complex = carg,
#ifdef  HAVE_CARGL
		.f_namel = "cargl",
		.f_libml_complex = cargl,
#endif
		.f_mpc = mpc_arg,
		.f_uc.fp1 = dom_carg
	},
#endif

	/* casin() */
#ifdef  HAVE_CASIN
	{
		.f_narg = 1,
		.f_name = "casin",
		.f_libm_complex = casin,
#ifdef  HAVE_CASINL
		.f_namel = "casinl",
		.f_libml_complex = casinl,
#endif
		.f_mpc = mpc_asin,
		.f_uc.fp1 = dom_casin
	},
#endif

	/* casinh() */
#ifdef  HAVE_CASINH
	{
		.f_narg = 1,
		.f_name = "casinh",
		.f_libm_complex = casinh,
#ifdef  HAVE_CASINHL
		.f_namel = "casinhl",
		.f_libml_complex = casinhl,
#endif
		.f_mpc = mpc_asinh,
		.f_uc.fp1 = dom_casinh
	},
#endif

	/* catan() */
#ifdef  HAVE_CATAN
	{
		.f_narg = 1,
		.f_name = "catan",
		.f_libm_complex = catan,
#ifdef  HAVE_CATANL
		.f_namel = "catanl",
		.f_libml_complex = catanl,
#endif
		.f_mpc = mpc_atan,
		.f_uc.fp1 = dom_catan
	},
#endif

	/* catanh() */
#ifdef  HAVE_CATANH
	{
		.f_narg = 1,
		.f_name = "catanh",
		.f_libm_complex = catanh,
#ifdef  HAVE_CATANHL
		.f_namel = "catanhl",
		.f_libml_complex = catanhl,
#endif
		.f_mpc = mpc_atanh,
		.f_uc.fp1 = dom_catanh
	},
#endif

	/* ccos() */
#ifdef  HAVE_CCOS
	{
		.f_narg = 1,
		.f_name = "ccos",
		.f_libm_complex = ccos,
#ifdef  HAVE_CARGL
		.f_namel = "ccosl",
		.f_libml_complex = ccosl,
#endif
		.f_mpc = mpc_cos,
		.f_uc.fp1 = dom_ccos
	},
#endif

	/* cexp() */
#ifdef  HAVE_CEXP
	{
		.f_narg = 1,
		.f_name = "cexp",
		.f_libm_complex = cexp,
#ifdef  HAVE_CEXPL
		.f_namel = "cexpl",
		.f_libml_complex = cexpl,
#endif
		.f_mpc = mpc_exp,
		.f_uc.fp1 = dom_cexp
	},
#endif

	/* clog() */
#ifdef	HAVE_CLOG
	{
		.f_narg = 1,
		.f_name = "clog",
		.f_libm_complex = clog,
#ifdef  HAVE_CLOGL
		.f_namel = "clogl",
		.f_libml_complex = clogl,
#endif
		.f_mpc = mpc_log,
		.f_uc.fp1 = dom_clog
	},
#endif

	/* conj() */
#ifdef  HAVE_CONJ
	{
		.f_narg = 1,
		.f_name = "conj",
		.f_libm_complex = conj,
#ifdef  HAVE_CONJL
		.f_namel = "conjl",
		.f_libml_complex = conjl,
#endif
		.f_mpc = mpc_conj,
		.f_uc.fp1 = dom_conj
	},
#endif

	/* cpow() */
#ifdef  HAVE_CPOW
	{
	  .f_narg = 2,
	  .f_name = "cpow",
	  .f_libm_complex = cpow,
#ifdef  HAVE_CPOWL
	  .f_namel = "cpowl",
	  .f_libml_complex = cpowl,
#endif
	  .f_mpc = mpc_pow,
	  .f_uc.fp1 = dom_cpow
	},
#endif

	/* csin() */
#ifdef  HAVE_CSIN
	{
	  .f_narg = 1,
	  .f_name = "csin",
	  .f_libm_complex = csin,
#ifdef  HAVE_CSINL
	  .f_namel = "csinl",
	  .f_libml_complex = csinl,
#endif
	  .f_mpc = mpc_sin,
	  .f_uc.fp1 = dom_csin
	},
#endif

	/* csinh() */
#ifdef  HAVE_CSINH
	{
		.f_narg = 1,
		.f_name = "csinh",
		.f_libm_complex = csinh,
#ifdef  HAVE_CSINHL
		.f_namel = "csinhl",
		.f_libml_complex = csinhl,
#endif
		.f_mpc = mpc_sinh,
		.f_uc.fp1 = dom_csinh
	},
#endif

	/* csqrt() */
#ifdef  HAVE_CSQRT
	{
		.f_narg = 1,
		.f_name = "csqrt",
		.f_libm_complex = csqrt,
#ifdef  HAVE_CSQRTL
		.f_namel = "csqrtl",
		.f_libml_complex = csqrtl,
#endif
		.f_mpc = mpc_sqrt,
		.f_uc.fp1 = dom_csqrt
	},
#endif

	/* ctan() */
#ifdef  HAVE_CTAN
	{
	  .f_narg = 1,
	  .f_name = "ctan",
	  .f_libm_complex = ctan,
#ifdef  HAVE_CTANL
	  .f_namel = "ctanl",
	  .f_libml_complex = ctanl,
#endif
	  .f_mpc = mpc_tan,
	  .f_uc.fp1 = dom_ctan
	}
#endif
};

const int fsize = sizeof(ftable) / sizeof(ftable[0]);

static void
assertfunction(const struct fentry *f)
{
	assert(f);
	assert(f->f_narg == 1 || f->f_narg == 2);
	assert(f->f_name);

	if (f->f_mpfr) {
		assert(f->f_u.fp1 || f->f_u.fp2);
		assert(f->f_libm_real);
		if (f->f_namel)
			assert(f->f_libml_real);
	}

	if (f->f_mpc) {
		assert(f->f_uc.fp1 || f->f_u.fp2);
		assert(f->f_libm_complex);
		if (f->f_namel)
			assert(f->f_libml_complex);
	}

	assert(f->f_mpfr || f->f_mpc);
}

const struct fentry *
getfunctionbyidx(int idx)
{
	assert(0 <= idx && idx < fsize);

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
			assertfunction(f);
			return (f);
		}
	}

	return (NULL);
}

void
printfunctions(void)
{
	const struct fentry *f;
	int i, cnt;

	for (i = 0, cnt = 0; i < fsize; i++, cnt++) {
		f = &ftable[i];
		printf("%-10s ", f->f_name);
		if (cnt % 5 == 4)
			printf("\n");
		if (f->f_namel) {
			cnt++;
			printf("%-10s ", f->f_namel);
			if (cnt % 5 == 4)
				printf("\n");
		}
	}

	/* If cnt % 5 was equal to 4, then now it's equal to 0 */
	if (cnt % 5 != 0)
		printf("\n");
}
