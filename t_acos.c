#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "lconstants.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "t_acos.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_acos1);
ATF_TC_HEAD(test_acos1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, acos(x)) pairs");
}
ATF_TC_BODY(test_acos1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	for (i = 0; i < N; i++) {
		/* Sanity check */
		ATF_REQUIRE(t1dtable[i].x >= -1.0 && t1dtable[i].x <= 1.0);

		/* Actual check */
		ATF_CHECK(fpreal_equal(
			    acos(t1dtable[i].x),
				 t1dtable[i].y));
	}

	/* long double */
#ifdef	HAVE_ACOSL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	for (i = 0; i < N; i++) {
		/* Sanity check */
		ATF_REQUIRE(t1ldtable[i].x >= -1.0 && t1ldtable[i].x <= 1.0);

		/* Actual check */
		ATF_CHECK(fpreal_equall(
			    acosl(t1ldtable[i].x),
				  t1ldtable[i].y));
	}
#endif
}

/*
 * Test case 2 -- Range
 */
ATF_TC(test_acos2);
ATF_TC_HEAD(test_acos2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "The range of acos functions is [0, pi] radians");
}
ATF_TC_BODY(test_acos2, tc)
{
	float fy;
	double dy;
	long double ldy;
	double x;
	long i, N;

	/* Reusing the table from test 1 */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	for (i = 0; i < N; i++) {
		/* float */
		fy = acos((float)t1dtable[i].x);
		ATF_CHECK(fy >= 0.0 && fy <= (float)M_PI);

		/* double */
		dy = acos(t1dtable[i].x);
		ATF_CHECK(dy >= 0.0 && dy <= M_PI);

		/* long double */
#ifdef	HAVE_ACOSL
		ldy = acosl(t1ldtable[i].x);
		ATF_CHECK(ldy >= 0.0 && ldy <= M_PIl);
#endif
	}

	/* Try the same thing but with some random input */
	srand(time(NULL));

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		x = -1.0 + rand() / ((RAND_MAX / 2.0) + 1.0);
		/* Sanity check */
		ATF_REQUIRE(x >= -1.0 && x <= 1.0);

		/* float */
		fy = acosf(x);
		ATF_PASS_OR_BREAK(fy >= 0.0 && fy <= (float)M_PI);

		/* double */
		dy = acos(x);
		ATF_PASS_OR_BREAK(dy >= 0.0 && dy <= M_PI);

		/* long double */
#ifdef	HAVE_ACOSL
		ldy = acosl(x);
		ATF_PASS_OR_BREAK(ldy >= 0.0 && ldy <= M_PIl);
#endif
	}
}

/*
 * Test case 3 -- Edge cases
 */
ATF_TC(test_acos3);
ATF_TC_HEAD(test_acos3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_acos3, tc)
{
	/* If x is NaN, a NaN shall be returned */
	ATF_CHECK_IFNAN(acosf(NAN));
	ATF_CHECK_IFNAN(acos(NAN));
#ifdef	HAVE_ACOSL
	ATF_CHECK_IFNAN(acosl(NAN));
#endif

	/* If x is +1, +0 shall be returned */
	ATF_CHECK(fpreal_equalf(acosf(1.0), 0.0));
	ATF_CHECK(fpreal_equal(acos(1.0), 0.0));
#ifdef	HAVE_ACOSL
	ATF_CHECK(fpreal_equall(acosl(1.0), 0.0));
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_acos1);
	ATF_TP_ADD_TC(tp, test_acos2);
	ATF_TP_ADD_TC(tp, test_acos3);

	return atf_no_error();
}
