#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "subr_random.h"

/*
 * Test case 1 -- Basic functionality
 */
struct t1entry {
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
	size_t i;
	float fx, fy;
	double dx, dy;

	/*
	 * These functions shall round their argument to the integer value, in
	 * floating format, nearest to but no larger in magnitude than the
	 * argument.
	 */
	for (i = 0; i < 1000000; i++) {
		/* double */
		dx = random_double(FP_NORMAL);
		dy = trunc(dx);
		ATF_CHECK(floor(dy) == dy);
		ATF_CHECK(ceil(dy) == dy);
		ATF_CHECK(fabs(dx) >= fabs(dy));

		/* float */
		fx = random_float(FP_NORMAL);
		fy = truncf(fx);
		ATF_CHECK(floor(fy) == fy);
                ATF_CHECK(ceil(fy) == fy);
                ATF_CHECK(fabs(fx) >= fabs(fy));
	}
}

/*
 * Test case 3 -- Edge cases
 */
#define CHK_ZERO        (1 << 0)
#define CHK_NAN         (1 << 1)
#define	CHK_INF		(1 << 2)
#define CHK_SIGN        (1 << 3)

#define DOESNT_MATTER	1

struct tentry {
	double x;	/* Input */
	double y;	/* trunc() output */
	int check;
} t3table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, DOESNT_MATTER, CHK_NAN },
#endif

	/* If x is +-0 or +-Inf, x shall be returned */
	{  0.0,  0.0, CHK_ZERO | CHK_SIGN },
	{ -0.0, -0.0, CHK_ZERO | CHK_SIGN },
#ifdef	INFINITY
	{  INFINITY,  INFINITY, CHK_INF | CHK_SIGN },
	{ -INFINITY, -INFINITY, CHK_INF | CHK_SIGN },
#endif

#ifdef	HUGE_VAL
	{  HUGE_VAL,  HUGE_VAL, CHK_INF | CHK_SIGN },
	{ -HUGE_VAL, -HUGE_VAL, CHK_INF | CHK_SIGN },
#endif

#ifdef	HUGE_VALF
        {  HUGE_VALF,  HUGE_VALF, CHK_INF | CHK_SIGN },
        { -HUGE_VALF, -HUGE_VALF, CHK_INF | CHK_SIGN },
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
	size_t i, N;
	double oval;	/* Output value */

	N = sizeof(t3table) / sizeof(t3table[0]);
	for (i = 0; i < N; i++) {
                /* Make sure that only allowed checks are set */
                ATF_REQUIRE((t3table[i].check
                        & ~(CHK_ZERO | CHK_NAN | CHK_INF | CHK_SIGN)) == 0);

                /* Don't allow conflicting types to be set */
                ATF_REQUIRE((t3table[i].check & (CHK_ZERO | CHK_NAN))
                    != (CHK_ZERO | CHK_NAN));
                ATF_REQUIRE((t3table[i].check & (CHK_ZERO | CHK_INF))
                    != (CHK_ZERO | CHK_INF));
                ATF_REQUIRE((t3table[i].check & (CHK_NAN | CHK_INF))
                    != (CHK_NAN | CHK_INF));

		/* Ready to go */
		oval = trunc(t3table[i].x);

		if (t3table[i].check & CHK_ZERO) {
                        ATF_CHECK(fpclassify(oval) == FP_ZERO);
                }
		if (t3table[i].check & CHK_NAN) {
			ATF_CHECK(isnan(oval));
			ATF_CHECK(fpclassify(oval) == FP_NAN);
		}
		if (t3table[i].check & CHK_INF) {
			ATF_CHECK(isinf(oval));
			ATF_CHECK(fpclassify(oval) == FP_INFINITE);
		}
                if (t3table[i].check & CHK_SIGN) {
                        ATF_CHECK(signbit(oval) == signbit(t3table[i].y));
                }
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
