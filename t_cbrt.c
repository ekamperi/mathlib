#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "subr_fpcmp.h"

/*
 * Test case 1 -- Basic functionality
 */
struct tentry {
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
ATF_TC(test_cbrt2);
ATF_TC_HEAD(test_cbrt2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_cbrt2, tc)
{
	double x;

#ifdef	NAN
	/* If x is NaN, a NaN shall be returned */
	ATF_CHECK(fpclassify(cbrt(NAN)) == FP_NAN);
#endif

	/* If x is +-0, x shall be returned */
	x = +0.0;
	ATF_CHECK(fpclassify(cbrt(x)) == FP_ZERO);
	ATF_CHECK(signbit(cbrt(x)) == 0);

	x = -0.0;
	ATF_CHECK(fpclassify(cbrt(x)) == FP_ZERO);
	ATF_CHECK(signbit(cbrt(x)) != 0);

#ifdef	INFINITY
	/* If x is +-Inf, x shall be returned */
	x = INFINITY;
	ATF_CHECK(fpclassify(cbrt(x)) == FP_INFINITE);
	ATF_CHECK(signbit(cbrt(x)) == 0);

	x = -INFINITY;
	ATF_CHECK(fpclassify(cbrt(x)) == FP_INFINITE);
	ATF_CHECK(signbit(cbrt(x)) != 0);
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_cbrt1);
	ATF_TP_ADD_TC(tp, test_cbrt2);

	return atf_no_error();
}
