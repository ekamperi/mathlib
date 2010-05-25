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
	double x;       /* Input */
	double y;	/* Input */
	int z;		/* fmax output (0 for x>y, 1 otherwise */
} t1table[] = {
	/* -1 digit */
	{ 1.1234567890123456, 1.1234567890123455, 0 },
	{ 1.1234567890123455, 1.1234567890123456, 1 },
	{ 2.0000000000000001, 2.0000000000000000, 1 },
	{ 2.0000000000000000, 2.0000000000000001, 1 },

	/* -2 */
	{ 1.1234567890123465, 1.1234567890123456, 0 },
	{ 1.1234567890123456, 1.1234567890123465, 1 },
	{ 2.0000000000000010, 2.0000000000000001, 0 },
	{ 2.0000000000000001, 2.0000000000000010, 1 },

	/* -3 */
	{ 1.1234567890123654, 1.1234567890123456, 0 },
	{ 1.1234567890123456, 1.1234567890123654, 1 },
	{ 2.0000000000000100, 2.0000000000000011, 1 },
	{ 2.0000000000000011, 2.0000000000000100, 1 },

	/* -4 */
	{ 1.1234567890126453, 1.1234567890125634, 0 },
};

ATF_TC(test_fmax1);
ATF_TC_HEAD(test_fmax1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good "
	    "(x, y, fmax(x,y)) pairs");
}
ATF_TC_BODY(test_fmax1, tc)
{
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++) {
		if (t1table[i].x == t1table[i].y) {
			atf_tc_fail_nonfatal(
				"Identical value (%.16e) at array position %u",
				    t1table[i].x, i);
		}

		ATF_CHECK(fmax(t1table[i].x, t1table[i].y)
		    == t1table[i].z ? t1table[i].x : t1table[i].y);
		ATF_CHECK(fmin(t1table[i].x, t1table[i].y)
		    == t1table[i].z ? t1table[i].y : t1table[i].x);
	}
}

/*
 * Test case 2 -- Edge cases
 */
#define CHK_ZERO        (1 << 0)
#define CHK_NAN         (1 << 1)
#define CHK_INF		(1 << 2)
#define CHK_SIGN	(1 << 3)

struct t2entry {
	double x;       /* Input */
	double y;       /* Input */
	double z;	/* fmax() output */
	int check;
} t2table[] = {
#ifdef	NAN
	/*
	 * If just one argument is a NaN, the other argument shall be
	 * returned.
	 */
	{ NAN, +0.0, +0.0, CHK_ZERO | CHK_SIGN },
	{ NAN, -0.0, -0.0, CHK_ZERO | CHK_SIGN },

	{ +0.0, NAN, +0.0, CHK_ZERO | CHK_SIGN },
	{ -0.0, NAN, -0.0, CHK_ZERO | CHK_SIGN },

#ifdef	INFINITY
	{ NAN, +INFINITY, +INFINITY, CHK_INF | CHK_SIGN },
	{ NAN, -INFINITY, -INFINITY, CHK_INF | CHK_SIGN },

	{ +INFINITY, NAN, +INFINITY, CHK_INF | CHK_SIGN },
	{ -INFINITY, NAN, -INFINITY, CHK_INF | CHK_SIGN },
#endif	/* INFINITY */

	/* If x and y are NaN, a NaN shall be returned */
	{ NAN, NAN, NAN, CHK_NAN }
#endif	/* NAN */
};

ATF_TC(test_fmax2);
ATF_TC_HEAD(test_fmax2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_fmax2, tc)
{
	size_t i, N;
	double oval;	/* fmax() output value */

	N = sizeof(t2table) / sizeof(t2table[0]);
	for (i = 0; i < N; i++) {
		/* Make sure that only allowed checks are set */
		ATF_REQUIRE((t2table[i].check
			& ~(CHK_ZERO | CHK_NAN | CHK_INF | CHK_SIGN)) == 0);

		/* Don't allow conflicting types to be set */
		ATF_REQUIRE((t2table[i].check & (CHK_ZERO | CHK_INF))
		    != (CHK_ZERO | CHK_INF));
		ATF_REQUIRE((t2table[i].check & (CHK_ZERO | CHK_NAN))
		    != (CHK_ZERO | CHK_NAN));
		ATF_REQUIRE((t2table[i].check & (CHK_NAN | CHK_INF))
		    != (CHK_NAN | CHK_INF));

		/* Ready to go */
		oval = fmax(t2table[i].x, t2table[i].y);
		if (t2table[i].check & CHK_ZERO) {
			ATF_CHECK(fpclassify(oval) == FP_ZERO);
		}
		if (t2table[i].check & CHK_NAN) {
			/*
			 * Grab the opportunity to cross-check the results and
			 * at the same time cross-check the functions with
			 * respect to each other.
			 */
			ATF_CHECK(isnan(oval));
			ATF_CHECK(fpclassify(oval) == FP_NAN);
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
		if (t2table[i].check & CHK_SIGN) {
			ATF_CHECK(signbit(oval) == signbit(t2table[i].z));
		}
	}
}

/*
 * Test case 3 - Some random input
 */
ATF_TC(test_fmax3);
ATF_TC_HEAD(test_fmax3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_fmax3, tc)
{
	size_t i;
	double x, y;

	/* Normal - normal */
	for (i = 0; i < 1000000; i++) {
		do {
			x = random_double(FP_NORMAL);
			y = random_double(FP_NORMAL);
		} while (x == y);

		ATF_CHECK((fmax(x, y) == x) || (fmax(x, y) == y));
		ATF_CHECK((fmin(x, y) == x) || (fmin(x, y) == y));
		ATF_CHECK((fmax(x, y) != fmin(x, y)));
	}

	/* Normal - subnormal */
	for (i = 0; i < 1000; i++) {
		do {
			x = random_double(FP_NORMAL);
			y = random_double(FP_SUBNORMAL);
		} while (x == y);

		ATF_CHECK((fmax(x, y) == x) || (fmax(x, y) == y));
		ATF_CHECK((fmin(x, y) == x) || (fmin(x, y) == y));
		ATF_CHECK((fmax(x, y) != fmin(x, y)));

#ifdef  NAN
		/*
		 * Since we are here, check again that if just one argument is a
		 * NaN, the other argument shall be returned. But this time do
		 * it with some random input and also try with subnormals.
		 */
		ATF_CHECK(fmax(NAN, x) == x);
		ATF_CHECK(fmax(NAN, y) == y);

		ATF_CHECK(fmax(x, NAN) == x);
		ATF_CHECK(fmax(y, NAN) == y);
#endif
	}

	/* Subnormal - subnormal */
	for (i = 0; i < 1000; i++) {
		do {
			x = random_double(FP_SUBNORMAL);
			y = random_double(FP_SUBNORMAL);
		} while (x == y);

		ATF_CHECK((fmax(x, y) == x) || (fmax(x, y) == y));
		ATF_CHECK((fmin(x, y) == x) || (fmin(x, y) == y));
		ATF_CHECK((fmax(x, y) != fmin(x, y)));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fmax1);
	ATF_TP_ADD_TC(tp, test_fmax2);
	ATF_TP_ADD_TC(tp, test_fmax3);

	return atf_no_error();
}
