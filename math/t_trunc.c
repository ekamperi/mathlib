#include <atf-c.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1 -- Basic functionality
 */
static const struct
t1entry {
	double x;	/* Input */
	double y;	/* Output */
} t1table[] = {
/* XXX: Add more cases */
	{  1.1,  1.0  },
	{ -1.1, -1.0  },

	{  1.01,  1.0 },
	{ -1.01, -1.0 }
};

ATF_TC(test_trunc1);
ATF_TC_HEAD(test_trunc1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, trunc(x)) pairs");
}
ATF_TC_BODY(test_trunc1, tc)
{
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(trunc(t1table[i].x) == t1table[i].y);
}

/*
 * Test case 2 -- Some fuzzing
 */
ATF_TC(test_trunc2);
ATF_TC_HEAD(test_trunc2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Use random input");
}
ATF_TC_BODY(test_trunc2, tc)
{
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	/*
	 * These functions shall round their argument to the integer value, in
	 * floating format, nearest to but no larger in magnitude than the
	 * argument.
	 */
	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		fy = truncf(fx);
		ATF_PASS_OR_BREAK(floorf(fy) == fy);
		ATF_PASS_OR_BREAK(ceilf(fy) == fy);
		ATF_PASS_OR_BREAK(fabsf(fx) >= fabsf(fy));

		/* double */
		dx = random_double(FP_NORMAL);
		dy = trunc(dx);
		ATF_PASS_OR_BREAK(floor(dy) == dy);
		ATF_PASS_OR_BREAK(ceil(dy) == dy);
		ATF_PASS_OR_BREAK(fabs(dx) >= fabs(dy));

		/* long double */
#ifdef	HAVE_TRUNCL
		ldx = random_long_double(FP_NORMAL);
		ldy = truncl(ldx);
		ATF_PASS_OR_BREAK(floorl(ldy) == ldy);
		ATF_PASS_OR_BREAK(ceill(ldy) == ldy);
		ATF_PASS_OR_BREAK(fabsl(ldx) >= fabsl(ldy));
#endif
	}
}

/*
 * Test case 3 -- Edge cases
 */
static const struct
t3entry {
	long double x;	/* Input */
	long double y;	/* trunc() output */
} t3table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0 or +-Inf, x shall be returned */
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

ATF_TC(test_trunc3);
ATF_TC_HEAD(test_trunc3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_trunc3, tc)
{
	float fy;
	double dy;
	long double ldy;
	size_t i, N;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		fy = truncf(t3table[i].x);
		ATF_CHECK(fpreal_equal(fy, (float)t3table[i].y));

		/* double */
		dy = trunc(t3table[i].x);
		ATF_CHECK(fpreal_equal(dy, (double)t3table[i].y));

		/* long double */
#ifdef	HAVE_TRUNCL
		ldy = truncl(t3table[i].x);
		ATF_CHECK(fpreal_equall(ldy, t3table[i].y));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_trunc1);
	ATF_TP_ADD_TC(tp, test_trunc2);
	ATF_TP_ADD_TC(tp, test_trunc3);

	return atf_no_error();
}
