#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

struct tentry {
	double x;       /* Input */
	double y;       /* floor output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_floor1);
ATF_TC_HEAD(test_floor1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, floor(x)) pairs");
}
ATF_TC_BODY(test_floor1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
	}
}

/*
 * Test case 2 -- A little bit fuzzing
 */
ATF_TC(test_floor2);
ATF_TC_HEAD(test_floor2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Result is the smallest _integral value_ _not greater_ than x");
}
ATF_TC_BODY(test_floor2, tc)
{
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		fy = floorf(fx);
		ATF_PASS_OR_BREAK(fy <= fx);
		ATF_PASS_OR_BREAK(floorf(fy) == fy);
		ATF_PASS_OR_BREAK(floorf(fy) == fy);

		/* double */
		dx = random_double(FP_NORMAL);
		dy = floor(dx);
		ATF_PASS_OR_BREAK(dy <= dx);
		ATF_PASS_OR_BREAK(floor(dy) == dy);
		ATF_PASS_OR_BREAK(floor(dy) == dy);

		/* long double */
		ldx = random_long_double(FP_NORMAL);
		ldy = floorl(ldx);
		ATF_PASS_OR_BREAK(ldy <= ldx);
		ATF_PASS_OR_BREAK(floorl(ldy) == ldy);
		ATF_PASS_OR_BREAK(floorl(ldy) == ldy);
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

ATF_TC(test_floor3);
ATF_TC_HEAD(test_floor3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_floor3, tc)
{
	size_t i, N;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equal(floor(t3table[i].x), t3table[i].y));
}

/*
 * Test case 4 -- Edge error condition
 */
ATF_TC(test_floor4);
ATF_TC_HEAD(test_floor4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check an edge error condition");
}
ATF_TC_BODY(test_floor4, tc)
{
	/*
	 * If the correct value would cause overflow, a range error shall occur
	 * and floor(), floorf(), and floorl() shall return the value of the macro
	 * HUGE_VAL, HUGE_VALF, and HUGE_VALL, respectively.
	 *
	 * The floor() function can only overflow when the floating-point
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
	ATF_TP_ADD_TC(tp, test_floor1);
	ATF_TP_ADD_TC(tp, test_floor2);
	ATF_TP_ADD_TC(tp, test_floor3);
	ATF_TP_ADD_TC(tp, test_floor4);

	return atf_no_error();
}
