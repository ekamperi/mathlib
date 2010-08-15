#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdio.h>

#include "mytypes.h"
#include "subr_atf.h"
#include "subr_random.h"

ATF_TC(test_rand1);
ATF_TC_HEAD(test_rand1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check that our random generator outputs valid fp numbers");
}
ATF_TC_BODY(test_rand1, tc)
{
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	/*
	 * This check may look like totally pointless, but it's not.
	 * It catches invalid floating point numbers. Really.
	 */
	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		fy = random_float(FP_NORMAL);
		ATF_PASS_OR_BREAK(fx < fy || fx >= fy);
		ATF_PASS_OR_BREAK(fpclassify( 1.0 * fx) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify( 1.0 * fy) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(-1.0 * fx) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(-1.0 * fy) == FP_NORMAL);

		/* double */
		dx = random_double(FP_NORMAL);
		dy = random_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(dx < dy || dx >= dy);
		ATF_PASS_OR_BREAK(fpclassify( 1.0 * dx) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify( 1.0 * dy) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(-1.0 * dx) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(-1.0 * dy) == FP_NORMAL);

		/* long double */
		ldx = random_long_double(FP_NORMAL);
		ldy = random_long_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(ldx < ldy || ldx >= ldy);
		ATF_PASS_OR_BREAK(fpclassify( 1.0 * ldx) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify( 1.0 * ldy) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(-1.0 * ldx) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(-1.0 * ldy) == FP_NORMAL);
	}
}

/*
 * Test case 2
 */
#if defined(__i386__) || defined(__x86_64__)
static const struct t2entry {
	union {
		long double x;
		uint32_t y[3];
	} u;
	int v;	/* Whether u.x is valid or not */
} t2table[] = {
	/***********************************************************************
	 *			VALID FLOATING POINT NUMBERS
	 **********************************************************************/
	/* Normals */
	{ .u.x =  1.00E-10, .v = 1 },
	{ .u.x = -1.00E-10, .v = 1 },
	{ .u.x =  1.23E-20, .v = 1 },
	{ .u.x = -1.23E-20, .v = 1 },
	{ .u.x =  1.234567, .v = 1 },
	{ .u.x = -1.234567, .v = 1 },

	/* Denormals */
	{ .u.x =  1.68105157155604675313133890866087630e-4932L, .v = 1 },
	{ .u.x = -1.68105157155604675313133890866087630e-4932L, .v = 1 },
	{ .u.x =  2.19619904728995135778739881115052741e-4932L, .v = 1 },
	{ .u.x = -2.19619904728995135778739881115052741e-4932L, .v = 1 },
	{ .u.x =  2.09040270689746357591598655419975601e-4932L, .v = 1 },
	{ .u.x = -2.09040270689746357591598655419975601e-4932L, .v = 1 },

	{ .u.x =  0.0, .v = 1 },
	{ .u.x = -0.0, .v = 1 },

#ifdef	NAN
	{ .u.x =  NAN, .v = 1 },
	{ .u.x = -NAN, .v = 1 },
#endif
#ifdef	INFINITY
	{ .u.x =  INFINITY, .v = 1 },
	{ .u.x = -INFINITY, .v = 1 },
#endif
#ifdef	HUGE_VAL
	{ .u.x =  HUGE_VAL, .v = 1 },
	{ .u.x = -HUGE_VAL, .v = 1 },
#endif
#ifdef	HUGE_VALF
	{ .u.x =  HUGE_VALF, .v = 1 },
	{ .u.x = -HUGE_VALF, .v = 1 },
#endif
#ifdef	HUGE_VALL
	{ .u.x =  HUGE_VALL, .v = 1 },
	{ .u.x = -HUGE_VALL, .v = 1 },
#endif
	/***********************************************************************
	 *			INVALID FLOATING POINT NUMBERS
	 **********************************************************************/
	{ .u.y = {1, 0, 0xFFFF}, .v = 0 }
};
#endif	/* defined(__i386__) || defined(__x86_64__) */

ATF_TC(test_rand2);
ATF_TC_HEAD(test_rand2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check zeros, nan, inf");
}
ATF_TC_BODY(test_rand2, tc)
{
#if defined(__i386__) || defined(__x86_64__)
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++)
		ATF_CHECK(isvalidfp_intel80bit(&t2table[i].u.y[0])
		    == t2table[i].v);
#endif	/* defined(__i386__) || defined(__x86_64__) */
}

/*
 * Test case 3 -- Check FP_NORMAL
 */
ATF_TC(test_rand3);
ATF_TC_HEAD(test_rand3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check FP_NORMAL generation");
}
ATF_TC_BODY(test_rand3, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(fx) == FP_NORMAL);

		/* double */
		dx = random_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(dx) == FP_NORMAL);

		/* long double */
		ldx = random_long_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(ldx) == FP_NORMAL);
	}
}

/*
 * Test csae 4 -- Check FP_SUBNORMAL
 */
ATF_TC(test_rand4);
ATF_TC_HEAD(test_rand4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check FP_SUBNORMAL generation");
}
ATF_TC_BODY(test_rand4, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_SUBNORMAL);
		ATF_PASS_OR_BREAK(fpclassify(fx) == FP_SUBNORMAL);

		/* double */
		dx = random_double(FP_SUBNORMAL);
		ATF_PASS_OR_BREAK(fpclassify(dx) == FP_SUBNORMAL);

		/* long double */
		ldx = random_long_double(FP_SUBNORMAL);
		ATF_PASS_OR_BREAK(fpclassify(ldx) == FP_SUBNORMAL);
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_rand1);
	ATF_TP_ADD_TC(tp, test_rand2);
	ATF_TP_ADD_TC(tp, test_rand3);
	ATF_TP_ADD_TC(tp, test_rand4);

	return atf_no_error();
}
