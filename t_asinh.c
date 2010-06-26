#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "subr_fpcmp.h"

static const struct
tentry {
	long double x;	/* Input */
	long double y;	/* asinh output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_asinh1);
ATF_TC_HEAD(test_asinh1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, asinh(x)) pairs");
}
ATF_TC_BODY(test_asinh1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equal(asinh(ttable[i].x), ttable[i].y));
	}
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;       /* Input */
	long double y;       /* asinh output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, or +-Inf, x shall be returned */
	{  0.0,  0.0 },
	{ -0.0, -0.0 },

#ifdef	INFINITY
	{  INFINITY,  INFINITY },
	{ -INFINITY, -INFINITY },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL,  HUGE_VAL },
	{ -HUGE_VAL, -HUGE_VAL },
#endif
#ifdef	HUGE_VALF
	{  HUGE_VALF,  HUGE_VALF },
	{ -HUGE_VALF, -HUGE_VALF },
#endif
#ifdef	HUGE_VALL
	{  HUGE_VALL,  HUGE_VALL },
	{ -HUGE_VALL, -HUGE_VALL }
#endif
};

ATF_TC(test_asinh2);
ATF_TC_HEAD(test_asinh2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_asinh2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpcmp_equalf(
			    asinhf((float)t2table[i].x),
				(float)t2table[i].y));

		/* double */
		ATF_CHECK(fpcmp_equal(
			    asinh((double)t2table[i].x),
				  (double)t2table[i].y));

		/* long double */
#ifdef	HAVE_ASINHL
		ATF_CHECK(fpcmp_equall(
			    asinhl(t2table[i].x),
				   t2table[i].y));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_asinh1);
	ATF_TP_ADD_TC(tp, test_asinh2);

	return atf_no_error();
}
