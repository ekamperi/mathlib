#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_exp2.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_exp21);
ATF_TC_HEAD(test_exp21, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, exp2(x)) pairs");
}
ATF_TC_BODY(test_exp21, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fpreal_equal(
			    exp2(t1dtable[i].x),
				 t1dtable[i].y));
	}

	/* long double */
#ifdef  HAVE_EXP2L
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fpreal_equall(
			    exp2l(t1ldtable[i].x),
				  t1ldtable[i].y));
	}
#endif
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;	/* Input */
	long double y;	/* exp2 output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, 1 shall be returned */
	{  0.0, 1.0 },
	{ -0.0, 1.0 },

	/* If x is +Inf, x shall be returned */
#ifdef	INFINITY
	{  INFINITY, INFINITY },
#endif
#ifdef	HUGE_VAL
	{ HUGE_VAL, HUGE_VAL },
#endif
#ifdef	HUGE_VALF
	{  HUGE_VALF, HUGE_VALF },
#endif
#ifdef	HUGE_VALL
	{  HUGE_VALL, HUGE_VALL }
#endif
};

ATF_TC(test_exp22);
ATF_TC_HEAD(test_exp22, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_exp22, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpreal_equalf(
			    exp2f((float)t2table[i].x),
				  (float)t2table[i].y));

		/* double */
		ATF_CHECK(fpreal_equal(
			    exp2((double)t2table[i].x),
				 (double)t2table[i].y));

		/* long double */
#ifdef	HAVE_EXP2L
		ATF_CHECK(fpreal_equall(
			    exp2l(t2table[i].x),
				  t2table[i].y));
#endif
	}
}

ATF_TC(test_exp23);
ATF_TC_HEAD(test_exp23, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check for under/overflow");
}
ATF_TC_BODY(test_exp23, tc)
{
	double dx;
	long i, N;
	int haserrexcept;
	int haserrno;

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/*
	 * For IEEE Std 754-1985 double, 1024 <= x implies exp2(x) has
	 * overflowed. The value x < -1022 implies exp2(x) has underflowed.
	 */
	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		do {
			dx = random_double(FP_NORMAL);
		} while (dx >= -1022 && dx < 1024);

		errno = 0;
		clear_exceptions();
		(void)exp2(dx);

		ATF_PASS_OR_BREAK(errno_equalto(ERANGE));
		if (dx < 0.0)
			ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_UNDERFLOW));
		else
			ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_OVERFLOW));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_exp21);
	ATF_TP_ADD_TC(tp, test_exp22);
	ATF_TP_ADD_TC(tp, test_exp23);

	return atf_no_error();
}
