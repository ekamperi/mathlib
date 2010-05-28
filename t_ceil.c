#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "subr_fpcmp.h"
#include "subr_random.h"

struct tentry {
	double x;       /* Input */
	double y;       /* ceil output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_ceil1);
ATF_TC_HEAD(test_ceil1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, ceil(x)) pairs");
}
ATF_TC_BODY(test_ceil1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
	}
}

/*
 * Test case 2 -- A little bit fuzzing
 */
ATF_TC(test_ceil2);
ATF_TC_HEAD(test_ceil2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Result is the smallest integral value _not less_ than x");
}
ATF_TC_BODY(test_ceil2, tc)
{
	size_t i;
	float fx;
	double dx;
	long double ldx;

	for (i = 0; i < 1000000; i++) {
		fx = random_float(FP_NORMAL);
		ATF_CHECK(ceil(fx) >= fx);

		dx = random_double(FP_NORMAL);
		ATF_CHECK(ceil(dx) >= dx);

		ldx = random_double(FP_NORMAL);
		ATF_CHECK(ceil(ldx) >= ldx);
	}
}

/*
 * Test case 3 -- Edge cases
 */
struct t3entry {
	long double x;
	long double y;
} t3table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif
	/* If x is +-0 or +-Inf, x shall be returned */
	{ 0.0,  0.0 },
	{-0.0, -0.0 },

#ifdef	IFINITIY
	{  INFINITY,  INFINITY },
	{ -INFINITY, -INFINITY },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL,  HUGE_VAL },
	{ -HUGE_VAL, -HUGE_VAL },
#endif
#ifdef  HUGE_VALF
	{  HUGE_VALF,  HUGE_VALF },
	{ -HUGE_VALF, -HUGE_VALF },
#endif
#ifdef  HUGE_VALL
	{  HUGE_VALL,  HUGE_VALL },
	{ -HUGE_VALL, -HUGE_VALL },
#endif
};

ATF_TC(test_ceil3);
ATF_TC_HEAD(test_ceil3, tc)
{
        atf_tc_set_md_var(tc,
            "descr",
            "Check some edge cases");
}
ATF_TC_BODY(test_ceil3, tc)
{
	size_t i, N;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equal(ceil(t3table[i].x), t3table[i].y));
	}
}

/*
 * Test case 4 -- Edge error condition
 */
ATF_TC(test_ceil4);
ATF_TC_HEAD(test_ceil4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check an edge error condition");
}
ATF_TC_BODY(test_ceil4, tc)
{
	/*
	 * If the correct value would cause overflow, a range error shall occur
	 * and ceil(), ceilf(), and ceill() shall return the value of the macro
	 * HUGE_VAL, HUGE_VALF, and HUGE_VALL, respectively.
	 *
	 * The ceil() function can only overflow when the floating-point
	 * representation has DBL_MANT_DIG > DBL_MAX_EXP. This is very unusual,
	 * so just check against it and report.
	 */
	if (DBL_MANT_DIG > DBL_MAX_EXP) {
		atf_tc_fail("The impossible has happened!"
		    "(DBL_MANT_DIG: %d, DBL_MAX_EXP: %d)",
		    DBL_MANT_DIG,
		    DBL_MAX_EXP);
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_ceil1);
	ATF_TP_ADD_TC(tp, test_ceil2);
	ATF_TP_ADD_TC(tp, test_ceil3);
	ATF_TP_ADD_TC(tp, test_ceil4);

	return atf_no_error();
}
