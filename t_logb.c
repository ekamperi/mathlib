#define _XOPEN_SOURCE 600

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

#include <atf-c.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fenv.h>

static struct
tentry {
	long double x;       /* Input */
	long double y;       /* logb() output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_logb1);
ATF_TC_HEAD(test_logb1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, logb(x)) pairs");
}
ATF_TC_BODY(test_logb1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equal(logb(ttable[i].x), ttable[i].y));
}

/*
 * Test case 2 -- Pole error
 *
 * If x is +-0, a pole error shall occur and logb(), logbf(), and logbl() shall
 * return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
 */
static long t2table[] = { +0.0, -0.0 };

ATF_TC(test_logb2);
ATF_TC_HEAD(test_logb2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x is +-0, a pole error shall occur");
}
ATF_TC_BODY(test_logb2, tc)
{
	float fy;
	double dy;
	long double ldy;
	int haserrexcept;
	int haserrno;
	int i;

	for (i = 0; i < 2; i++ ) {
		/* float */
		errno = 0;
		clear_exceptions();
		fy = logbf((float)t2table[i]);
#ifdef  HUGE_VALF
		ATF_CHECK(fy == -HUGE_VALF);
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(FE_DIVBYZERO));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = logb((double)t2table[i]);
#ifdef	HUGE_VAL
		ATF_CHECK(dy == -HUGE_VAL);
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(FE_DIVBYZERO));

		/* long double */
		errno = 0;
		clear_exceptions();
		ldy = logbl(t2table[i]);
#ifdef  HUGE_VALL
		ATF_CHECK(ldy == -HUGE_VALL);
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(FE_DIVBYZERO));
	}

	/*
	 * Revenge is a Dish Best Served Cold :)
	 *
	 * We put this check here and not in the beginning of the test case,
	 * because we don't want the lack of error handling support to
	 * block the check against HUGE_VAL*. Nor do we want to split the test
	 * case into 2.
	 */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);
}

/*
 * Test case 3 -- Edge cases
 */
static
struct t3entry {
	long double x;
	long double y;
} t3table[] = {
	/* If x is NaN, a NaN shall be returned */
	{ NAN, NAN },

        /* If x is +-Inf, x shall be returned */
#ifdef  INFINITY
	{  INFINITY, INFINITY },
	{ -INFINITY, INFINITY },
#endif
#ifdef  HUGE_VAL
	{  HUGE_VAL, HUGE_VAL },
	{ -HUGE_VAL, HUGE_VAL },
#endif
#ifdef  HUGE_VALF
	{  HUGE_VALF, HUGE_VALF },
	{ -HUGE_VALF, HUGE_VALF },
#endif
#ifdef  HUGE_VALL
	{  HUGE_VALL, HUGE_VALL },
	{ -HUGE_VALL, HUGE_VALL }
#endif	
};

ATF_TC(test_logb3);
ATF_TC_HEAD(test_logb3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_logb3, tc)
{
	size_t i, N;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equalf(
			    logbf((float)t3table[i].x),
				  (float)t3table[i].y));

		ATF_CHECK(fpcmp_equal(
			    logb((double)t3table[i].x),
				 (double)t3table[i].y));

		ATF_CHECK(fpcmp_equall(
			    logbl(t3table[i].x),
				  t3table[i].y));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_logb1);
	ATF_TP_ADD_TC(tp, test_logb2);
	ATF_TP_ADD_TC(tp, test_logb3);

	return atf_no_error();
}
