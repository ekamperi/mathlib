#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "subr_fpcmp.h"

struct tentry {
	double x;       /* Input */
	double y;       /* acosh output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_acosh1);
ATF_TC_HEAD(test_acosh1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, acosh(x)) pairs");
}
ATF_TC_BODY(test_acosh1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(FPCMP_EQUAL(acosh(ttable[i].x), ttable[i].y));
	}
}

/*
 * Test case 2 -- Edge cases
 */
#define CHK_ZERO        (1 << 0)
#define CHK_NAN         (1 << 1)
#define CHK_INF		(1 << 2)
#define CHK_SIGN        (1 << 3)

struct t2entry {
	double x;       /* Input */
	double y;       /* acosh output */
	int check;
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN, CHK_NAN },
#endif

	/* If x is +1, +0 shall be returned */
	{ 1.0, +0.0, CHK_ZERO | CHK_SIGN },
	
	/* If x is +Inf, +Inf shall be returned */
#ifdef	INFINITY
	{ INFINITY, INFINITY, CHK_INF | CHK_SIGN },
#endif
#ifdef	HUGE_VALF
	{ HUGE_VALF, HUGE_VALF, CHK_INF | CHK_SIGN },
#endif
#ifdef	HUGE_VALL
	{ HUGE_VALL, HUGE_VALL, CHK_INF | CHK_SIGN }
#endif
};

ATF_TC(test_acosh2);
ATF_TC_HEAD(test_acosh2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_acosh2, tc)
{
	size_t i, N;
	double oval;    /* output value */

	N = sizeof(t2table) / sizeof(t2table[0]);
	for (i = 0; i < N; i++) {
		/* Make sure that only allowed checks are set */
		ATF_REQUIRE((t2table[i].check
			& ~(CHK_ZERO | CHK_NAN | CHK_INF | CHK_SIGN)) == 0);

		/* Don't allow conflicting types to be set */
		ATF_REQUIRE((t2table[i].check & (CHK_ZERO | CHK_NAN))
		    != (CHK_ZERO | CHK_NAN));
		ATF_REQUIRE((t2table[i].check & (CHK_ZERO | CHK_INF))
		    != (CHK_ZERO | CHK_INF));
		ATF_REQUIRE((t2table[i].check & (CHK_NAN | CHK_INF))
                    != (CHK_NAN | CHK_INF));

		/* Ready to go */
		oval = acosh(t2table[i].x);
		if (t2table[i].check & CHK_ZERO) {
			ATF_CHECK(fpclassify(oval) == FP_ZERO);
		}
		if (t2table[i].check & CHK_SIGN) {
			ATF_CHECK(signbit(oval) == signbit(t2table[i].y));
		}
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_acosh1);
	ATF_TP_ADD_TC(tp, test_acosh2);

	return atf_no_error();
}
