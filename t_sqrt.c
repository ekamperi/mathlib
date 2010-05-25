#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <atf-c.h>

struct tentry {
	double x;       /* Input */
	double y;       /* sqrt output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_sqrt1);
ATF_TC_HEAD(test_sqrt1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, sqrt(x)) pairs");
}
ATF_TC_BODY(test_sqrt1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		ATF_REQUIRE(ttable[i].x >= 0.0);

		ATF_CHECK(fabs(sqrt(ttable[i].x) - ttable[i].y) < 1E-5);
	}
}

/*
 * Test case 2 -- Edge cases
 */
ATF_TC(test_sqrt2);
ATF_TC_HEAD(test_sqrt2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_sqrt2, tc)
{
	double x;

#ifdef	NAN
	/* If x is NaN, a NaN shall be returned	*/
	ATF_CHECK(fpclassify(sqrt(x)) == FP_NAN);
#endif

	/* If x is +-0, x shall be returned */
	x = +0.0;
	ATF_CHECK(fpclassify(sqrt(x)) == FP_ZERO);
	ATF_CHECK(signbit(sqrt(x)) == 0);

	x = -0.0;
	ATF_CHECK(fpclassify(sqrt(x)) == FP_ZERO);
	ATF_CHECK(signbit(sqrt(x)) != 0);

#ifdef	INFINITY
	/* If x is +Inf, x shall be returned */
	x = INFINITY;
	ATF_CHECK(fpclassify(sqrt(x)) == FP_INFINITE);
	ATF_CHECK(signbit(sqrt(x)) == 0);
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_sqrt1);
	ATF_TP_ADD_TC(tp, test_sqrt2);

	return atf_no_error();
}
