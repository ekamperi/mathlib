#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_erfc.h"

/* Don't be too harsh with symbols */
#ifndef M_PI_2
#define M_PI_2	1.57079632679489661923
#endif

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_erfc1);
ATF_TC_HEAD(test_erfc1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, erfc(x)) pairs");
}
ATF_TC_BODY(test_erfc1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equal(
			    erfc(t1dtable[i].x),
				 t1dtable[i].y));

	/* long double */
#ifdef	HAVE_ERFCL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equall(
			    erfcl(t1ldtable[i].x),
				  t1ldtable[i].y));
#endif
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;	/* Input */
	long double y;	/* erfc output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, +1 shall be returned */
	{  0.0, 1.0 },
	{ -0.0, 1.0 },

        /* If x is -Inf, +2 shall be returned */
        /* If x is +Inf, +0 shall be returned */
#ifdef	INFINITY
	{ -INFINITY, 2.0 },
	{  INFINITY, 0.0 },
#endif
#ifdef	HUGE_VAL
	{ -HUGE_VAL, 2.0 },
	{  HUGE_VAL, 0.0 },
#endif
#ifdef  HUGE_VALF
	{ -HUGE_VALF, 2.0 },
	{  HUGE_VALF, 0.0 },
#endif
#ifdef  HUGE_VALL
	{ -HUGE_VAL, 2.0 },
	{  HUGE_VAL, 0.0 }
#endif
};

ATF_TC(test_erfc2);
ATF_TC_HEAD(test_erfc2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_erfc2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpreal_equalf(
			    erfcf((float)t2table[i].x),
				  (float)t2table[i].y));

		/* double */
		ATF_CHECK(fpreal_equal(
			    erfc((double)t2table[i].x),
				 (double)t2table[i].y));

		/* long double */
#ifdef	HAVE_ERFCL
		ATF_CHECK(fpreal_equall(
			    erfcl(t2table[i].x),
				 t2table[i].y));
#endif
	}
}

ATF_TC(test_erfc3);
ATF_TC_HEAD(test_erfc3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check for underflow");
}
ATF_TC_BODY(test_erfc3, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;
	int haserrexcept;
	int haserrno;

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/*
	 * Note for IEEE Std 754-1985 double, 26.55 < x implies erfc(x) has
	 * underflowed. And if this happens, it _may_ result to a range error.
	 * Check the ~ of this instead.
	 *
	 * XXX: Allow FE_INEXACT ?
	 */
	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		do {
			fx = random_float(FP_NORMAL);
		} while (fabsf(fx) > 26.55);
		errno = 0;
		clear_exceptions();
		(void)erfcf(fx);
		ATF_PASS_OR_BREAK(errno == 0);
		ATF_PASS_OR_BREAK(not_raised_exceptions(MY_FE_ALL_EXCEPT));

		/* double */
		do {
			dx = random_double(FP_NORMAL);
		} while (fabs(dx) > 26.55);
		errno = 0;
		clear_exceptions();
		(void)erfc(dx);
		ATF_PASS_OR_BREAK(errno == 0);
		ATF_PASS_OR_BREAK(not_raised_exceptions(MY_FE_ALL_EXCEPT));

		/* long double */
#ifdef	HAVE_ERFCL
		do {
			ldx = random_long_double(FP_NORMAL);
		} while (fabsl(ldx) > 26.55);
		errno = 0;
		clear_exceptions();
		(void)erfcl(ldx);
		ATF_PASS_OR_BREAK(errno == 0);
		ATF_PASS_OR_BREAK(not_raised_exceptions(MY_FE_ALL_EXCEPT));
#endif
	}

	/* Ok, couldn't resist */
	/* float */
	errno = 0;
	clear_exceptions();
	(void)erfcf(26.55 + 1.0);
	ATF_CHECK(errno == 0 || errno == ERANGE);
	ATF_CHECK(not_raised_exceptions(MY_FE_ALL_EXCEPT) ||
		      raised_exceptions(MY_FE_UNDERFLOW));

	/* double */
	errno = 0;
        clear_exceptions();
        (void)erfc(26.55 + 1.0);
	ATF_CHECK(errno == 0 || errno == ERANGE);
        ATF_CHECK(not_raised_exceptions(MY_FE_ALL_EXCEPT) ||
		      raised_exceptions(MY_FE_UNDERFLOW));

	/* long double */
#ifdef	HAVE_ERFCL
	errno = 0;
        clear_exceptions();
        (void)erfcl(26.55 + 1.0);
        ATF_CHECK(errno == 0 || errno == ERANGE);
        ATF_CHECK(not_raised_exceptions(MY_FE_ALL_EXCEPT) ||
		      raised_exceptions(MY_FE_UNDERFLOW));
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_erfc1);
	ATF_TP_ADD_TC(tp, test_erfc2);
	ATF_TP_ADD_TC(tp, test_erfc3);

	return atf_no_error();
}
