#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <float.h>	/* for DBL_MAX */
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_tgamma.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_tgamma1);
ATF_TC_HEAD(test_tgamma1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, tgamma(x)) pairs");
}
ATF_TC_BODY(test_tgamma1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equal(
			    tgamma(t1dtable[i].x),
				   t1dtable[i].y));


	/* long double */
#ifdef	HAVE_TGAMMAL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equall(
			    tgammal(t1ldtable[i].x),
				    t1ldtable[i].y));
#endif
}

/*
 * Test case 2 -- Identities
 */
ATF_TC(test_tgamma2);
ATF_TC_HEAD(test_tgamma2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check if G(z+1) = zG(z) identity holds for random input");
}
ATF_TC_BODY(test_tgamma2, tc)
{
	long i, N, cnt;
	int haserrexcept;
	int haserrno;

	/*
	 * We can't proceed if there's no way to detect errors,
	 * especially overflows.
	 */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/* First for real numbers */
	init_randgen();

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	cnt = 0;
	ATF_FOR_LOOP(i, N, i++) {
		errno = 0;
		clear_exceptions();

		double z, g0, g1;
		z = random_double(FP_NORMAL);

		g0 = tgamma(z);
		g1 = tgamma(z+1);

		/* If there was an error, just skip this value */
		if (raised_exceptions(MY_FE_ALL_EXCEPT) || errno)
			continue;

		ATF_PASS_OR_BREAK(fpreal_equal(g1, z*g0));
		++cnt;
	}
	ATF_CHECK(cnt > i/2);
}

/*
 * Test case 3 -- Edge cases
 */
ATF_TC(test_tgamma3);
ATF_TC_HEAD(test_tgamma3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_tgamma3, tc)
{
	/* If x is NaN, a NaN shall be returned */
	ATF_CHECK_IFNAN(tgamma(NAN));
	ATF_CHECK_IFNAN(tgammaf(NAN));
#ifdef	HAVE_TGAMMAL
	ATF_CHECK_IFNAN(tgammal(NAN));
#endif	/* HAVE_TGAMMAL */

	/* If x is +Inf, x shall be returned */
#ifdef	INFINITY
	ATF_CHECK(isinf(tgamma(INFINITY)));
	ATF_CHECK(isinf(tgammaf(INFINITY)));
#ifdef	HAVE_TGAMMAL
	ATF_CHECK(isinf(tgammal(INFINITY)));
#endif  /* HAVE_TGAMMAL */
#endif

#ifdef	HUGE_VAL
	ATF_CHECK(isinf(tgamma(HUGE_VAL)));
	ATF_CHECK(isinf(tgammaf(HUGE_VAL)));
#ifdef	HAVE_TGAMMAL
	ATF_CHECK(isinf(tgammal(HUGE_VAL)));
#endif  /* HAVE_TGAMMAL */
#endif

#ifdef	HUGE_VALF
	ATF_CHECK(isinf(tgamma(HUGE_VALF)));
	ATF_CHECK(isinf(tgammaf(HUGE_VAL)));
#ifdef	HAVE_TGAMMAL
	ATF_CHECK(isinf(tgammal(HUGE_VALL)));
#endif  /* HAVE_TGAMMAL */
#endif

#ifdef	HUGE_VALL
	ATF_CHECK(isinf(tgamma(HUGE_VALL)));
	ATF_CHECK(isinf(tgammaf(HUGE_VALF)));
#ifdef	HAVE_TGAMMAL
	ATF_CHECK(isinf(tgammal(HUGE_VALL)));
#endif  /* HAVE_TGAMMAL */
#endif
}

/*
 * Test case 4 -- Pole error
 */
long t4table[] = { +0.0, -0.0 };

ATF_TC(test_tgamma4);
ATF_TC_HEAD(test_tgamma4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x is +-0, a pole error shall occur");
}
ATF_TC_BODY(test_tgamma4, tc)
{
	float fy;
	double dy;
	long double ldy;
	int haserrexcept;
	int haserrno;
	int i;

	for (i = 0; i < 2; i++ ) {
		/* float */
		errno = 0;
		clear_exceptions();
		fy = tgammaf((float)t4table[i]);
#ifdef  HUGE_VALF
		ATF_CHECK(fy == HUGE_VALF);
#endif
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = tgamma((double)t4table[i]);
#ifdef	HUGE_VAL
		ATF_CHECK(dy == HUGE_VAL);
#endif
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		/* long double */
#ifdef	HAVE_TGAMMAL
		errno = 0;
		clear_exceptions();
		ldy = tgammal(t4table[i]);
#ifdef  HUGE_VALL
		ATF_CHECK(ldy == HUGE_VALL);
#endif
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));
#endif	/* HAVE_TGAMMAL */
	}

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
 * Test case 5 -- Domain error
 */
static const long double
t5table[] = {
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

ATF_TC(test_tgamma5);
ATF_TC_HEAD(test_tgamma5, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x is -Inf, a domain error shall occur");
}
ATF_TC_BODY(test_tgamma5, tc)
{
	int haserrexcept;
	int haserrno;
	int i, N;
	float fy;
	double dy;
	long double ldy;

	N = sizeof(t5table) / sizeof(t5table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		fy = tgammaf((float)t5table[i]);
		ATF_CHECK_IFNAN(fy);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = tgamma((double)t5table[i]);
		ATF_CHECK_IFNAN(dy);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_TGAMMAL
		errno = 0;
		clear_exceptions();
		ldy = tgammal(t5table[i]);
		ATF_CHECK_IFNAN(ldy);
		ATF_CHECK(errno_equalto(EDOM));
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
 * Test case 6 -- Deliberate overflow
 */
ATF_TC(test_tgamma6);
ATF_TC_HEAD(test_tgamma6, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "For IEEE Std 754-1985 double, overflow happens when "
	    "0 < x < 1/DBL_MAX, and 171.7 < x");
}
ATF_TC_BODY(test_tgamma6, tc)
{
	double x;
	long i, N;
	int haserrexcept;
	int haserrno;

	/*
	 * We can't proceed if there's no way to detect errors,
	 * especially overflows.
	 */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		do {
			x = random_double(FP_NORMAL);
		} while (x <= 171.7 && (x <= 0 || x >= 1/DBL_MAX));

		errno = 0;
		clear_exceptions();
		(void)tgamma(x);
		ATF_PASS_OR_BREAK(errno_equalto(ERANGE));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_OVERFLOW));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_tgamma1);
	ATF_TP_ADD_TC(tp, test_tgamma2);
	ATF_TP_ADD_TC(tp, test_tgamma3);
	ATF_TP_ADD_TC(tp, test_tgamma4);
	ATF_TP_ADD_TC(tp, test_tgamma5);
	ATF_TP_ADD_TC(tp, test_tgamma6);

	return atf_no_error();
}
