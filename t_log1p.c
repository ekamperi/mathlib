#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

static const struct
tentry {
	long double x;       /* Input */
	long double y;       /* log1p() output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_log1p1);
ATF_TC_HEAD(test_log1p1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, log1p(x)) pairs");
}
ATF_TC_BODY(test_log1p1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equal(log1p(ttable[i].x), ttable[i].y));
}

/*
 * Test case 2 -- Pole error
 *
 * If x is -1, a pole error shall occur and log1p(), log1pf(), and log1pl() shall
 * return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
 */
ATF_TC(test_log1p2);
ATF_TC_HEAD(test_log1p2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x is -1, a pole error shall occur");
}
ATF_TC_BODY(test_log1p2, tc)
{
	float fy;
	double dy;
	long double ldy;
	int haserrexcept;
	int haserrno;

	/* float */
	errno = 0;
	clear_exceptions();
	fy = log1pf((float)-1.0);
#ifdef  HUGE_VALF
	ATF_CHECK(fy == -HUGE_VALF);
#endif
	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(FE_DIVBYZERO));

	/* double */
	errno = 0;
	clear_exceptions();
	dy = log1p((double)-1.0);
#ifdef	HUGE_VAL
	ATF_CHECK(dy == -HUGE_VAL);
#endif
	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(FE_DIVBYZERO));

	/* long double */
#ifdef	HAVE_LOG1PL
	errno = 0;
	clear_exceptions();
	ldy = log1pl((long double)-1.0);
#ifdef  HUGE_VALL
	ATF_CHECK(ldy == -HUGE_VALL);
#endif
	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(FE_DIVBYZERO));
#endif	/* HAVE_LOG1PL */

	/*
	 * Revenge is a Dish Best Served Cold :)
	 *
	 * We put this check here and not in the beginning of the test case,
	 * because we don't want the lack of error handling support to
	 * block the check against HUGE_VAL*. Nor do we want to split the test
	 * case into 2.
	 */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);
}

/*
 * Test case 3 -- Domain error
 *
 * For finite values of x that are less than -1, or if x is -Inf, a domain error
 * shall occur, and either a NaN (if supported), or an implementation-defined
 * value shall be returned.
 */
static const long double
t3table[] = {
#ifdef	INFINITY
	-INFINITY,
#endif
#ifdef	HUGE_VAL
	-HUGE_VAL,
#endif
#ifdef	HUGE_VALF
	-HUGE_VALF,
#endif
#ifdef	HUGE_VALL
	-HUGE_VALL
#endif
};

ATF_TC(test_log1p3);
ATF_TC_HEAD(test_log1p3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x < -1 or -Inf, a domain error shall occur");
}
ATF_TC_BODY(test_log1p3, tc)
{
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	int haserrexcept;
	int haserrno;
	long i, N;

	/*
	 * First try with negative infinities
	 */
	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		fy = log1pf((float)t3table[i]);
		ATF_CHECK_IFNAN(fy);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = log1p((double)t3table[i]);
		ATF_CHECK_IFNAN(dy);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_LOG1PL
		errno = 0;
		clear_exceptions();
		ldy = log1pl(t3table[i]);
		ATF_CHECK_IFNAN(ldy);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
	}

	/*
	 * Now try finite negative values less than -1.0
	 */
	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		do {
			fx = random_float(FP_NORMAL);
		} while (fx >= -1.0);
		errno = 0;
		clear_exceptions();
		fy = log1pf((float)t3table[i]);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(fy));
#endif
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* double */
		do {
			dx = random_double(FP_NORMAL);
		} while (dx >= 0.0);
		errno = 0;
		clear_exceptions();
		dy = log1p((double)t3table[i]);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(dy));
#endif
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* long double */
		do {
			ldx = random_long_double(FP_NORMAL);
		} while (ldx >= 0.0);
		errno = 0;
		clear_exceptions();
		ldy = log1pl(t3table[i]);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(ldy));
#endif
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));
	}

	/*
	 * Revenge is a Dish Best Served Cold :)
	 *
	 * We put this check here and not in the beginning of the test case,
	 * because we don't want the lack of error handling support to
	 * block the check against NANs. Nor do we want to split the test
	 * case into 2.
	 */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);
}

/*
 * Test case 4 -- Edge cases
 */
static const struct
t4entry {
	long double x;
	long double y;
} t4table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, or +Inf, x shall be returned */
	{  0.0,  0.0 },
	{ -0.0, -0.0 },

#ifdef	INFINITY
	{ INFINITY, INFINITY },
#endif
#ifdef	HUGE_VAL
	{ HUGE_VAL, HUGE_VAL },
#endif
#ifdef	HUGE_VALF
	{ HUGE_VALF, HUGE_VALF },
#endif
#ifdef	HUGE_VALL
	{ HUGE_VALL, HUGE_VALL },
#endif
};

ATF_TC(test_log1p4);
ATF_TC_HEAD(test_log1p4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_log1p4, tc)
{
	size_t i, N;

	N = sizeof(t4table) / sizeof(t4table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpcmp_equalf(
			    log1pf((float)t4table[i].x),
				   (float)t4table[i].y));

		/* double */
		ATF_CHECK(fpcmp_equal(
			    log1p((double)t4table[i].x),
				  (double)t4table[i].y));

		/* long double */
#ifdef	HAVE_LOG1PL
		ATF_CHECK(fpcmp_equall(
			    log1pl(t4table[i].x),
				   t4table[i].y));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_log1p1);
	ATF_TP_ADD_TC(tp, test_log1p2);
	ATF_TP_ADD_TC(tp, test_log1p3);
	ATF_TP_ADD_TC(tp, test_log1p4);

	return atf_no_error();
}
