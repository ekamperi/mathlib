#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "subr_random.h"

struct tentry {
	double y;	/* Input */
	double x;	/* tanh() output */
} ttable[] = {
};

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

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
	}
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
	size_t i;
	double x, y;

	for (i = 0; i < 1000; i++) {
		x = random_double(FP_NORMAL);
		y = tanh(x);

		ATF_CHECK(y >= -1.0 && y <= 1.0);
	}
}

/*
 * Test case 3 -- Edge cases
 */
#define	CHK_REG		(1 << 0)
#define CHK_ZERO        (1 << 1)
#define CHK_NAN		(1 << 2)
#define CHK_SIGN        (1 << 3)

struct t3entry {
	double x;       /* Input */
	double y;	/* tanh() output */
	int check;
} t3table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN, CHK_NAN },
#endif

	/* If x is +-0, x shall be returned */
	{ +0.0, +0.0, CHK_ZERO | CHK_SIGN },
	{ -0.0, -0.0, CHK_ZERO | CHK_SIGN },

	/* If x is +-Inf, +-1 shall be returned */
#ifdef	INFINITY
	{  INFINITY,  1.0, CHK_REG },
	{ -INFINITY, -1.0, CHK_REG },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL,  1.0, CHK_REG },
	{ -HUGE_VAL, -1.0, CHK_REG },
#endif
#ifdef	HUGE_VALF
	{  HUGE_VALF,  1.0, CHK_REG },
	{ -HUGE_VALF, -1.0, CHK_REG }, 
#endif
#ifdef	HUGE_VALL
	{  HUGE_VALL,  1.0, CHK_REG },
	{ -HUGE_VALL, -1.0, CHK_REG },
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
	size_t i, N;
	double oval;	/* output value */

	N = sizeof(t3table) / sizeof(t3table[0]);
	for (i = 0; i < N; i++) {
		/* Make sure that only allowed checks are set */
		ATF_REQUIRE((t3table[i].check
			& ~(CHK_REG | CHK_ZERO | CHK_NAN | CHK_SIGN)) == 0);

		/* Don't allow conflicting types to be set */
                ATF_REQUIRE((t3table[i].check & (CHK_REG | CHK_NAN))
                    != (CHK_REG | CHK_NAN));
                ATF_REQUIRE((t3table[i].check & (CHK_ZERO | CHK_NAN))
                    != (CHK_ZERO | CHK_NAN));

		/* Ready to go */
		oval = tanh(t3table[i].x);
		if (t3table[i].check & CHK_REG) {
			ATF_CHECK(fabs(oval - t3table[i].y) < 1E-5);
		}
		if (t3table[i].check & CHK_ZERO) {
			ATF_CHECK(fpclassify(oval) == FP_ZERO);
		}
		if (t3table[i].check & CHK_SIGN) {
			ATF_CHECK(signbit(oval) == signbit(t3table[i].y));
		}
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
