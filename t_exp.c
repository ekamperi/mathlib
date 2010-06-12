#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <float.h>	/* DBL_MIN */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_exp.h"

/*
 * Test case 1 -- Basic functionality
 */
struct t1entry {
	long double x;       /* Input */
	long double y;       /* exp output */
} t1table[] = {
};

ATF_TC(test_exp1);
ATF_TC_HEAD(test_exp1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, exp(x)) pairs");
}
ATF_TC_BODY(test_exp1, tc)
{
	float fy;
	size_t i, N;

	N = sizeof(t1ftable) / sizeof(t1ftable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equal(expf(t1ftable[i].x), t1ftable[i].y));
		if (!fpcmp_equal(expf(t1ftable[i].x), t1ftable[i].y)) {
			printf("%.16e\t%.16e\n", expf(t1ftable[i].x), t1ftable[i].y);
			fflush(NULL);
		}
	}
}

/*
 * Test case 2 -- Edge cases
 */
struct t2entry {
	long double x;	/* Input */
	long double y;	/* exp output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, 1 shall be returned */
	{  0.0, 1.0 },
	{ -0.0, 1.0 },

	/* If x is -Inf, +0 shall be returned */
	/* If x is +Inf, x shall be returned */
#ifdef	INFINITY
	{ -INFINITY, 0.0 },
	{  INFINITY, INFINITY },
#endif
#ifdef	HUGE_VAL
	{ -HUGE_VAL, 0.0 },
	{ HUGE_VAL, HUGE_VAL },
#endif
#ifdef	HUGE_VALF
	{ -HUGE_VALF, 0.0 },
	{  HUGE_VALF, HUGE_VALF },
#endif
#ifdef	HUGE_VALL
	{ -HUGE_VALL, 0.0 },
	{  HUGE_VALL, HUGE_VALL }
#endif
};

ATF_TC(test_exp2);
ATF_TC_HEAD(test_exp2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_exp2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equal(
			    expf((float)t2table[i].x),
			    t2table[i].y));

		ATF_CHECK(fpcmp_equal(
			    exp((double)t2table[i].x),
			    t2table[i].y));

		ATF_CHECK(fpcmp_equal(
			    expl(t2table[i].x),
			    t2table[i].y));
	}
}

ATF_TC(test_exp3);
ATF_TC_HEAD(test_exp3, tc)
{
  atf_tc_set_md_var(tc,
		    "descr",
		    "Check for undexplow");
}
ATF_TC_BODY(test_exp3, tc)
{
	double dx;
	long i, N;
	int haserrexcept;
	int haserrno;

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/*
	 * For IEEE Std 754-1985 double, 709.8 < x implies exp(x) has overflowed.
	 * The value x < -708.4 implies exp(x) has underflowed.
	 */
	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		do {
			dx = random_double(FP_NORMAL);
		} while (dx > -708.4 && dx < 709.8);

		errno = 0;
		clear_exceptions();
		(void)exp(dx);

		ATF_CHECK(iserrno_equalto(ERANGE));
		if (dx < 0.0)
			ATF_CHECK(raised_exceptions(MY_FE_UNDERFLOW));
		else
			ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_exp1);
	ATF_TP_ADD_TC(tp, test_exp2);
	ATF_TP_ADD_TC(tp, test_exp3);

	return atf_no_error();
}
