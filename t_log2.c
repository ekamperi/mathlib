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
	long double y;       /* log2() output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_log21);
ATF_TC_HEAD(test_log21, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, log2(x)) pairs");
}
ATF_TC_BODY(test_log21, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equal(log2(ttable[i].x), ttable[i].y));
}

/*
 * Test case 2 -- Pole error
 *
 * If x is +-0, a pole error shall occur and log2(), log2f(), and log2l() shall
 * return -HUGE_VAL, -HUGE_VALF, and -HUGE_VALL, respectively.
 */
static long t2table[] = { +0.0, -0.0 };

ATF_TC(test_log22);
ATF_TC_HEAD(test_log22, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x is +-0, a pole error shall occur");
}
ATF_TC_BODY(test_log22, tc)
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
		fy = log2f((float)t2table[i]);
#ifdef  HUGE_VALF
		ATF_CHECK(fy == -HUGE_VALF);
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = log2((double)t2table[i]);
#ifdef	HUGE_VAL
		ATF_CHECK(dy == -HUGE_VAL);
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		/* long double */
#ifdef	HAVE_LOG2L
		errno = 0;
		clear_exceptions();
		ldy = log2l(t2table[i]);
#ifdef  HUGE_VALL
		ATF_CHECK(ldy == -HUGE_VALL);
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));
#endif	/* HAVE_LOG2L */
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
static long double
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

ATF_TC(test_log23);
ATF_TC_HEAD(test_log23, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x < 0, a domain error shall occur");
}
ATF_TC_BODY(test_log23, tc)
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
		fy = log2f((float)t3table[i]);
		ATF_CHECK_IFNAN(fy);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = log2((double)t3table[i]);
		ATF_CHECK_IFNAN(dy);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_LOG2L
		errno = 0;
		clear_exceptions();
		ldy = log2l(t3table[i]);
		ATF_CHECK_IFNAN(ldy);
		ATF_CHECK(iserrno_equalto(EDOM));
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
		fy = log2f((float)t3table[i]);
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
		dy = log2((double)t3table[i]);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(dy));
#endif
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_LOG2L
		do {
			ldx = random_long_double(FP_NORMAL);
		} while (ldx >= 0.0);
		errno = 0;
		clear_exceptions();
		ldy = log2l(t3table[i]);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(ldy));
#endif
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));
#endif	/* HAVE_LOG2L */
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
ATF_TC(test_log24);
ATF_TC_HEAD(test_log24, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_log24, tc)
{
	/* If x is NaN, a NaN shall be returned */
	ATF_CHECK_IFNAN(log2f(NAN));
	ATF_CHECK_IFNAN(log2(NAN));
	ATF_CHECK_IFNAN(log2l(NAN));

	/* If x is 1, +0 shall be returned */
	ATF_CHECK(fpcmp_equalf(log2f(1.0), 0.0));
	ATF_CHECK(fpcmp_equal (log2 (1.0), 0.0));
	ATF_CHECK(fpcmp_equall(log2l(1.0), 0.0));

	/* If x is +Inf, x shall be returned */
#ifdef	INFINITY
	ATF_CHECK(fpcmp_equalf(log2f(INFINITY), INFINITY));
	ATF_CHECK(fpcmp_equal (log2 (INFINITY), INFINITY));
#ifdef	HAVE_LOG2L
	ATF_CHECK(fpcmp_equall(log2l(INFINITY), INFINITY));
#endif	/* HAVE_LOG2L */
#endif

#ifdef  HUGE_VAL
	ATF_CHECK(fpcmp_equalf(log2f(HUGE_VAL), HUGE_VAL));
	ATF_CHECK(fpcmp_equal (log2 (HUGE_VAL), HUGE_VAL));
#ifdef	HAVE_LOG2L
	ATF_CHECK(fpcmp_equall(log2l(HUGE_VAL), HUGE_VAL));
#endif  /* HAVE_LOG2L */
#endif

#ifdef	HUGE_VALF
	ATF_CHECK(fpcmp_equalf(log2f(HUGE_VALF), HUGE_VALF));
	ATF_CHECK(fpcmp_equal (log2 (HUGE_VALF), HUGE_VALF));
#ifdef	HAVE_LOG2L
	ATF_CHECK(fpcmp_equall(log2l(HUGE_VALF), HUGE_VALF));
#endif  /* HAVE_LOG2L */
#endif

#ifdef  HUGE_VALL
	ATF_CHECK(fpcmp_equalf(log2f(HUGE_VALL), HUGE_VALL));
	ATF_CHECK(fpcmp_equal( log2 (HUGE_VALL), HUGE_VALL));
#ifdef	HAVE_LOG2L
	ATF_CHECK(fpcmp_equall(log2l(HUGE_VALL), HUGE_VALL));
#endif  /* HAVE_LOG2L */
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_log21);
	ATF_TP_ADD_TC(tp, test_log22);
	ATF_TP_ADD_TC(tp, test_log23);
	ATF_TP_ADD_TC(tp, test_log24);

	return atf_no_error();
}
