#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <limits.h>	/* LLONG_MAX */
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
	double y;       /* llround output */
} t1table[] = {
};

ATF_TC(test_llround1);
ATF_TC_HEAD(test_llround1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, llround(x)) pairs");
}
ATF_TC_BODY(test_llround1, tc)
{
}

/*
 * Test case 2 -- Domain errors
 */
static const long double
t2table[] = {
	/*
	 * If x is NaN, a domain error shall occur and
	 * an unspecified value is returned
	 */
#ifdef	NAN
	NAN,
#endif

	/*
	 * If x is +-Inf, a domain error shall occur
	 * and an unspecified value is returned
	 */
#ifdef  INFINITY
	INFINITY,
	-INFINITY,
#endif
#ifdef  HUGE_VAL
	HUGE_VAL,
	-HUGE_VAL,
#endif
#ifdef  HUGE_VALF
	HUGE_VALF,
	-HUGE_VALF,
#endif
#ifdef  HUGE_VALL
	HUGE_VALL,
	-HUGE_VALL
#endif

	/*
	 * If the correct value is positive and too large to represent
	 * as a long, a domain error shall occur and an unspecified
	 * value is returned
	 */
	+LLONG_MAX + 1,

	/*
	 * If the correct value is negative and too large to represent
	 * as a long, a domain error shall occur and an unspecified
	 * value is returned
	 */
	-LLONG_MAX - 1
};

ATF_TC(test_llround2);
ATF_TC_HEAD(test_llround2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check that domain errors are properly generated");
}
ATF_TC_BODY(test_llround2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		(void)llroundf((float)t2table[i]);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		(void)llround((double)t2table[i]);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_LLROUNDL
		errno = 0;
		clear_exceptions();
		(void)llroundl(t2table[i]);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
	}
}

/*
 * Test case 3
 */
ATF_TC(test_llround3);
ATF_TC_HEAD(test_llround3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check for spurious errors");
}
ATF_TC_BODY(test_llround3, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float_range_in(FP_NORMAL, -LLONG_MAX, LLONG_MAX);
		errno = 0;
		clear_exceptions();
		(void)llroundf(fx);
		ATF_PASS_OR_BREAK(not_errno_equalto(EDOM));
		ATF_PASS_OR_BREAK(not_raised_exceptions(MY_FE_INVALID));

		/* double */
		dx = random_double_range_in(FP_NORMAL, -LLONG_MAX, LLONG_MAX);
		errno = 0;
		clear_exceptions();
		(void)llround(dx);
		ATF_PASS_OR_BREAK(not_errno_equalto(EDOM));
		ATF_PASS_OR_BREAK(not_raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_LLROUNDL
		ldx = random_long_double_range_in(FP_NORMAL,
						  -LLONG_MAX, LLONG_MAX);
		errno = 0;
		clear_exceptions();
		(void)llroundl(ldx);
		ATF_PASS_OR_BREAK(not_errno_equalto(EDOM));
		ATF_PASS_OR_BREAK(not_raised_exceptions(MY_FE_INVALID));
#endif
	}
}

/*
 * Test case 4
 * XXX: Try to change rounding modes, if host supports fenv.h
 */
ATF_TC(test_llround4);
ATF_TC_HEAD(test_llround4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Rounding of halfway cases happens away from zero, "
	    "regardless of the current rounding direction");
}
ATF_TC_BODY(test_llround4, tc)
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
		dy = llround(dx);
		ATF_PASS_OR_BREAK(fabs(dy) > fabs(dx));

		/* long double */
#if defined(HAVE_LLROUNDL) && defined(HAVE_FABSL)
		ldx = mrand48() + 0.5;
		ldy = llroundl(ldx);
		ATF_PASS_OR_BREAK(fabsl(ldy) > fabsl(ldx));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_llround1);
	ATF_TP_ADD_TC(tp, test_llround2);
	ATF_TP_ADD_TC(tp, test_llround3);
	ATF_TP_ADD_TC(tp, test_llround4);

	return atf_no_error();
}
