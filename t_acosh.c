#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "t_acosh.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_acosh1);
ATF_TC_HEAD(test_acosh1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, acosh(x)) pairs");
}
ATF_TC_BODY(test_acosh1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equal(
			    acosh(t1dtable[i].x),
				  t1dtable[i].y));

	/* long double */
        N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
        for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equall(
			    acoshl(t1ldtable[i].x),
				   t1ldtable[i].y));
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;       /* Input */
	long double y;       /* acosh output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +1, +0 shall be returned */
	{ 1.0, +0.0 },

	/* If x is +Inf, +Inf shall be returned */
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
	{ HUGE_VALL, HUGE_VALL }
#endif
};

ATF_TC(test_acosh2);
ATF_TC_HEAD(test_acosh2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_acosh2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpcmp_equalf(
			    acoshf((float)t2table[i].x),
				   (float)t2table[i].y));

		/* double */
		ATF_CHECK(fpcmp_equal(
			    acosh((double)t2table[i].x),
				  (double)t2table[i].y));

		/* long double */
#ifdef	HAVE_ACOSHL
		ATF_CHECK(fpcmp_equall(
			    acoshl(t2table[i].x),
				   t2table[i].y));
#endif
	}
}

/*
 * Test case 3
 *
 * If x is -Inf, a domain error shall occur, and either a NaN (if supported),
 * or an implementation-defined value shall be returned.
 */
ATF_TC(test_acosh3);
ATF_TC_HEAD(test_acosh3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_acosh3, tc)
{
	float fy;
	double dy;
	long double ldy;
	size_t i, N;

	/* We are re-using t2table[] data, but we keep only the infinities */
	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		if (isinf(t2table[i].x)) {
			/* float */
			errno = 0;
			clear_exceptions();
			fy = acoshf((float)t2table[i].x);
			ATF_CHECK_IFNAN(fy);
			ATF_CHECK(iserrno_equalto(EDOM));
			ATF_CHECK(raised_exceptions(MY_FE_INVALID));

			/* double */
			errno = 0;
			clear_exceptions();
			dy = acosh((double)t2table[i].x);
			ATF_CHECK_IFNAN(dy);
			ATF_CHECK(iserrno_equalto(EDOM));
			ATF_CHECK(raised_exceptions(MY_FE_INVALID));

			/* long double */
#ifdef	HAVE_ACOSHL
			errno = 0;
			clear_exceptions();
			ldy = acoshl(t2table[i].x);
			ATF_CHECK_IFNAN(ldy);
			ATF_CHECK(iserrno_equalto(EDOM));
			ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
		}
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_acosh1);
	ATF_TP_ADD_TC(tp, test_acosh2);
	ATF_TP_ADD_TC(tp, test_acosh3);

	return atf_no_error();
}
