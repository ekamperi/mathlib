#define _XOPEN_SOURCE 600

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <atf-c.h>

struct tentry {
	double x;	/* Input */
	double exp;	/* Input */
	double z;       /* ldexp() output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_ldexp1);
ATF_TC_HEAD(test_ldexp1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, ldexp(x)) pairs");
}
ATF_TC_BODY(test_ldexp1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fabs(ldexp(ttable[i].x, ttable[i].exp) - ttable[i].z)
		    < 1E-5);
	}
}

/*
 * Test case 2 -- Edge cases
 */
#define CHK_REG         (1 << 0)
#define CHK_ZERO        (1 << 1)
#define CHK_NAN		(1 << 2)
#define	CHK_INF		(1 << 3)
#define CHK_SIGN        (1 << 4)

#define MY_NAN		1
#define MY_INF		2
#define DOESNT_MATTER	3

struct t2entry {
	double x;       /* Input */
	double exp;	/* Input */
	double z;       /* ldexp() output */
	int check;
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
	{ MY_NAN,    0, DOESNT_MATTER, CHK_NAN },
	{ MY_NAN,    1, DOESNT_MATTER, CHK_NAN },
	{ MY_NAN,    2, DOESNT_MATTER, CHK_NAN },
	{ MY_NAN,   10, DOESNT_MATTER, CHK_NAN },
	{ MY_NAN,  100, DOESNT_MATTER, CHK_NAN },
	{ MY_NAN, 1000, DOESNT_MATTER, CHK_NAN },
	{ MY_NAN, 1E50, DOESNT_MATTER, CHK_NAN },

	/* If x is +-0 or +-Inf, x shall be returned */
	{ +0.0,    0, +0.0, CHK_ZERO | CHK_SIGN },
	{ +0.0,    1, +0.0, CHK_ZERO | CHK_SIGN },
	{ +0.0,    2, +0.0, CHK_ZERO | CHK_SIGN },
	{ +0.0,   10, +0.0, CHK_ZERO | CHK_SIGN },
	{ +0.0,  100, +0.0, CHK_ZERO | CHK_SIGN },
	{ +0.0, 1000, +0.0, CHK_ZERO | CHK_SIGN },
	{ +0.0, 1E50, +0.0, CHK_ZERO | CHK_SIGN },

	{ -0.0,    0, -0.0, CHK_ZERO | CHK_SIGN },
	{ -0.0,    1, -0.0, CHK_ZERO | CHK_SIGN },
	{ -0.0,    2, -0.0, CHK_ZERO | CHK_SIGN },
	{ -0.0,   10, -0.0, CHK_ZERO | CHK_SIGN },
	{ -0.0,  100, -0.0, CHK_ZERO | CHK_SIGN },
	{ -0.0, 1000, -0.0, CHK_ZERO | CHK_SIGN },
	{ -0.0, 1E50, -0.0, CHK_ZERO | CHK_SIGN },

#ifdef	INFINITY
	{ +INFINITY,    0, INFINITY, CHK_INF | CHK_SIGN },
        { +INFINITY,    1, INFINITY, CHK_INF | CHK_SIGN },
        { +INFINITY,    2, INFINITY, CHK_INF | CHK_SIGN },
        { +INFINITY,   10, INFINITY, CHK_INF | CHK_SIGN },
        { +INFINITY,  100, INFINITY, CHK_INF | CHK_SIGN },
        { +INFINITY, 1000, INFINITY, CHK_INF | CHK_SIGN },
	{ +INFINITY, 1E50, INFINITY, CHK_INF | CHK_SIGN },

        { -INFINITY,    0, -INFINITY, CHK_INF | CHK_SIGN },
        { -INFINITY,    1, -INFINITY, CHK_INF | CHK_SIGN },
        { -INFINITY,    2, -INFINITY, CHK_INF | CHK_SIGN },
        { -INFINITY,   10, -INFINITY, CHK_INF | CHK_SIGN },
        { -INFINITY,  100, -INFINITY, CHK_INF | CHK_SIGN },
        { -INFINITY, 1000, -INFINITY, CHK_INF | CHK_SIGN },
	{ -INFINITY, 1E50, -INFINITY, CHK_INF | CHK_SIGN },
#endif

	/* If exp is 0, x shall be returned */
        {   0.0, 0,    0.0, CHK_REG },
        {   0.1, 0,    0.1, CHK_REG },
        {   0.2, 0,    0.2, CHK_REG },
        {   0.4, 0,    0.4, CHK_REG },
        {   0.8, 0,    0.8, CHK_REG },
        {  1E-5, 0,   1E-5, CHK_REG },
        { 1E-10, 0,  1E-10, CHK_REG },
	{ 1E-50, 0,  1E-50, CHK_REG },

	{    0, 0,    0, CHK_REG },
        {    1, 0,    1, CHK_REG },
	{    2, 0,    2, CHK_REG },
	{    4, 0,    4, CHK_REG },
        {    8, 0,    8, CHK_REG },
        {  100, 0,  100, CHK_REG },
	{ 1000, 0, 1000, CHK_REG },
	{ 1E10, 0, 1E10, CHK_REG },
	{ 1E50, 0, 1E50, CHK_REG }
};

ATF_TC(test_ldexp2);
ATF_TC_HEAD(test_ldexp2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_ldexp2, tc)
{
	size_t i, N;
	double oval;	/* output value */

	N = sizeof(t2table) / sizeof(t2table[0]);
	for (i = 0; i < N; i++) {
		/* Make sure that only allowed checks are set */
		ATF_REQUIRE((t2table[i].check
			& ~(CHK_REG | CHK_ZERO | CHK_NAN | CHK_INF | CHK_SIGN))
		    == 0);

		/* Don't allow conflicting types to be set */
		ATF_REQUIRE((t2table[i].check & (CHK_REG | CHK_NAN))
		    != (CHK_REG | CHK_NAN));
                ATF_REQUIRE((t2table[i].check & (CHK_REG | CHK_INF))
                    != (CHK_REG | CHK_INF));

		ATF_REQUIRE((t2table[i].check & (CHK_ZERO | CHK_NAN))
		    != (CHK_ZERO | CHK_NAN));
		ATF_REQUIRE((t2table[i].check & (CHK_ZERO | CHK_INF))
                    != (CHK_ZERO | CHK_INF));

		ATF_REQUIRE((t2table[i].check & (CHK_NAN | CHK_INF))
                    != (CHK_NAN | CHK_INF));

		/*
		 * Override MY_NAN with NAN
		 * Note: If the implementation does not support quiet NaNs, zero will be
		 * returned. Check against that, as we don't want it.
		 */
		if (t2table[i].exp == MY_NAN) {
			double tmp = nan("");
			if (fpclassify(tmp) != FP_NAN) {
				continue;
				/* NEVER REACHED */
			}

			/* Override MY_NAN with NAN */
			t2table[i].exp = tmp;
		}

		/* Ready to go */
		oval = ldexp(t2table[i].x, t2table[i].exp);
		if (t2table[i].check & CHK_REG) {
			ATF_CHECK(fabs(oval - t2table[i].z) < 1E-5);
		}
		if (t2table[i].check & CHK_ZERO) {
			ATF_CHECK(fpclassify(oval) == FP_ZERO);
		}
		if (t2table[i].check & CHK_INF) {
			/*
			 * Grab the opportunity to cross-check the results and
			 * at the same time cross-check the functions with
			 * respect to each other.
			 */
			ATF_CHECK(isinf(oval));
			ATF_CHECK(!isfinite(oval));
			ATF_CHECK(fpclassify(oval) == FP_INFINITE);
		}
		if (t2table[i].check & CHK_SIGN) {
			ATF_CHECK(signbit(oval) == signbit(t2table[i].z));
		}
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_ldexp1);
	ATF_TP_ADD_TC(tp, test_ldexp2);

	return atf_no_error();
}
