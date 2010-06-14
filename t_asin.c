#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "subr_atf.h"
#include "subr_fpcmp.h"

struct tentry {
	double x;       /* Input */
	double y;       /* asin output */
} ttable[] = {
	{  0.08615288812175992, 1.48453650540555027 },
	{  0.4695530092012254,  1.0820118902033638  },
	{ -0.5307089733580832,  2.1302331665977766  },
	{  0.1495538508054541,  1.4206792932791348  },
	{ -0.9961039037608814,  3.05329057209047    },
	{  0.4272472354989070,  1.1293503873107065  },
	{ -0.7306972753197032,  2.390139067080998   },
	{ -0.08584188927510716, 1.65674399291414827 },
	{  0.7113482334820476,  0.779381708932496   },
	{  0.1880596326987010,  1.3816101723900659  },
	{ -0.7207724711306560,  2.375712401838810   },
	{ -0.3130203460733267,  1.8891678602452887  },
	{  0.5054264693751129,	1.0409201875696186  },
	{  0.5965707233358248,  0.9315749583405370  },
	{ -0.4555739256799129,  2.0438131515590001  },
	{ -0.7971665374874495,  2.493383864694156   },
	{ -0.9933428418408301,  3.026140866317254   },
	{  0.06734719891833763, 1.50339811318482790 },
	{ -0.1681783506599399,  1.7397777327092778  },
	{  0.3093290420259538,  1.2563089376215559  }
};

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

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		/* Sanity check */
		ATF_REQUIRE(ttable[i].x >= -1.0 && ttable[i].x <= 1.0);

		/* Actual check */
		ATF_CHECK(fpcmp_equal(acos(ttable[i].x), ttable[i].y));
	}
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
	double x;
	long i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		/* Sanity check */
		ATF_REQUIRE(ttable[i].x >= -1.0 && ttable[i].x <= 1.0);

		/* Actual checks */
		ATF_CHECK(asin(ttable[i].x) >= -M_PI_2);
		ATF_CHECK(asin(ttable[i].x) <=  M_PI_2);
	}

	/* Try the same thing but with some random input */
	srand(time(NULL));

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		x = -1.0 + rand() / ((RAND_MAX / 2.0) + 1.0);
		/* Sanity check */
		ATF_REQUIRE(x >= -1.0 && x <= 1.0);

		/* Actual checks */
		ATF_PASS_OR_BREAK(asin(x) >= -M_PI_2);
		ATF_PASS_OR_BREAK(asin(x) <=  M_PI_2);
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
#ifdef	NAN
	/* If x is NaN, a NaN shall be returned */
	ATF_CHECK(fpclassify(asin(NAN)) == FP_NAN);
#endif

	/* If x is +-0, x shall be returned */
	ATF_CHECK(fpclassify(asin(+0.0)) == FP_ZERO);
	ATF_CHECK(signbit(asin(+0.0)) == 0);

	ATF_CHECK(fpclassify(asin(-0.0)) == FP_ZERO);
	ATF_CHECK(signbit(asin(-0.0)) != 0);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_asin1);
	ATF_TP_ADD_TC(tp, test_asin2);
	ATF_TP_ADD_TC(tp, test_asin3);

	return atf_no_error();
}
