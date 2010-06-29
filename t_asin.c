#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "t_asin.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_asin1);
ATF_TC_HEAD(test_asin1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, asin(x)) pairs");
}
ATF_TC_BODY(test_asin1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	for (i = 0; i < N; i++) {
		/* Sanity check */
		ATF_REQUIRE(t1dtable[i].x >= -1.0 && t1dtable[i].x <= 1.0);

		/* Actual check */
		ATF_CHECK(fpcmp_equal(
			    asin(t1dtable[i].x),
				 t1dtable[i].y));
	}

        /* long double */
#ifdef	HAVE_ASINL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
        for (i = 0; i < N; i++) {
                /* Sanity check */
                ATF_REQUIRE(t1ldtable[i].x >= -1.0 && t1ldtable[i].x <= 1.0);

                /* Actual check */
                ATF_CHECK(fpcmp_equall(
                            asinl(t1ldtable[i].x),
				  t1ldtable[i].y));
        }
#endif
}

/*
 * Test case 2 -- Range
 */
ATF_TC(test_asin2);
ATF_TC_HEAD(test_asin2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "The range of asin function is [-pi/2, pi/2] radians");
}
ATF_TC_BODY(test_asin2, tc)
{
	float fy;
	double dy;
	long double ldy;
	double x;
	long i, N;

	/* We are reusing the table from test 1 */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	for (i = 0; i < N; i++) {
		/* Sanity check */
		ATF_REQUIRE(t1dtable[i].x >= -1.0 && t1dtable[i].x <= 1.0);

		/* Actual checks */
		ATF_CHECK(asin(t1dtable[i].x) >= -M_PI_2);
		ATF_CHECK(asin(t1dtable[i].x) <=  M_PI_2);
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
		fy = asinf(x);
		ATF_PASS_OR_BREAK(fy >= -M_PI_2 && fy <= M_PI_2);

		/* double */
		dy = asin(x);
		ATF_PASS_OR_BREAK(dy >= -M_PI_2 && dy <= M_PI_2);

		/* long double */
#ifdef	HAVE_ASINL
		ldy = asinl(x);
		ATF_PASS_OR_BREAK(ldy >= -M_PI_2 && ldy <= M_PI_2);
#endif
	}
}

/*
 * Test case 3 -- Edge cases
 */
ATF_TC(test_asin3);
ATF_TC_HEAD(test_asin3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_asin3, tc)
{
	/* If x is NaN, a NaN shall be returned */
	ATF_CHECK_IFNAN(asinf(NAN));
        ATF_CHECK_IFNAN(asin(NAN));
#ifdef	HAVE_ASINL
        ATF_CHECK_IFNAN(asinl(NAN));
#endif

	/* If x is +-0, x shall be returned */
	ATF_CHECK(fpcmp_equalf(asinf( 0.0),  0.0));
        ATF_CHECK(fpcmp_equalf(asinf(-0.0), -0.0));

        ATF_CHECK(fpcmp_equal(asin( 0.0),  0.0));
        ATF_CHECK(fpcmp_equal(asin(-0.0), -0.0));

#ifdef	HAVE_ASINL
        ATF_CHECK(fpcmp_equall(asinl( 0.0),  0.0));
        ATF_CHECK(fpcmp_equall(asinl(-0.0), -0.0));
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_asin1);
	ATF_TP_ADD_TC(tp, test_asin2);
	ATF_TP_ADD_TC(tp, test_asin3);

	return atf_no_error();
}
