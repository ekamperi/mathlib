#define _XOPEN_SOURCE 600

#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

#include <atf-c.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static struct
tentry {
	long double x;       /* Input */
	long double y;       /* ilogb() output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_ilogb1);
ATF_TC_HEAD(test_ilogb1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, ilogb(x)) pairs");
}
ATF_TC_BODY(test_ilogb1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equal(ilogb(ttable[i].x), ttable[i].y));
}

/*
 * Test case 2 -- Domain error
 *
 */
static struct
t2entry {
	long double x;
	long double y;
} t2table[] = {
	/*
	 * If x is 0, a domain error shall occur,
	 * and the value FP_ILOGB0 shall be returned.
	 */
#ifdef	FP_ILOGB0
	{ 0.0, FP_ILOGB0 },
#endif

	/*
	 * If x is +-Inf, a domain error shall occur,
	 * and the value {INT_MAX} shall be returned.
	 */
#ifdef	INFINITY
	{  INFINITY, INT_MAX },
	{ -INFINITY, INT_MAX },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL, INT_MAX },
	{ -HUGE_VAL, INT_MAX },
#endif
#ifdef	HUGE_VALF
	{  HUGE_VALF, INT_MAX },
	{ -HUGE_VALF, INT_MAX },
#endif
#ifdef	HUGE_VALL
	{  HUGE_VALL, INT_MAX },
	{ -HUGE_VALL, INT_MAX },
#endif

	/*
	 * If x is a NaN, a domain error shall occur,
	 * and the value FP_ILOGBNAN shall be returned.
	 */
#ifdef	FP_ILOGBNAN
	{ NAN, FP_ILOGBNAN },
#endif
};

ATF_TC(test_ilogb2);
ATF_TC_HEAD(test_ilogb2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Domain errors");
}
ATF_TC_BODY(test_ilogb2, tc)
{
	int haserrexcept;
	int haserrno;
	long i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		ATF_CHECK(fpcmp_equalf(
			    ilogbf((float)t2table[i].x),
				   (float)t2table[i].y));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		ATF_CHECK(fpcmp_equal(
			    ilogb((double)t2table[i].x),
				  (double)t2table[i].y));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
		errno = 0;
		clear_exceptions();
		ATF_CHECK(fpcmp_equall(
			    ilogbl(t2table[i].x),
				   t2table[i].y));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
	}

	/*
	 * Revenge is a Dish Best Served Cold :)
	 *
	 * We put this check here and not in the beginning of the test case,
	 * because we don't want the lack of error handling support to
	 * block the equality checks. Nor do we want to split the test
	 * case into 2.
	 */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);
}

/*
 * Test case 3
 *
 * If the correct value is greater than {INT_MAX}, {INT_MAX}
 * shall be returned and a domain error shall occur.
 */
ATF_TC(test_ilogb3);
ATF_TC_HEAD(test_ilogb3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Domain errors");
}
ATF_TC_BODY(test_ilogb3, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;
	int haserrexcept;
	int haserrno;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		do {
			fx = random_float(FP_NORMAL);
		} while (fx <= INT_MAX);
		errno = 0;
		clear_exceptions();
		ATF_PASS_OR_BREAK(fpcmp_equalf(ilogbf(fx), INT_MAX));
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* double */
		do {
			dx = random_float(FP_NORMAL);
		} while (dx <= INT_MAX);
		errno = 0;
		clear_exceptions();
		ATF_PASS_OR_BREAK(fpcmp_equal(ilogb(dx), INT_MAX));
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* long double */
		do {
			ldx = random_long_double(FP_NORMAL);
		} while (ldx <= INT_MAX);
		errno = 0;
		clear_exceptions();
		ATF_PASS_OR_BREAK(fpcmp_equall(ilogbl(ldx), INT_MAX));
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));
	}

	/*
	 * Revenge is a Dish Best Served Cold :)
	 *
	 * We put this check here and not in the beginning of the test case,
	 * because we don't want the lack of error handling support to
	 * block the equality checks. Nor do we want to split the test
	 * case into 2.
	 */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);
}

/*
 * Test case 4
 *
 * If the correct value is less than {INT_MIN},
 * {INT_MIN} shall be returned and a domain error
 * shall occur.
 */
ATF_TC(test_ilogb4);
ATF_TC_HEAD(test_ilogb4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Domain errors");
}
ATF_TC_BODY(test_ilogb4, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;
	int haserrexcept;
	int haserrno;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		do {
			fx = random_float(FP_NORMAL);
		} while (fx >= INT_MIN);
		errno = 0;
		clear_exceptions();
		ATF_PASS_OR_BREAK(fpcmp_equalf(ilogbf(fx), INT_MIN));
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* double */
		do {
			dx = random_float(FP_NORMAL);
		} while (dx >= INT_MIN);
		errno = 0;
		clear_exceptions();
		ATF_PASS_OR_BREAK(fpcmp_equal(ilogb(dx), INT_MIN));
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* long double */
		do {
			ldx = random_long_double(FP_NORMAL);
		} while (ldx >= INT_MIN);
		errno = 0;
		clear_exceptions();
		ATF_PASS_OR_BREAK(fpcmp_equall(ilogbl(ldx), INT_MIN));
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));
	}

	/*
	 * Revenge is a Dish Best Served Cold :)
	 *
	 * We put this check here and not in the beginning of the test case,
	 * because we don't want the lack of error handling support to
	 * block the equality checks. Nor do we want to split the test
	 * case into 2.
	 */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_ilogb1);
	ATF_TP_ADD_TC(tp, test_ilogb2);
	ATF_TP_ADD_TC(tp, test_ilogb3);
	ATF_TP_ADD_TC(tp, test_ilogb4);

	return atf_no_error();
}
