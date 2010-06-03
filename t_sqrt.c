#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "subr_fpcmp.h"

struct tentry {
	double x;       /* Input */
	double y;       /* sqrt output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_sqrt1);
ATF_TC_HEAD(test_sqrt1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, sqrt(x)) pairs");
}
ATF_TC_BODY(test_sqrt1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		/* Sanity check */
		ATF_REQUIRE(ttable[i].x >= 0.0);

		/* Actual check */
		ATF_CHECK(fpcmp_equal(sqrt(ttable[i].x), ttable[i].y));
	}
}

/*
 * Test case 2 -- Edge cases
 */
struct t2entry {
	long double x;
	long double y;
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef  NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, x shall be returned */
	{  0.0,  0.0 },
	{ -0.0, -0.0 },

        /* If x is +Inf, x shall be returned */
#ifdef  INFINITY
	{ INFINITY, INFINITY },
#endif
#ifdef	HUGE_VAL
	{ HUGE_VAL, HUGE_VAL },
#endif
#ifdef	HUGE_VALF
	{ HUGE_VALF, HUGE_VALF },
#endif
#ifdef	HUGE_VALL
	{ HUGE_VALL, HUGE_VALL },
#endif
};

ATF_TC(test_sqrt2);
ATF_TC_HEAD(test_sqrt2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_sqrt2, tc)
{
	float fy;
	double dy;
	long double ldy;
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);

	for (i = 0; i < N; i++) {
		/* float */
		fy = sqrtf(t2table[i].x);
		ATF_CHECK(fpcmp_equal(fy, (float)t2table[i].y));

		/* double */
		dy = sqrt(t2table[i].x);
		ATF_CHECK(fpcmp_equal(dy, (double)t2table[i].y));

		/* long double */
		ldy = sqrtl(t2table[i].x);
		ATF_CHECK(fpcmp_equal(ldy, t2table[i].y));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_sqrt1);
	ATF_TP_ADD_TC(tp, test_sqrt2);

	return atf_no_error();
}
