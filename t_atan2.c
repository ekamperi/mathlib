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

struct tentry {
	double y;	/* Input */
	double x;	/* Input */
	double z;       /* atan2 output */
} ttable[] = {
	{  0.7028644765157690,  1.157903844233632,   0.5455612972773314 },
	{ -1.174026967951160,   0.5037106674006866, -1.1655043308926833 },
	{  0.2203423739732324,  0.4673428153504151,  0.4405716876053110 },
	{ -0.1625555214802983,  0.6091478421246359, -0.2607803191459014 },
	{  0.8451155082572130, -0.6415958791868741,  2.2201476103823028 },
	{  1.503814002705408,   1.606727792799137,   0.7523247408256290 },
	{  1.915085704331369,   0.8034882929944445,  1.1735446654267677 },
	{  0.04629664851306625, 0.7783956832567789,  0.0594070257500998 },
	{ -1.051717486846842,   1.755975233697224,  -0.5396370752557706 },
	{ -0.03814502202595182, 0.2879208246253793, -0.1317173496847148 },
	{ -1.496997654131485,  -1.131846210209786,  -2.2181756881124856 },
	{ -1.638460406632822,   1.343239570408348,  -0.8840874259674157 },
	{  0.2976119060687941,  1.066268054782833,   0.2721882876722251 },
	{ -1.894293267915478,   1.629495323961864,  -0.8604030027935500 },
	{ -0.4270159900720696,  0.6657962438682716, -0.5702783777332218 },
	{ -0.1602732083402367,  0.9262042271812438, -0.1713462424392867 },
	{  1.255419071224451,   1.662301564671465,   0.6468395547296143 },
	{  0.2171019980717572, -0.7803014832433854,  2.8702275509755315 },
	{  1.335709144460190,   0.3405931326612235,  1.3211262727421737 },
	{  0.7444431289095341,  0.4647278732156348,  1.0127275780143663 },

	{   25.11770127766777,  131.3299577351823,  0.1889743374996462  },
	{ -420.4309999091702,   818.9136951037774, -0.4743107370881560  },
	{  -25.46085312326651, -707.3534344760266, -3.10561365825668309 },
	{  218.6107914516988,   282.8801599505280,  0.6579363622105629  },
	{  278.7854079624790,   544.6946057314648,  0.4730585642082959  },
	{ -919.2038883420381,   398.5608515183068, -1.1616694635098354  },
	{  638.3964006940141,  -808.1803688326570,  2.4730294399497195  },
	{  284.3729777510462,  -357.7030490501620,  2.4699095574618105  },
	{ -181.2814129274356,  -973.9217909856829, -2.9575631937386646  },
	{  818.1147572745582,  -286.1268026268422,  1.9072387908403712  }
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_atan21);
ATF_TC_HEAD(test_atan21, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, atan2(x)) pairs");
}
ATF_TC_BODY(test_atan21, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equal(
			    atan2(ttable[i].y, ttable[i].x),
			    ttable[i].z));
	}
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

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(atan2(ttable[i].y, ttable[i].x) >= -M_PI);
		ATF_CHECK(atan2(ttable[i].y, ttable[i].x) <=  M_PI);
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
		ATF_CHECK(fpcmp_equalf(fz, (float)t3table[i].z));

		/* double */
		dz = atan2((double)t3table[i].y,
			   (double)t3table[i].x);
		ATF_CHECK(fpcmp_equal(dz, (double)t3table[i].z));

		/* long double */
#ifdef	HAVE_ATAN2L
		ldz = atan2l(t3table[i].y,
			     t3table[i].x);
		ATF_CHECK(fpcmp_equall(ldz, t3table[i].z));
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
