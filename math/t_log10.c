#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_log10.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_log101);
ATF_TC_HEAD(test_log101, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, log10(x)) pairs");
}
ATF_TC_BODY(test_log101, tc)
{
	size_t i, N;
        /* double */
        N = sizeof(t1dtable) / sizeof(t1dtable[0]);
        ATF_REQUIRE(N > 0);
        for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equal(
			    log10(t1dtable[i].x),
				  t1dtable[i].y));

        /* long double */
#ifdef  HAVE_LOG10L
        N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
        ATF_REQUIRE(N > 0);
        for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equall(
			    log10l(t1ldtable[i].x),
				   t1ldtable[i].y));
#endif
}

/*
 * Test case 2 -- Pole error
 *
 * If x is +-0, a pole error shall occur and log10(), log10f(), and log10l() shall
 * return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
 */
static const long t2table[] = { +0.0, -0.0 };

ATF_TC(test_log102);
ATF_TC_HEAD(test_log102, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x is +-0, a pole error shall occur");
}
ATF_TC_BODY(test_log102, tc)
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
		fy = log10f((float)t2table[i]);
#ifdef  HUGE_VALF
		ATF_CHECK(fy == -HUGE_VALF);
#endif
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = log10((double)t2table[i]);
#ifdef	HUGE_VAL
		ATF_CHECK(dy == -HUGE_VAL);
#endif
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		/* long double */
#ifdef	HAVE_LOG10L
		errno = 0;
		clear_exceptions();
		ldy = log10l(t2table[i]);
#ifdef  HUGE_VALL
		ATF_CHECK(ldy == -HUGE_VALL);
#endif
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));
#endif	/* HAVE_LOG10L */
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
 * Test case 3 -- Domain error
 *
 * For finite values of x that are less than 0, or if x is -Inf, a domain error
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

ATF_TC(test_log103);
ATF_TC_HEAD(test_log103, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x < 0, a domain error shall occur");
}
ATF_TC_BODY(test_log103, tc)
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
		fy = log10f((float)t3table[i]);
		ATF_CHECK_IFNAN(fy);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = log10((double)t3table[i]);
		ATF_CHECK_IFNAN(dy);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_LOG10L
		errno = 0;
		clear_exceptions();
		ldy = log10l(t3table[i]);
		ATF_CHECK_IFNAN(ldy);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
	}

	/*
	 * Now try finite negative values
	 */
	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		do {
			fx = random_float(FP_NORMAL);
		} while (fx >= 0.0);
		errno = 0;
		clear_exceptions();
		fy = log10f((float)t3table[i]);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(fy));
#endif
		ATF_PASS_OR_BREAK(errno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* double */
		do {
			dx = random_double(FP_NORMAL);
		} while (dx >= 0.0);
		errno = 0;
		clear_exceptions();
		dy = log10((double)t3table[i]);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(dy));
#endif
		ATF_PASS_OR_BREAK(errno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_LOG10L
		do {
			ldx = random_long_double(FP_NORMAL);
		} while (ldx >= 0.0);
		errno = 0;
		clear_exceptions();
		ldy = log10l(t3table[i]);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(ldy));
#endif
		ATF_PASS_OR_BREAK(errno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));
#endif	/* HAVE_LOG10L */
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
ATF_TC(test_log104);
ATF_TC_HEAD(test_log104, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_log104, tc)
{
	/* If x is NaN, a NaN shall be returned */
	ATF_CHECK_IFNAN(log10f(NAN));
	ATF_CHECK_IFNAN(log10(NAN));
#ifdef	HAVEL_LOG10L
	ATF_CHECK_IFNAN(log10l(NAN));
#endif

	/* If x is 1, +0 shall be returned */
	ATF_CHECK(fpreal_equalf(log10f(1.0), 0.0));
	ATF_CHECK(fpreal_equal (log10 (1.0), 0.0));
#ifdef	HAVE_LOG10L
	ATF_CHECK(fpreal_equall(log10l(1.0), 0.0));
#endif

	/* If x is +Inf, x shall be returned */
#ifdef	INFINITY
	ATF_CHECK(fpreal_equalf(log10f(INFINITY), INFINITY));
	ATF_CHECK(fpreal_equal (log10 (INFINITY), INFINITY));
#ifdef	HAVE_LOG10L
	ATF_CHECK(fpreal_equall(log10l(INFINITY), INFINITY));
#endif	/* HAVE_LOG10L */
#endif

#ifdef  HUGE_VAL
	ATF_CHECK(fpreal_equalf(log10f(HUGE_VAL), HUGE_VAL));
	ATF_CHECK(fpreal_equal (log10 (HUGE_VAL), HUGE_VAL));
#ifdef	HAVE_LOG10L
	ATF_CHECK(fpreal_equall(log10l(HUGE_VAL), HUGE_VAL));
#endif  /* HAVE_LOG10L */
#endif

#ifdef	HUGE_VALF
	ATF_CHECK(fpreal_equalf(log10f(HUGE_VALF), HUGE_VALF));
	ATF_CHECK(fpreal_equal( log10 (HUGE_VALF), HUGE_VALF));
#ifdef	HAVE_LOG10L
	ATF_CHECK(fpreal_equall(log10l(HUGE_VALF), HUGE_VALF));
#endif  /* HAVE_LOG10L */
#endif

#ifdef  HUGE_VALL
	ATF_CHECK(fpreal_equalf(log10f(HUGE_VALL), HUGE_VALL));
	ATF_CHECK(fpreal_equal (log10 (HUGE_VALL), HUGE_VALL));
#ifdef	HAVE_LOG10L
	ATF_CHECK(fpreal_equall(log10l(HUGE_VALL), HUGE_VALL));
#endif  /* HAVE_LOG10L */
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_log101);
	ATF_TP_ADD_TC(tp, test_log102);
	ATF_TP_ADD_TC(tp, test_log103);
	ATF_TP_ADD_TC(tp, test_log104);

	return atf_no_error();
}
