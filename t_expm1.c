#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_expm1.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_expm11);
ATF_TC_HEAD(test_expm11, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, expm1(x)) pairs");
}
ATF_TC_BODY(test_expm11, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equal(
			    expm1(t1dtable[i].x),
				  t1dtable[i].y));

  /* long double */
#ifdef  HAVE_EXPM1L
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equall(
			    expm1l(t1ldtable[i].x),
				   t1ldtable[i].y));
	}
#endif
}

/*
 * Test case 2 -- Edge cases
 */
struct t2entry {
	long double x;	/* Input */
	long double y;	/* expm1 output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, 1 shall be returned */
	{  0.0, 1.0 },
	{ -0.0, 1.0 },

	/* If x is -Inf, -1 shall be returned */
	/* If x is +Inf, x shall be returned */
#ifdef	INFINITY
	{ -INFINITY, -1.0     },
	{  INFINITY, INFINITY },
#endif
#ifdef	HUGE_VAL
	{ -HUGE_VAL, -1.0     },
	{  HUGE_VAL, HUGE_VAL },
#endif
#ifdef	HUGE_VALF
	{ -HUGE_VALF, -1.0      },
	{  HUGE_VALF, HUGE_VALF },
#endif
#ifdef	HUGE_VALL
	{ -HUGE_VALL, -1.0	},
	{  HUGE_VALL, HUGE_VALL }
#endif
};

ATF_TC(test_expm12);
ATF_TC_HEAD(test_expm12, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_expm12, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpcmp_equal(
			    expm1f((float)t2table[i].x),
				   (float)t2table[i].y));

		/* double */
		ATF_CHECK(fpcmp_equal(
			    expm1((double)t2table[i].x),
				  (double)t2table[i].y));

		/* long double */
#ifdef	HAVE_EXPM1
		ATF_CHECK(fpcmp_equal(
			    expm1l(t2table[i].x),
				   t2table[i].y));
#endif
	}
}

ATF_TC(test_expm13);
ATF_TC_HEAD(test_expm13, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check for overflow");
}
ATF_TC_BODY(test_expm13, tc)
{
	double dx;
	long i, N;
	int haserrexcept;
	int haserrno;

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/*
	 * For IEEE Std 754-1985 double, 709.8 < x implies expm1(x) has
	 * overflowed.
	 */
	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		do {
			dx = random_double(FP_NORMAL);
		} while (dx >= 709.8);

		errno = 0;
		clear_exceptions();
		(void)expm1(dx);

		ATF_PASS_OR_BREAK(iserrno_equalto(ERANGE));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_OVERFLOW));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_expm11);
	ATF_TP_ADD_TC(tp, test_expm12);
	ATF_TP_ADD_TC(tp, test_expm13);

	return atf_no_error();
}
