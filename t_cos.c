#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>

#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1 -- Basic functionality
 */
struct tentry {
        long double x;       /* Input */
        long double y;       /* cos output */
} ttable[] = {
};

ATF_TC(test_cos1);
ATF_TC_HEAD(test_cos1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, cos(x)) pairs");
}
ATF_TC_BODY(test_cos1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(FPCMP_EQUAL(cos(ttable[i].x), ttable[i].y));
}

/*
 * Test case 2 -- Range
 */
ATF_TC(test_cos2);
ATF_TC_HEAD(test_cos2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "The range of cos functions is [-1, 1] radians");
}
ATF_TC_BODY(test_cos2, tc)
{
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		fy = cosf(fx);
		ATF_PASS_OR_BREAK(fy >= -1.0 && fy <= 1.0);

		/* double */
		dx = random_double(FP_NORMAL);
		dy = cos(dx);
		ATF_PASS_OR_BREAK(dy >= -1.0 && dy <= 1.0);

		/* long double */
		ldx = random_long_double(FP_NORMAL);
		ldy = cosl(ldx);
		ATF_PASS_OR_BREAK(ldy >= -1.0 && ldy <= 1.0);
	}
}

/*
 * Test case 3 -- Edge cases
 */
ATF_TC(test_cos3);
ATF_TC_HEAD(test_cos3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_cos3, tc)
{
#ifdef	NAN
	/* If x is NaN, a NaN shall be returned */
	ATF_CHECK(fpcmp_equal(cos(NAN), NAN));
	ATF_CHECK(fpcmp_equal(cosf(NAN), NAN));
	ATF_CHECK(fpcmp_equal(cosl(NAN), NAN));
#endif

	/* If x is +-0, x shall be returned */
	ATF_CHECK(fpcmp_equal(cos( 0.0), 1.0));
	ATF_CHECK(fpcmp_equal(cos(-0.0), 1.0));

	ATF_CHECK(fpcmp_equal(cosf( 0.0), 1.0));
	ATF_CHECK(fpcmp_equal(cosf(-0.0), 1.0));

	ATF_CHECK(fpcmp_equal(cosl( 0.0), 1.0));
	ATF_CHECK(fpcmp_equal(cosl(-0.0), 1.0));
}

/*
 * Test case 4 -- Error conditions
 */
long double t4table[] = {
#ifdef	INFINITY
	INFINITY,
#endif
#ifdef	HUGE_VAL
	HUGE_VAL,
#endif
#ifdef	HUGE_VALF
	HUGE_VALF,
#endif
#ifdef	HUGE_VALL
	HUGE_VALL
#endif
};

ATF_TC(test_cos4);
ATF_TC_HEAD(test_cos4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check what happens when +-Inf is given as input");
}
ATF_TC_BODY(test_cos4, tc)
{
	float fy;
	double dy;
	long double ldy;
	int haserrexcept;
	int haserrno;
	size_t i, N;

	/*
	 * If x is +-Inf, a domain error shall occur, and either a NaN
	 * (if supported), or an implementation-defined value shall be
	 * returned.
	 */
	N = sizeof(t4table) / sizeof(t4table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		fy = cosf((float)t4table[i]);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#ifdef	NAN
		ATF_CHECK(isnan(fy));
#endif

		/* double */
		errno = 0;
		clear_exceptions();
		dy = cos((double)t4table[i]);
                ATF_CHECK(iserrno_equalto(EDOM));
                ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#ifdef	NAN
		ATF_CHECK(isnan(dy));
#endif

		/* long double */
		errno = 0;
		clear_exceptions();
		ldy = cosl(t4table[i]);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#ifdef	NAN
		ATF_CHECK(isnan(ldy));
#endif
	}

	/*
	 * Revenge is a Dish Best Served Cold :)
	 *
	 * We put this check here and not in the start of the test case,
	 * because we don't want the lack of error handling support to
	 * block the check against NANs. Nor do we want to split the test
	 * case into 2.
	 */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_cos1);
	ATF_TP_ADD_TC(tp, test_cos2);
	ATF_TP_ADD_TC(tp, test_cos3);
	ATF_TP_ADD_TC(tp, test_cos4);

	return atf_no_error();
}
