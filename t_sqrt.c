#include <atf-c.h>
#include <errno.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_sqrt.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_sqrt1);
ATF_TC_HEAD(test_sqrt1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, sqrt(x)) pairs");
}
ATF_TC_BODY(test_sqrt1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equal(
			    sqrt(t1dtable[i].x),
				 t1dtable[i].y));

	/* long double */
#ifdef	HAVE_SQRTL
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equall(
			    sqrtl(t1ldtable[i].x),
				  t1ldtable[i].y));
#endif
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;
	long double y;
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef  NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, x shall be returned */
	{  0.0,  0.0 },
	{ -0.0, -0.0 },

	/* If x is +Inf, x shall be returned */
#ifdef  INFINITY
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

ATF_TC(test_sqrt2);
ATF_TC_HEAD(test_sqrt2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_sqrt2, tc)
{
	float fy;
	double dy;
	long double ldy;
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		fy = sqrtf(t2table[i].x);
		ATF_CHECK(fpreal_equal(fy, (float)t2table[i].y));

		/* double */
		dy = sqrt(t2table[i].x);
		ATF_CHECK(fpreal_equal(dy, (double)t2table[i].y));

		/* long double */
#ifdef	HAVE_SQRTL
		ldy = sqrtl(t2table[i].x);
		ATF_CHECK(fpreal_equal(ldy, t2table[i].y));
#endif
	}
}

/*
 * Test case 3 -- Error conditions
 */
ATF_TC(test_sqrt3);
ATF_TC_HEAD(test_sqrt3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check what happens with finite values of x < -0");
}
ATF_TC_BODY(test_sqrt3, tc)
{
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	int haserrexcept;
	int haserrno;
	long i, N;

	/*
	 * For finite values of x < -0, a domain error shall occur, and either
	 * a NaN (if supported), or an implementation-defined value shall be
	 * returned.
	 */
	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		do {
			fx = random_float(FP_NORMAL);
		} while (fx >= -0.0);

		errno = 0;
		clear_exceptions();
		fy = sqrtf(fx);
		ATF_CHECK_IFNAN(fy);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		do {
			dx = random_double(FP_NORMAL);
		} while (dx >= -0.0);

		errno = 0;
		clear_exceptions();
		dy = sqrt(fx);
		ATF_CHECK_IFNAN(dy);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));


		/* long double */
#ifdef	HAVE_SQRTL
		do {
			ldx = random_long_double(FP_NORMAL);
		} while (ldx >= -0.0);

		errno = 0;
		clear_exceptions();
		ldy = sqrtl(ldx);
		ATF_CHECK_IFNAN(ldy);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
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
 * Test case 4
 *
 * If x is -Inf, a domain error shall occur, and either a NaN (if supported),
 * or an implementation-defined value shall be returned.
 */
static const long double
t4table[] = {
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

ATF_TC(test_sqrt4);
ATF_TC_HEAD(test_sqrt4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check what happens with -Inf as x");
}
ATF_TC_BODY(test_sqrt4, tc)
{
	float fy;
	double dy;
	long double ldy;
	int haserrexcept;
	int haserrno;
	size_t i, N;

	N = sizeof(t4table) / sizeof(t4table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		fy = sqrtf((float)t4table[i]);
		ATF_CHECK_IFNAN(fy);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = sqrt((double)t4table[i]);
		ATF_CHECK_IFNAN(dy);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_SQRTL
		errno = 0;
		clear_exceptions();
		ldy = sqrtl(t4table[i]);
		ATF_CHECK_IFNAN(ldy);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
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

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_sqrt1);
	ATF_TP_ADD_TC(tp, test_sqrt2);
	ATF_TP_ADD_TC(tp, test_sqrt3);
	ATF_TP_ADD_TC(tp, test_sqrt4);

	return atf_no_error();
}
