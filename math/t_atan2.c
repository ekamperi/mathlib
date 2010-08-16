#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "t_atan2.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_atan21);
ATF_TC_HEAD(test_atan21, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good "
	    "(x, y, atan2(x, y)) pairs");
}
ATF_TC_BODY(test_atan21, tc)
{
	size_t i, N;

	/* double */
        N = sizeof(t1dtable) / sizeof(t1dtable[0]);
        ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		ATF_CHECK(fpreal_equal(
			    atan2(t1dtable[i].x, t1dtable[i].y),
				  t1dtable[i].z));
	}

	/* long double */
#ifdef	HAVE_ATAN2L
        N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
        ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
                ATF_CHECK(fpreal_equall(
                            atan2l(t1ldtable[i].x, t1ldtable[i].y),
				   t1ldtable[i].z));
	}
#endif
}

/*
 * Test case 2 -- Range
 */
ATF_TC(test_atan22);
ATF_TC_HEAD(test_atan22, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "The range of atan2 function is [-pi, pi] radians");
}
ATF_TC_BODY(test_atan22, tc)
{
	long i, N;

	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(atan2(t1dtable[i].y, t1dtable[i].x) >= -M_PI);
		ATF_CHECK(atan2(t1dtable[i].y, t1dtable[i].x) <=  M_PI);
	}

	/* Try the same thing but with some random input */
	srand(time(NULL));

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

#define X1      -1E4
#define X2       1E4
	ATF_FOR_LOOP(i, N, i++) {
		double x = X1 + rand() / (RAND_MAX / (X2 - X1 + 1.0) + 1.0);
		double y = X1 + rand() / (RAND_MAX / (X2 - X1 + 1.0) + 1.0);
		if (fpclassify(x) == FP_ZERO) {
			continue;
		}

		ATF_PASS_OR_BREAK(atan2(y, x) >= -M_PI);
		ATF_PASS_OR_BREAK(atan2(y, x) <=  M_PI);
	}
}

/*
 * Test case 3 -- Edge cases
 */
static const struct
t3entry {
	long double y;       /* Input */
	long double x;
	long double z;       /* atan2 output */
} t3table[] = {
	/* If y is +-0 and x is < 0, +-Pi shall be returned */
	{ +0.0, -1E-5, M_PI },
	{ +0.0, -1.0,  M_PI },
	{ +0.0, -2.0,  M_PI },

	{ -0.0, -1E-5,-M_PI },
	{ -0.0, -1.0, -M_PI },
	{ -0.0, -2.0, -M_PI },

	/* If y is +-0 and x is > 0, +-0 shall be returned */
	{ +0.0, 1E-5,+0.0 },
	{ +0.0, 1.0, +0.0 },
	{ +0.0, 2.0, +0.0 },

	{ -0.0, 1E-5,-0.0 },
	{ -0.0, 1.0, -0.0 },
	{ -0.0, 2.0, -0.0 },

	/* If y is < 0 and x is +-0, -Pi/2 shall be returned */
	{ -1E-5, +0.0, -M_PI_2 },
	{ -1.0,  +0.0, -M_PI_2 },
	{ -2.0,  +0.0, -M_PI_2 },

	{ -1E-5, -0.0, -M_PI_2 },
	{ -1.0,  -0.0, -M_PI_2 },
	{ -2.0,  -0.0, -M_PI_2 },

	/* If y is > 0 and x is +-0, Pi/2 shall be returned */
	{ 1E-5, +0.0, M_PI_2 },
	{ 1.0,  +0.0, M_PI_2 },
	{ 2.0,  +0.0, M_PI_2 },

	{ 1E-5, -0.0, M_PI_2 },
	{ 1.0,  -0.0, M_PI_2 },
	{ 2.0,  -0.0, M_PI_2 },

	/* If either x or y is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, +0.0, NAN },
	{ NAN, +1.0, NAN },
	{ NAN, +2.0, NAN },
	{ NAN, -0.0, NAN },
	{ NAN, -1.0, NAN },
	{ NAN, -2.0, NAN },

	{ +0.0, NAN, NAN },
	{ +1.0, NAN, NAN },
	{ +2.0, NAN, NAN },
	{ -0.0, NAN, NAN },
	{ -1.0, NAN, NAN },
	{ -2.0, NAN, NAN },
#endif	/* NAN */
	/* If y is +-0 and x is -0, +-Pi shall be returned */
	{ +0.0, -0.0,  M_PI },
	{ +0.0, -0.0,  M_PI },
	{ +0.0, -0.0,  M_PI },

	{ -0.0, -0.0, -M_PI },
	{ -0.0, -0.0, -M_PI },
	{ -0.0, -0.0, -M_PI },

	/* If y is +-0 and x is +0, +-0 shall be returned */
	{ +0.0, +0.0, +0.0 },
	{ +0.0, +0.0, +0.0 },
	{ +0.0, +0.0, +0.0 },

	{ -0.0, +0.0, -0.0 },
	{ -0.0, +0.0, -0.0 },
	{ -0.0, +0.0, -0.0 },

	/* For finite values of +-y > 0, if x is -Inf, +-Pi shall be returned */
#ifdef	INFINITY
	{ 1E-5, -INFINITY, M_PI },
	{  1.0, -INFINITY, M_PI },
	{  2.0, -INFINITY, M_PI },
	{ 1E+5, -INFINITY, M_PI },

	{ -1E-5, -INFINITY, -M_PI },
	{  -1.0, -INFINITY, -M_PI },
	{  -2.0, -INFINITY, -M_PI },
	{ -1E+5, -INFINITY, -M_PI },

	/* For finite values of +-y > 0, if x is +Inf, +-0 shall be returned */
	{ 1E-5, +INFINITY, +0.0 },
	{  1.0, +INFINITY, +0.0 },
	{  2.0, +INFINITY, +0.0 },
	{ 1E+5, +INFINITY, +0.0 },

	{ -1E-5, +INFINITY, -0.0 },
	{  -1.0, +INFINITY, -0.0 },
	{  -2.0, +INFINITY, -0.0 },
	{ -1E+5, +INFINITY, -0.0 },

	/* For finite values of x, if y is +-Inf, +-Pi/2 shall be returned */
	{ +INFINITY,  1E-5,  M_PI_2 },
	{ +INFINITY,   1.0,  M_PI_2 },
	{ +INFINITY,   2.0,  M_PI_2 },
	{ +INFINITY,  1E+5,  M_PI_2 },

	{ -INFINITY, -1E-5, -M_PI_2 },
	{ -INFINITY,  -1.0, -M_PI_2 },
	{ -INFINITY,  -2.0, -M_PI_2 },
	{ -INFINITY, -1E+5, -M_PI_2 },

	/* If y is +-Inf and x is -Inf, +-3Pi/4 shall be returned */
	{ +INFINITY, -INFINITY,  3*M_PI/4 },
	{ +INFINITY, -INFINITY,  3*M_PI/4 },
	{ +INFINITY, -INFINITY,  3*M_PI/4 },
	{ +INFINITY, -INFINITY,  3*M_PI/4 },

	{ -INFINITY, -INFINITY, -3*M_PI/4 },
	{ -INFINITY, -INFINITY, -3*M_PI/4 },
	{ -INFINITY, -INFINITY, -3*M_PI/4 },
	{ -INFINITY, -INFINITY, -3*M_PI/4 },

	/* If y is +-Inf and x is +Inf, +-Pi/4 shall be returned */
	{ +INFINITY, INFINITY,  M_PI/4 },
	{ +INFINITY, INFINITY,  M_PI/4 },
	{ +INFINITY, INFINITY,  M_PI/4 },
	{ +INFINITY, INFINITY,  M_PI/4 },

	{ -INFINITY, INFINITY, -M_PI/4 },
	{ -INFINITY, INFINITY, -M_PI/4 },
	{ -INFINITY, INFINITY, -M_PI/4 },
	{ -INFINITY, INFINITY, -M_PI/4 },
#endif	/* INFINITY */

};

ATF_TC(test_atan23);
ATF_TC_HEAD(test_atan23, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_atan23, tc)
{
	float fz;
	double dz;
	long double ldz;
	size_t i, N;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* Mind that y comes before x */
		/* float */
		fz = atan2f((float)t3table[i].y,
			    (float)t3table[i].x);
		ATF_CHECK(fpreal_equalf(fz, (float)t3table[i].z));

		/* double */
		dz = atan2((double)t3table[i].y,
			   (double)t3table[i].x);
		ATF_CHECK(fpreal_equal(dz, (double)t3table[i].z));

		/* long double */
#ifdef	HAVE_ATAN2L
		ldz = atan2l(t3table[i].y,
			     t3table[i].x);
		ATF_CHECK(fpreal_equall(ldz, t3table[i].z));
#endif
	}
}

/*
 * Test case 4 -- Edge case on errno
 *
 * XXX: Check for any raised exceptions as well
 */
ATF_TC(test_atan24);
ATF_TC_HEAD(test_atan24, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If both arguments are 0, a domain error shall not occur");
}
ATF_TC_BODY(test_atan24, tc)
{
	/* float */
	errno = 0; atan2f(+0.0, +0.0); ATF_CHECK(errno != EDOM);
	errno = 0; atan2f(+0.0, -0.0); ATF_CHECK(errno != EDOM);
	errno = 0; atan2f(-0.0, +0.0); ATF_CHECK(errno != EDOM);
	errno = 0; atan2f(-0.0, -0.0); ATF_CHECK(errno != EDOM);

	/* double */
	errno = 0; atan2(+0.0, +0.0); ATF_CHECK(errno != EDOM);
	errno = 0; atan2(+0.0, -0.0); ATF_CHECK(errno != EDOM);
	errno = 0; atan2(-0.0, +0.0); ATF_CHECK(errno != EDOM);
	errno = 0; atan2(-0.0, -0.0); ATF_CHECK(errno != EDOM);

	/* long double */
#ifdef	HAVE_ATAN2L
	errno = 0; atan2l(+0.0, +0.0); ATF_CHECK(errno != EDOM);
	errno = 0; atan2l(+0.0, -0.0); ATF_CHECK(errno != EDOM);
	errno = 0; atan2l(-0.0, +0.0); ATF_CHECK(errno != EDOM);
	errno = 0; atan2l(-0.0, -0.0); ATF_CHECK(errno != EDOM);
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_atan21);
	ATF_TP_ADD_TC(tp, test_atan22);
	ATF_TP_ADD_TC(tp, test_atan23);
	ATF_TP_ADD_TC(tp, test_atan24);

	return atf_no_error();
}