#include <atf-c.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_tanh.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_tanh1);
ATF_TC_HEAD(test_tanh1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, tanh(x)) pairs");
}
ATF_TC_BODY(test_tanh1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	for (i = 0; i < N; i++)
	  ATF_CHECK(fpreal_equal(
		      tanh(t1dtable[i].x),
			   t1dtable[i].y));

	/* long double */
#ifdef  HAVE_TANHL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equall(
			    tanhl(t1ldtable[i].x),
				  t1ldtable[i].y));
#endif
}

/*
 * Test case 2 -- Range
 */
ATF_TC(test_tanh2);
ATF_TC_HEAD(test_tanh2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "The range of tanh function is [-1, +1]");
}
ATF_TC_BODY(test_tanh2, tc)
{
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		fy = tanhf(fx);
		ATF_PASS_OR_BREAK(fy >= -1.0 && fy <= 1.0);

		/* double */
		dx = random_double(FP_NORMAL);
		dy = tanh(dx);
		ATF_PASS_OR_BREAK(dy >= -1.0 && dy <= 1.0);

		/* long double */
#ifdef	HAVE_TANHL
		ldx = random_long_double(FP_NORMAL);
		ldy = tanhl(FP_NORMAL);
		ATF_PASS_OR_BREAK(ldy >= -1.0 && ldy <= 1.0);
#endif
	}
}

/*
 * Test case 3 -- Edge cases
 */
static const struct
t3entry {
	long double x;	/* Input */
	long double y;	/* tanh() output */
} t3table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, x shall be returned */
	{ +0.0, +0.0 },
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
#ifdef	HUGE_VALF
	{  HUGE_VALF,  1.0 },
	{ -HUGE_VALF, -1.0 },
#endif
#ifdef	HUGE_VALL
	{  HUGE_VALL,  1.0 },
	{ -HUGE_VALL, -1.0 },
#endif

	/* If x is subnormal, [...] x should be returned */
};

ATF_TC(test_tanh3);
ATF_TC_HEAD(test_tanh3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
 ATF_TC_BODY(test_tanh3, tc)
{
	float fy;
	double dy;
	long double ldy;
	size_t i, N;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		fy = tanhf(t3table[i].x);
		ATF_CHECK(fpreal_equal(fy, t3table[i].y));

		/* double */
		dy = tanh(t3table[i].x);
		ATF_CHECK(fpreal_equal(fy, t3table[i].y));

		/* long double */
#ifdef	HAVE_TANHL
		ldy = tanhl(t3table[i].y);
		ATF_CHECK(fpreal_equall(ldy, t3table[i].y));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_tanh1);
	ATF_TP_ADD_TC(tp, test_tanh2);
	ATF_TP_ADD_TC(tp, test_tanh3);

	return atf_no_error();
}
