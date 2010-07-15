#include <atf-c.h>
#include <math.h>

#include "subr_atf.h"
#include "subr_random.h"

static const struct
t1entry {
	long double x;       /* Input */
	long double y;       /* signbit output */
} t1table[] = {
	/* Some special cases */
	{  0.0, 0 },
	{ -0.0, 1 },

	/* Infinities */
#ifdef  INFINITIY
        {  INFINITY, 0 },
        { -INFINITY, 1 },
#endif
#ifdef  HUGE_VAL
        {  HUGE_VAL, 0 },
        { -HUGE_VAL, 1 },
#endif
#ifdef  HUGE_VALF
        {  HUGE_VALF, 0 },
        { -HUGE_VALF, 1 },
#endif
#ifdef  HUGE_VALL
        {  HUGE_VALL, 0 },
        { -HUGE_VALL, 1 },
#endif
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

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(signbit(	  t1table[i].x) == t1table[i].y);
		ATF_CHECK(signbit((float) t1table[i].x) == t1table[i].y);
		ATF_CHECK(signbit((double)t1table[i].x) == t1table[i].y);
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

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		ATF_PASS_OR_BREAK(
			(fx >= 0.0 && !signbit(fx)) ||
			(fx <  0.0 &&  signbit(fx)));

		/* double */
		dx = random_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(
			(dx >= 0.0 && !signbit(dx)) ||
			(dx <  0.0 &&  signbit(dx)));

		/* long double */
		ldx = random_long_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(
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
