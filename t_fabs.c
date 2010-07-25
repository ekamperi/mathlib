#include <atf-c.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"

/*
 * Test case 1 -- Basic functionality
 */
static const struct
t1entry {
	double x;       /* Input */
	double z;	/* fabs output */
} t1table[] = {
};

ATF_TC(test_fabs1);
ATF_TC_HEAD(test_fabs1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, fabs(x)) pairs");
}
ATF_TC_BODY(test_fabs1, tc)
{
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++) {
	}
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;		/* Input */
	long double y;		/* fabs() output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, +0 shall be returned */
	{  0.0, 0.0 },
	{ -0.0, 0.0 },

	/* If x is +-Inf, +Inf shall be returned */
#ifdef	INFINITY
	{  INFINITY, INFINITY },
	{ -INFINITY, INFINITY },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL, HUGE_VAL },
	{ -HUGE_VAL, HUGE_VAL },
#endif
#ifdef	HUGE_VALF
        {  HUGE_VALF, HUGE_VALF },
        { -HUGE_VALF, HUGE_VALF },
#endif
#ifdef	HUGE_VALL
        {  HUGE_VALL, HUGE_VALL },
        { -HUGE_VALL, HUGE_VALL },
#endif
};

ATF_TC(test_fabs2);
ATF_TC_HEAD(test_fabs2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_fabs2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpreal_equalf(
			    fabsf((float)t2table[i].x),
				  (float)t2table[i].y));

		/* double */
                ATF_CHECK(fpreal_equal(
			    fabs((double)t2table[i].x),
				 (double)t2table[i].y));

		/* long double */
#ifdef	HAVE_FABSL
                ATF_CHECK(fpreal_equall(
			    fabsl(t2table[i].x),
				 t2table[i].y));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fabs1);
	ATF_TP_ADD_TC(tp, test_fabs2);

	return atf_no_error();
}
