#include <atf-c.h>
#include <math.h>
#include <stdio.h>

#include "subr_atf.h"
#include "subr_random.h"

struct tentry {
	long double x;       /* Input */
	long double y;       /* signbit output */
} ttable[] = {
	/* Some special cases */
	{  0.0, 0 },
	{ -0.0, 1 }
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_signbit1);
ATF_TC_HEAD(test_signbit1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, signbit(x)) pairs");
}
ATF_TC_BODY(test_signbit1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(         signbit(ttable[i].x)  == ttable[i].y);
		ATF_CHECK(((float )signbit(ttable[i].x)) == ttable[i].y);
                ATF_CHECK(((double)signbit(ttable[i].x)) == ttable[i].y);
	}
}

/*
 * Test case 2 -- Here comes the fuzzing
 */
ATF_TC(test_signbit2);
ATF_TC_HEAD(test_signbit2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "XXX");
}
ATF_TC_BODY(test_signbit2, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		ATF_CHECK(
			(fx >= 0.0 && !signbit(fx)) ||
			(fx <  0.0 &&  signbit(fx)));

		/* double */
		dx = random_double(FP_NORMAL);
		ATF_CHECK(
			(dx >= 0.0 && !signbit(dx)) ||
			(dx <  0.0 &&  signbit(dx)));

		/* long double */
		ldx = random_long_double(FP_NORMAL);
		ATF_CHECK(
			(ldx >= 0.0 && !signbit(ldx)) ||
			(ldx <  0.0 &&  signbit(ldx)));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_signbit1);
	ATF_TP_ADD_TC(tp, test_signbit2);

	return atf_no_error();
}
