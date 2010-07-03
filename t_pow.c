#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <errno.h>
#include <float.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*#include "t_pow.h"*/

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_pow1);
ATF_TC_HEAD(test_pow1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, pow(x)) pairs");
}
ATF_TC_BODY(test_pow1, tc)
{
#if 0
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	for (i = 0; i < N; i++) {
		/* Sanity check */
		ATF_REQUIRE(t1dtable[i].x >= -1.0 && t1dtable[i].x <= 1.0);

		/* Actual check */
		ATF_CHECK(fpcmp_equal(
			    pow(t1dtable[i].x),
				 t1dtable[i].y));
	}

	/* long double */
#ifdef	HAVE_POWL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	for (i = 0; i < N; i++) {
		/* Sanity check */
		ATF_REQUIRE(t1ldtable[i].x >= -1.0 && t1ldtable[i].x <= 1.0);

		/* Actual check */
		ATF_CHECK(fpcmp_equall(
			    powl(t1ldtable[i].x),
				  t1ldtable[i].y));
	}
#endif
#endif
}

/*
 * Test case 2 -- Domain error
 *
 * For finite values of x < 0, and finite non-integer values of y,
 * a domain error shall occur and either a NaN (if representable),
 * or an implementation-defined value shall be returned.
 */
ATF_TC(test_pow2);
ATF_TC_HEAD(test_pow2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check domain error");
}
ATF_TC_BODY(test_pow2, tc)
{
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		do {
			fx = random_float(FP_NORMAL);
			fy = random_float(FP_NORMAL);
		} while (fx >= 0 || floorf(fy) == fy);
		ATF_CHECK_IFNAN(powf(fx, fy));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		do {
			dx = random_double(FP_NORMAL);
			dy = random_double(FP_NORMAL);
		} while (dx >= 0 || floor(dy) == dy);
		ATF_CHECK_IFNAN(pow(dx, dy));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_POWL
		errno = 0;
		clear_exceptions();
		do {
			ldx = random_long_double(FP_NORMAL);
			ldy = random_long_double(FP_NORMAL);
		} while (ldx >= 0 || floorl(ldy) == ldy);
		ATF_CHECK_IFNAN(powl(ldx, ldy));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
	}
}

/*
 * Test case 3 -- Overflow
 *
 * If the correct value would cause overflow, a range error shall occur and
 * pow(), powf(), and powl() shall return +-HUGE_VAL, +-HUGE_VALF, and
 * +-HUGE_VALL, respectively, with the same sign as the correct value of the
 * function.
 */
ATF_TC(test_pow3);
ATF_TC_HEAD(test_pow3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check overflow");
}
ATF_TC_BODY(test_pow3, tc)
{
	/* float */
#ifdef	FLT_MAX
	errno = 0;
	clear_exceptions();
	ATF_CHECK(powf(FLT_MAX, FLT_MAX) == HUGE_VALF);
	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
#endif

	/* double */
#ifdef	DBL_MAX
	errno = 0;
	clear_exceptions();
	ATF_CHECK(pow(DBL_MAX, DBL_MAX) == HUGE_VAL);
	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
#endif

	/* long double */
#if defined(HAVE_POWL) && defined(LDBL_MAX)
	errno = 0;
	clear_exceptions();
	ATF_CHECK(powl(LDBL_MAX, LDBL_MAX) == HUGE_VALL);
	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
#endif
}

/*
 * Test case 4 -- Edge cases
 * XXX: Should we turn those into fuzzing tests ?
 */
static const struct
t4entry {
	long double x;	/* Input */
	long double y;	/* Input */
	long double z;	/* Output */
} t4table[] = {
/*
	If x is -1, and y is ±Inf, 1.0 shall be returned.
	For |x| < 1, if y is -Inf, +Inf shall be returned.
	For |x| > 1, if y is -Inf, +0 shall be returned.
	For |x| < 1, if y is +Inf, +0 shall be returned.
	For |x| > 1, if y is +Inf, +Inf shall be returned.
	For y an odd integer < 0, if x is -Inf, -0 shall be returned.
	For y < 0 and not an odd integer, if x is -Inf, +0 shall be returned.
	For y an odd integer > 0, if x is -Inf, -Inf shall be returned.
	For y > 0 and not an odd integer, if x is -Inf, +Inf shall be returned.
	For y < 0, if x is +Inf, +0 shall be returned.
	For y > 0, if x is +Inf, +Inf shall be returned.
*/
#ifdef	INFINITY
	{ -1.0,  INFINITY, 1.0 },
	{ -1.0, -INFINITY, 1.0 },

	{  1.0 - FLT_EPSILON, -INFINITY, INFINITY },
	{ -1.0 + FLT_EPSILON, -INFINITY, INFINITY },

        {  1.0 - FLT_EPSILON, -INFINITY, INFINITY },
        { -1.0 + FLT_EPSILON, -INFINITY, INFINITY },
#endif
};

ATF_TC(test_pow4);
ATF_TC_HEAD(test_pow4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check edge cases");
}
ATF_TC_BODY(test_pow4, tc)
{
	size_t i, N;

	N = sizeof(t4table) / sizeof(t4table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpcmp_equalf(
			    powf((float)t4table[i].x,
				(float)t4table[i].y),
				(float)t4table[i].z));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_pow1);
	ATF_TP_ADD_TC(tp, test_pow2);
	ATF_TP_ADD_TC(tp, test_pow3);
	ATF_TP_ADD_TC(tp, test_pow4);

	return atf_no_error();
}
