#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "subr_atf.h"
#include "subr_fpcmp.h"

/*
 * Test case 1 -- Basic functionality
 */
struct tentry {
  double x;       /* Input */
  double y;       /* erf output */
} ttable[] = {
};

ATF_TC(test_erf1);
ATF_TC_HEAD(test_erf1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, erf(x)) pairs");
}
ATF_TC_BODY(test_erf1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
	}
}

/*
 * Test case 2 -- Edge cases
 */
struct t2entry {
	long double x;	/* Input */
	long double y;	/* erf output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, +-0 shall be returned */
	{  0.0,  0.0 },
	{ -0.0, -0.0 },

	/* If x is +-Inf, +-1 shall be returned */
#ifdef	INFINITY
	{  INFINITY,  1.0 },
	{ -INFINITY, -1.0 },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL,  1.0 },
	{ -HUGE_VAL, -1.0 },
#endif
#ifdef  HUGE_VALF
	{  HUGE_VALF,  1.0 },
	{ -HUGE_VALF, -1.0 },
#endif
#ifdef  HUGE_VALL
	{  HUGE_VAL,  1.0 },
	{ -HUGE_VAL, -1.0 }
#endif
};

ATF_TC(test_erf2);
ATF_TC_HEAD(test_erf2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_erf2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equal(
			    erff((float)t2table[i].x),
			    t2table[i].y));

                ATF_CHECK(fpcmp_equal(
			    erf((double)t2table[i].x),
			    t2table[i].y));

                ATF_CHECK(fpcmp_equal(
			    erfl(t2table[i].x),
			    t2table[i].y));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_erf1);
	ATF_TP_ADD_TC(tp, test_erf2);

	return atf_no_error();
}
