#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <float.h>	/* {FLT, DBL, LDLBL}_MAX */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1 -- Basic functionality
 */
static const struct
t1entry {
	double x;       /* Input */
	double y;       /* round output */
} t1table[] = {
};

ATF_TC(test_round1);
ATF_TC_HEAD(test_round1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, round(x)) pairs");
}
ATF_TC_BODY(test_round1, tc)
{
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;
	long double y;
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0 or +-Inf, x shall be returned */
	{  0.0,  0.0 },
	{ -0.0, -0.0 },

#ifdef  INFINITY
	{  INFINITY,  INFINITY },
	{ -INFINITY, -INFINITY },
#endif
#ifdef  HUGE_VAL
	{  HUGE_VAL,  HUGE_VAL },
	{ -HUGE_VAL, -HUGE_VAL },
#endif
#ifdef  HUGE_VALF
	{  HUGE_VALF,  HUGE_VALF },
	{ -HUGE_VALF, -HUGE_VALF },
#endif
#ifdef  HUGE_VALL
	{  HUGE_VALL,  HUGE_VALL },
	{ -HUGE_VALL, -HUGE_VALL }
#endif
};

ATF_TC(test_round2);
ATF_TC_HEAD(test_round2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_round2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpreal_equalf(
			    roundf((float)t2table[i].x),
				   (float)t2table[i].y));

		/* double */
		ATF_CHECK(fpreal_equal(
			    round((double)t2table[i].x),
				  (double)t2table[i].y));

		/* long double */
#ifdef	HAVE_ROUNDL
		ATF_CHECK(fpreal_equall(
			    roundl(t2table[i].x),
				   t2table[i].y));
#endif
	}
}

/*
 * Test case 3
 */
ATF_TC(test_round3);
ATF_TC_HEAD(test_round3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check for spurious errors");
}
ATF_TC_BODY(test_round3, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float_range_ex(FP_NORMAL, -FLT_MAX, FLT_MAX);
		errno = 0;
		clear_exceptions();
		(void)roundf(fx);
		ATF_PASS_OR_BREAK(not_errno_equalto(ERANGE));
		ATF_PASS_OR_BREAK(not_raised_exceptions(MY_FE_OVERFLOW));

		/* double */
		dx = random_double_range_ex(FP_NORMAL, -DBL_MAX, DBL_MAX);
		errno = 0;
		clear_exceptions();
		(void)round(dx);
		ATF_PASS_OR_BREAK(not_errno_equalto(ERANGE));
		ATF_PASS_OR_BREAK(not_raised_exceptions(MY_FE_OVERFLOW));

		/* long double */
#ifdef	HAVE_ROUNDL
		ldx = random_long_double_range_ex(FP_NORMAL, -LDBL_MAX, LDBL_MAX);
		errno = 0;
		clear_exceptions();
		(void)roundl(ldx);
		ATF_PASS_OR_BREAK(not_errno_equalto(ERANGE));
		ATF_PASS_OR_BREAK(not_raised_exceptions(MY_FE_OVERFLOW));
#endif
	}
}

/*
 * Test case 4
 * XXX: Try to change rounding modes, if host supports fenv.h
 */
ATF_TC(test_round4);
ATF_TC_HEAD(test_round4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Rounding of halfway cases happens away from zero, "
	    "regardless of the current rounding direction");
}
ATF_TC_BODY(test_round4, tc)
{
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	srand48(time(NULL));

	ATF_FOR_LOOP(i, N, i++) {
		/* double */
		dx = mrand48() + 0.5;
		dy = round(dx);
		ATF_PASS_OR_BREAK(fabs(dy) > fabs(dx));

		/* long double */
#if defined(HAVE_ROUNDL) && defined(HAVE_FABSL)
		ldx = mrand48() + 0.5;
		ldy = roundl(ldx);
		ATF_PASS_OR_BREAK(fabsl(ldy) > fabsl(ldx));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_round1);
	ATF_TP_ADD_TC(tp, test_round2);
	ATF_TP_ADD_TC(tp, test_round3);
	ATF_TP_ADD_TC(tp, test_round4);

	return atf_no_error();
}
