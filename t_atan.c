#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"

struct tentry {
	double x;       /* Input */
	double y;       /* atan output */
} ttable[] = {
	{   0.2746968750816663,  0.2680843762480451  },
	{  -0.6533192920401181, -0.5787051040482428  },
	{   1.711232427407161,   1.0419456601902422  },
	{  -1.246969484557519,  -0.8948709940669250  },
	{   0.2449559926325371,  0.2402257569534314  },
	{  -1.601381594401872,  -1.0125848589164963  },
	{   1.566410045518831,   1.0026172984325502  },
	{   0.8968664489700577,  0.7310811581675730  },
	{  -1.323657975719039,  -0.9237958518208762  },
	{   0.1604104449543282,  0.1590554363795588  },
	{   0.9703219032425796,  0.7703367399058148  },
	{   1.978948399056122,   1.1029026684533513  },
	{   1.756387255122500,   1.0532181446147413  },
	{  -0.3095501907121543, -0.3001952453616477  },
	{  -0.7126646723375600, -0.6191752701433691  },
	{   1.149582555243343,   0.8548729605680148  },
	{  -0.9528324881152832, -0.7612494752261140  },
	{   0.6881271010600418,  0.6027130513781367  },
	{   0.09317548940142384, 0.09290724563009386 },
	{   0.2667379065241580,  0.2606689008080304  },
	{ -10.83285632848368,   -1.47874544767466197 },
	{  -3.999094271680332,  -1.3257643741749723  },
	{  -5.871650536009957,  -1.4021050253139658  },
	{ -16.91913128747894,   -1.51176031821366197 },
	{  -3.110560616000720,  -1.2597464886677286  },
	{   6.610295925050561,   1.4206556045260153  },
	{  16.08218608249333,    1.50869567906956284 },
	{   3.207106322430098,   1.2685424169025704  },
	{  18.35200339751547,    1.51636019690264897 },
	{   5.638511994239757,   1.3952697021155197  }
};

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

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equal(atan(ttable[i].x), ttable[i].y));
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

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(atan(ttable[i].x) >= -M_PI_2 -0.1);
		ATF_CHECK(atan(ttable[i].x) <=  M_PI_2 +0.1);
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
