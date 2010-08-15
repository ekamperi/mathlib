#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_log.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_log1);
ATF_TC_HEAD(test_log1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, log(x)) pairs");
}
ATF_TC_BODY(test_log1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equal(
			    log(t1dtable[i].x),
				t1dtable[i].y));

	/* long double */
#ifdef	HAVE_LOGL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equall(
			    logl(t1ldtable[i].x),
				 t1ldtable[i].y));
#endif
}

/*
 * Test case 2 -- Pole error
 *
 * If x is +-0, a pole error shall occur and log(), logf(), and logl() shall
 * return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
 */
static const long t2table[] = { +0.0, -0.0 };

ATF_TC(test_log2);
ATF_TC_HEAD(test_log2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x is +-0, a pole error shall occur");
}
ATF_TC_BODY(test_log2, tc)
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
		fy = logf((float)t2table[i]);
#ifdef  HUGE_VALF
		ATF_CHECK(fy == -HUGE_VALF);
#endif
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = log((double)t2table[i]);
#ifdef	HUGE_VAL
		ATF_CHECK(dy == -HUGE_VAL);
#endif
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		/* long double */
#ifdef	HAVE_LOGL
		errno = 0;
		clear_exceptions();
		ldy = logl(t2table[i]);
#ifdef  HUGE_VALL
		ATF_CHECK(ldy == -HUGE_VALL);
#endif
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));
#endif	/* HAVE_LOGL */
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

ATF_TC(test_log3);
ATF_TC_HEAD(test_log3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x < 0, a domain error shall occur");
}
ATF_TC_BODY(test_log3, tc)
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
		fy = logf((float)t3table[i]);
		ATF_CHECK_IFNAN(fy);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = log((double)t3table[i]);
		ATF_CHECK_IFNAN(dy);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_LOGL
		errno = 0;
		clear_exceptions();
		ldy = logl(t3table[i]);
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
		fy = logf((float)t3table[i]);
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
		dy = log((double)t3table[i]);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(dy));
#endif
		ATF_PASS_OR_BREAK(errno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_LOGL
		do {
			ldx = random_long_double(FP_NORMAL);
		} while (ldx >= 0.0);
		errno = 0;
		clear_exceptions();
		ldy = logl(t3table[i]);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(ldy));
#endif
		ATF_PASS_OR_BREAK(errno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));
#endif	/* HAVE_LOG10 */
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
ATF_TC(test_log4);
ATF_TC_HEAD(test_log4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_log4, tc)
{
	/* If x is NaN, a NaN shall be returned */
	ATF_CHECK_IFNAN(logf(NAN));
	ATF_CHECK_IFNAN(log(NAN));
#ifdef	HAVE_LOGL
	ATF_CHECK_IFNAN(logl(NAN));
#endif

	/* If x is 1, +0 shall be returned */
	ATF_CHECK(fpreal_equalf(logf(1.0), 0.0));
	ATF_CHECK(fpreal_equal (log (1.0), 0.0));
#ifdef	HAVE_LOGL
	ATF_CHECK(fpreal_equall(logl(1.0), 0.0));
#endif

	/* If x is +Inf, x shall be returned */
#ifdef	INFINITY
	ATF_CHECK(fpreal_equalf(logf(INFINITY), INFINITY));
	ATF_CHECK(fpreal_equal (log (INFINITY), INFINITY));
#ifdef	HAVE_LOGL
	ATF_CHECK(fpreal_equall(logl(INFINITY), INFINITY));
#endif	/* HAVE_LOGL */
#endif

#ifdef  HUGE_VAL
	ATF_CHECK(fpreal_equalf(logf(HUGE_VAL), HUGE_VAL));
	ATF_CHECK(fpreal_equal( log (HUGE_VAL), HUGE_VAL));
#ifdef	HAVE_LOGL
	ATF_CHECK(fpreal_equall(logl(HUGE_VAL), HUGE_VAL));
#endif  /* HAVE_LOGL */
#endif

#ifdef	HUGE_VALF
	ATF_CHECK(fpreal_equalf(logf(HUGE_VALF), HUGE_VALF));
	ATF_CHECK(fpreal_equal( log( HUGE_VALF), HUGE_VALF));
#ifdef	HAVE_LOGL
	ATF_CHECK(fpreal_equall(logl(HUGE_VALF), HUGE_VALF));
#endif  /* HAVE_LOGL */
#endif

#ifdef  HUGE_VALL
	ATF_CHECK(fpreal_equalf(logf(HUGE_VALL), HUGE_VALL));
	ATF_CHECK(fpreal_equal( log( HUGE_VALL), HUGE_VALL));
#ifdef	HAVE_LOGLL
	ATF_CHECK(fpreal_equall(logl(HUGE_VALL), HUGE_VALL));
#endif  /* HAVE_LOGL */
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_log1);
	ATF_TP_ADD_TC(tp, test_log2);
	ATF_TP_ADD_TC(tp, test_log3);
	ATF_TP_ADD_TC(tp, test_log4);

	return atf_no_error();
}
