#define _XOPEN_SOURCE 600

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <gmp.h>
#include <mpfr.h>

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
dom_cosh(double x)
{
	return 1;
}

static int
dom_erfc(double x)
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
dom_pow(double x, double y)
{
	/* return (x >= 0.0 || (floor(y) == y)); */
	return ((x >= 0.0 || (floor(y) == y)) &&
	    (x > 1.0 && x < 1E5) && (y > 0.0 && y < 1E5));
}

static int
dom_y0(double x)
{
	return (x > 0.0);
}

static int
dom_y1(double x)
{
	return (x > 0.0);
}

static int
dom_yn(double x)
{
	return (x > 0.0);
}

const struct fentry
ftable[] = {
	/* acos() */
	{
		.f_name = "acos",
		.f_narg = 1,
		.f_libm = acos,
		.f_mpfr = mpfr_acos,
		.f_u.fp1 = dom_acos
	},

	/* acosh() */
	{
                .f_name = "acosh",
                .f_narg = 1,
                .f_libm = acosh,
                .f_mpfr = mpfr_acosh,
                .f_u.fp1 = dom_acosh
        },

	/* asin() */
	{
		.f_name = "asin",
		.f_narg = 1,
		.f_libm = asin,
		.f_mpfr = mpfr_asin,
		.f_u.fp1 = dom_asin
	},

	/* asinh() */
        {
                .f_name = "asinh",
                .f_narg = 1,
                .f_libm = asinh,
                .f_mpfr = mpfr_asinh,
                .f_u.fp1 = dom_asinh
        },

	/* atan() */
	{
		.f_name = "atan",
		.f_narg = 1,
		.f_libm = atan,
		.f_mpfr = mpfr_atan,
		.f_u.fp1 = dom_atan
	},

	/* atan2() */
	{
		.f_name = "atan2",
		.f_narg = 2,
		.f_libm = atan2,
		.f_mpfr = mpfr_atan2,
		.f_u.fp1 = dom_atan
	},

	/* sin() */
	{
		.f_name = "sin",
		.f_narg = 1,
		.f_libm = sin,
		.f_mpfr = mpfr_sin,
		.f_u.fp1 = dom_sin
	},

	/* cos() */
	{
		.f_name = "cos",
		.f_narg = 1,
		.f_libm = cos,
		.f_mpfr = mpfr_cos,
		.f_u.fp1 = dom_cos
	},

	/* cosh() */
	{
		.f_name = "cosh",
		.f_narg = 1,
		.f_libm = cosh,
		.f_mpfr = mpfr_cosh,
		.f_u.fp1 = dom_cosh
	},

	/* exp() */
	{
		.f_name = "exp",
		.f_narg = 1,
		.f_libm = exp,
		.f_mpfr = mpfr_exp,
		.f_u.fp1 = dom_exp
	},

        /* expm1() */
        {
                .f_name = "expm1",
                .f_narg = 1,
                .f_libm = expm1,
                .f_mpfr = mpfr_expm1,
		.f_u.fp1 = dom_expm1
        },

	/* exp2() */
	{
		.f_name = "exp2",
		.f_narg = 1,
		.f_libm = exp2,
		.f_mpfr = mpfr_exp2,
                .f_u.fp1 = dom_exp2
	},

	/* erfc() */
	{
		.f_name = "erfc",
		.f_narg = 1,
		.f_libm = erfc,
		.f_mpfr = mpfr_erfc,
		.f_u.fp1 = dom_erfc
	},

	/* hypot() */
	{
		.f_name = "hypot",
                .f_narg = 2,
		.f_libm = hypot,
                .f_mpfr = mpfr_hypot,
                .f_u.fp2 = dom_hypot
	},

	/* log()*/
	{
		.f_name = "log",
		.f_narg = 1,
		.f_libm = log,
		.f_mpfr = mpfr_log,
		.f_u.fp1 = dom_log
	},

	/* log1p() */
	{
		.f_name = "log1p",
		.f_narg = 1,
		.f_libm = log1p,
		.f_mpfr = mpfr_log1p,
                .f_u.fp1 = dom_log1p
	},

	/* log10() */
	{
		.f_name = "log10",
		.f_narg = 1,
		.f_libm = log10,
		.f_mpfr = mpfr_log10,
		.f_u.fp1 = dom_log10
	},

	/* log2() */
	{
		.f_name = "log2",
		.f_narg = 1,
		.f_libm = log2,
		.f_mpfr = mpfr_log2,
		.f_u.fp1 = dom_log2
	},

	/* rint() */
	{
		.f_name = "rint",
		.f_narg = 1,
		.f_libm = rint,
		.f_mpfr = mpfr_rint,
		.f_u.fp1 = dom_rint
	},

	/* sinh() */
	{
		.f_name = "sinh",
		.f_narg = 1,
		.f_libm = sinh,
		.f_mpfr = mpfr_sinh,
		.f_u.fp1 = dom_sinh
	},

	/* sqrt() */
	{
		.f_name = "sqrt",
		.f_narg = 1,
		.f_libm = sqrt,
		.f_mpfr = mpfr_sqrt,
		.f_u.fp1 = dom_sqrt
	},

	/* pow() */
	{
		.f_name = "pow",
		.f_narg = 2,
		.f_libm = pow,
		.f_mpfr = mpfr_pow,
		.f_u.fp2 = dom_pow
	},

	/* y0() */
	{
                .f_name = "y0",
                .f_narg = 1,
		.f_libm = y0,
                .f_mpfr = mpfr_y0,
                .f_u.fp1 = dom_y0
        },

	/* y1() */
	{
		.f_name = "y1",
                .f_narg = 1,
                .f_libm = y1,
		.f_mpfr = mpfr_y1,
                .f_u.fp1 = dom_y1
        },

	/* yn() */
	{
		.f_name = "yn",
                .f_narg = 1,
                .f_libm = yn,
		.f_mpfr = mpfr_yn,
                .f_u.fp1 = dom_yn
        }
};

const struct fentry *
getfuncbyname(const char *fname)
{
	size_t i, N;

	N = sizeof(ftable) / sizeof(ftable[i]);
	for (i = 0; i < N; i++) {
		if (strcmp(fname, ftable[i].f_name) == 0) {
			return (&ftable[i]);
		}
	}

	return (NULL);
}

#if 0
int
main(void)
{
	printf("%d\n",
	    (*ftable[2].f_u.fp2)(-100, 200.1));

	return 0;
}
#endif
