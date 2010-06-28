#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "t_atan.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_atan1);
ATF_TC_HEAD(test_atan1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, atan(x)) pairs");
}
ATF_TC_BODY(test_atan1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equal(
			    atan(t1dtable[i].x),
				 t1dtable[i].y));

	/* long double */
#ifdef	HAVE_ATANL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equall(
			    atanl(t1ldtable[i].x),
				  t1ldtable[i].y));
#endif
}

/*
 * Test case 2 -- Range
 */
ATF_TC(test_atan2);
ATF_TC_HEAD(test_atan2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "The range of atan function is [-pi/2, pi/2] radians");
}
ATF_TC_BODY(test_atan2, tc)
{
	float fy;
	double dy;
	long double ldy;
	double x;
	long i, N;

	/* We are reusing table from test 1 */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(atan(t1dtable[i].x) >= -M_PI_2 -0.1);
		ATF_CHECK(atan(t1dtable[i].x) <=  M_PI_2 +0.1);
	}

	/* Try the same thing but with some random input */
	srand(time(NULL));

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		x = -1.0 + rand() / ((RAND_MAX / 2.0) + 1.0);

		/* float */
		fy = atanf(x);
		ATF_PASS_OR_BREAK(fy >= -M_PI_2 && fy <= M_PI_2);

		/* double */
		dy = atan(x);
		ATF_PASS_OR_BREAK(dy >= -M_PI_2 && dy <= M_PI_2);

		/* long double */
#ifdef	HAVE_ATANL
		ldy = atanl(x);
		ATF_PASS_OR_BREAK(ldy >= -M_PI_2 && ldy <= M_PI_2);
#endif
	}
}

/*
 * Test case 3 -- Edge cases
 */
static const struct
t3entry {
	long double x;	/* Input */
	long double y;	/* atan output */
} t3table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, x shall be returned */
	{ +0.0, +0.0 },
	{ -0.0, -0.0 },

	/* If x is +-Inf, +-pi/2 shall be returned */
#ifdef  INFINITY
	{  INFINITY,  M_PI_2 },
	{ -INFINITY, -M_PI_2 },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL,  M_PI_2 },
	{ -HUGE_VAL, -M_PI_2 },
#endif
#ifdef	HUGE_VALF
	{  HUGE_VALF,  M_PI_2 },
	{ -HUGE_VALF, -M_PI_2 },
#endif
#ifdef	HUGE_VALL
	{  HUGE_VALL,  M_PI_2 },
	{ -HUGE_VALL, -M_PI_2 },
#endif
};

ATF_TC(test_atan3);
ATF_TC_HEAD(test_atan3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_atan3, tc)
{
	size_t i, N;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpcmp_equalf(
			    atanf((float)t3table[i].x),
					 t3table[i].y));

		/* double */
		ATF_CHECK(fpcmp_equal(
			    atan((double)t3table[i].x),
					 t3table[i].y));

		/* long double */
#ifdef	HAVE_ATANL
		ATF_CHECK(fpcmp_equall(
			    atanl(t3table[i].x),
				  t3table[i].y));
#endif
	}

	/*
	 * XXX: If x is subnormal, a range error _may_ occur and x _should_ be
	 * returned.
	 */
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_atan1);
	ATF_TP_ADD_TC(tp, test_atan2);
	ATF_TP_ADD_TC(tp, test_atan3);

	return atf_no_error();
}
