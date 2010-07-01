#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

static const struct
tentry {
	double x;       /* Input */
	double y;       /* rint output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_rint1);
ATF_TC_HEAD(test_rint1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, rint(x)) pairs");
}
ATF_TC_BODY(test_rint1, tc)
{
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;	/* Input */
	long double y;	/* Output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, x shall be returned */
	{  0.0,  0.0 },
	{ -0.0, -0.0 },

	/* If x is +-Inf, x shall be returned */
#ifdef	INFINITY
	{  INFINITY,  INFINITY },
	{ -INFINITY, -INFINITY },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL,  HUGE_VAL },
	{ -HUGE_VAL, -HUGE_VAL },
#endif
#ifdef	HUGE_VALF
	{  HUGE_VALF,  HUGE_VALF },
	{ -HUGE_VALF, -HUGE_VALF },
#endif
#ifdef	HUGE_VALL
	{  HUGE_VALL,  HUGE_VALL },
	{ -HUGE_VALL, -HUGE_VALL }
#endif
};
ATF_TC(test_rint2);
ATF_TC_HEAD(test_rint2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_rint2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	/*
	 * Since we are here, check also that no spurious
	 * errors are generated.
	 */
	errno = 0;
	clear_exceptions();

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpcmp_equalf(
			    rintf((float)t2table[i].x),
				  (float)t2table[i].y));

		/* double */
		ATF_CHECK(fpcmp_equal(
			    rint((double)t2table[i].x),
				 (double)t2table[i].y));

		/* long double */
#ifdef	HAVE_RINTL
		ATF_CHECK(fpcmp_equall(
			    rintl(t2table[i].x),
				  t2table[i].y));
#endif
	}

	ATF_CHECK(errno == 0);
	ATF_CHECK(not_raised_exceptions(MY_FE_ALL_EXCEPT));
}

/*
 * Test case 3
 *
 * The rint*() functions return the integral value (represented as a double)
 * nearest x in the direction of the current rounding mode. Therefore, if
 * an integral value is given, the same shall be returned back.
 */
ATF_TC(test_rint3);
ATF_TC_HEAD(test_rint3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Fuzzy testing");
}
ATF_TC_BODY(test_rint3, tc)
{
	long i, N, x;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	/* Initialize random number generator */
	srand48(time(NULL));

	/*
	 * Since we are here, check also that no spurious
	 * errors are generated.
	 */
	errno = 0;
	clear_exceptions();

	ATF_FOR_LOOP(i, N, i++) {
		x = mrand48();

		ATF_PASS_OR_BREAK(rintf((float)x) == (float)x);
		ATF_PASS_OR_BREAK(rint((double)x) == (double)x);
#ifdef	HAVE_RINTL
		ATF_PASS_OR_BREAK(rintl((long double)x == (long double)x));
#endif
	}

	ATF_CHECK(errno == 0);
	ATF_CHECK(not_raised_exceptions(MY_FE_ALL_EXCEPT));
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_rint1);
	ATF_TP_ADD_TC(tp, test_rint2);
	ATF_TP_ADD_TC(tp, test_rint3);

	return atf_no_error();
}
