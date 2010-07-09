#include <atf-c.h>
#include <math.h>

#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_frexp1);
ATF_TC_HEAD(test_frexp1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, frexp(x)) pairs");
}
ATF_TC_BODY(test_frexp1, tc)
{
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;	/* Input */
	long double y;	/* Output */
} t2table[] = {
	/*
	 * If num is NaN, a NaN shall be returned,
	 * and the value of *exp  is unspecified.
	 */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/*
	 * If num is +-0, +-0 shall be returned,
	 * and the value of *exp shall be 0.
	 */
	{  0.0,  0.0 },
	{ -0.0, -0.0 },

	/*
	 * If num is +-Inf, num shall be returned,
	 * and the value of *exp is unspecified.
	 */
#ifdef  INFINITY
        {  INFINITY,  INFINITY },
	{ -INFINITY, -INFINITY },
#endif
#ifdef  HUGE_VAL
        {  HUGE_VAL,  HUGE_VAL },
	{ -HUGE_VAL, -HUGE_VAL },
#endif
#ifdef  HUGE_VALF
        {  HUGE_VALF,  HUGE_VALF },
	{ -HUGE_VALF, -HUGE_VALF },
#endif
#ifdef  HUGE_VALL
        {  HUGE_VALL,  HUGE_VALL },
	{ -HUGE_VALL, -HUGE_VALL }
#endif
};

ATF_TC(test_frexp2);
ATF_TC_HEAD(test_frexp2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_frexp2, tc)
{
	size_t i, N;
	int myexp;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpcmp_equalf(
			    frexpf((float)t2table[i].x, &myexp),
				   (float)t2table[i].y));

		/* double */
		ATF_CHECK(fpcmp_equal(
			    frexp((double)t2table[i].x, &myexp),
				  (double)t2table[i].y));

		/* long double */
		ATF_CHECK(fpcmp_equal(
			    frexp(t2table[i].x, &myexp),
				  t2table[i].y));
	}
}


/*
 * Test case 3
 *
 * For finite arguments, these functions shall return the value x, such that
 * x has a magnitude in the interval [½,1) or 0, and num equals x times
 * 2 raised to the power *exp.
 */
ATF_TC(test_frexp3);
ATF_TC_HEAD(test_frexp3, tc)
{
        atf_tc_set_md_var(tc,
            "descr",
            "Fuzzing");
}
ATF_TC_BODY(test_frexp3, tc)
{
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */

		/* double */

		/* long double */
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_frexp1);
	ATF_TP_ADD_TC(tp, test_frexp2);
	ATF_TP_ADD_TC(tp, test_frexp3);

	return atf_no_error();
}
