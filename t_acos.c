#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"

static const struct
tentry {
	double x;       /* Input */
	double y;       /* acos output */
} ttable[] = {
	{  0.4692869279377545,  1.0823132365133747 },
	{ -0.11347486190157241, 1.6845161380609253 },
	{  0.4647505724144434,  1.087443434912057  },
	{ -0.6976324477513756,  2.3428839502471837 },
	{ -0.2452115558455712,  1.8185342314424182 },
	{  0.4589749043710669,  1.0939552758742006 },
	{  0.6611969343774788,  0.8483832246938233 },
	{ -0.9541427464713692,  2.8375792166185008 },
	{ -0.2530558053894705,  1.8266338963621322 },
	{ -0.7909187169003782,  2.4831052355172236 },
	{  0.5757528726313965,  0.9572716712493159 },
	{ -0.7155298261397389,  2.368178569675924  },
	{  0.42990536464184537, 1.1264083681668051 },
	{  0.3573656087378052,  1.2053506152655027 },
	{  0.8352071602803219,  0.5822869499591871 },
	{  0.6113005673267504,  0.9130933983404338 },
	{ -0.2545812668769236,  1.8282110054372431 },
	{  0.8354152061585842,  0.5819085315754063 },
	{  0.6010496167223316,  0.9259825505608865 },
	{ -0.04366697399123387, 1.6144771901052724 }
};

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

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		/* Sanity check */
		ATF_REQUIRE(ttable[i].x >= -1.0 && ttable[i].x <= 1.0);

		/* Actual checks */
		ATF_CHECK(acos(ttable[i].x) >= 0.0);
		ATF_CHECK(acos(ttable[i].x) <= M_PI);
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
		ATF_PASS_OR_BREAK(fy >= 0.0 && fy <= M_PI);

		/* double */
		dy = acos(x);
		ATF_PASS_OR_BREAK(dy >= 0.0 && dy <= M_PI);

		/* long double */
#ifdef	HAVE_ACOSL
		ldy = acosl(x);
		ATF_PASS_OR_BREAK(ldy >= 0.0 && ldy <= M_PI);
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
	ATF_CHECK(fpcmp_equalf(acosf(1.0), 0.0));
	ATF_CHECK(fpcmp_equal(acos(1.0), 0.0));
#ifdef	HAVE_ACOSL
	ATF_CHECK(fpcmp_equall(acosl(1.0), 0.0));
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
