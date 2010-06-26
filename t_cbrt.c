#include <atf-c.h>
#include <math.h>
#include <stdio.h>

#include "config.h"
#include "subr_fpcmp.h"

/*
 * Test case 1 -- Basic functionality
 */
static const struct
tentry {
	double x;       /* Input */
	double y;       /* cbrt output */
} ttable[] = {
};

ATF_TC(test_cbrt1);
ATF_TC_HEAD(test_cbrt1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, cbrt(x)) pairs");
}
ATF_TC_BODY(test_cbrt1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		/* Sanity check */
		ATF_REQUIRE(ttable[i].x >= -1.0 && ttable[i].x <= 1.0);

		/* Actual checks */
		ATF_CHECK(fpcmp_equal(cbrt(ttable[i].x), ttable[i].y));
	}
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

	/* If x is +-0, x shall be returned */
	{  0.0,  0.0 },
	{ -0.0, -0.0 },

	/* If x is +-Inf, x shall be returned */
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

ATF_TC(test_cbrt2);
ATF_TC_HEAD(test_cbrt2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_cbrt2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpcmp_equalf(
			    cbrtf((float)t2table[i].x),
				  (float)t2table[i].y));

		/* double */
		ATF_CHECK(fpcmp_equalf(
			    cbrtf((double)t2table[i].x),
				  (double)t2table[i].y));

		/* long double */
#ifdef	HAVE_CBRTL
		ATF_CHECK(fpcmp_equalf(
			    cbrtf(t2table[i].x),
				  t2table[i].y));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_cbrt1);
	ATF_TP_ADD_TC(tp, test_cbrt2);

	return atf_no_error();
}
