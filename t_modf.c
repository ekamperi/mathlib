#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1 -- Basic functionality
 */
static const struct
t1entry {
	long double x;	/* Input */
	long double y;
} t1table[] = {
};

ATF_TC(test_modf1);
ATF_TC_HEAD(test_modf1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, modf(x)) pairs");
}
ATF_TC_BODY(test_modf1, tc)
{
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++) {
	}
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;		/* Input */
	long double y;		/* Output */
	long double iptr;
} t2table[] = {
	/*
	 * If x is NaN, a NaN shall be returned,
	 * and *iptr shall be set to a NaN
	 */
#ifdef	NAN
	{ NAN, NAN, NAN },
#endif

	/*
	 * If x is +-Inf, +-0 shall be returned,
	 * and *iptr shall be set to +-Inf
	 */
#ifdef	INFINITY
	{  INFINITY,  0.0,  INFINITY },
	{ -INFINITY, -0.0, -INFINITY },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL,  0.0,  HUGE_VAL },
	{ -HUGE_VAL, -0.0, -HUGE_VAL },
#endif
#ifdef	HUGE_VALF
	{  HUGE_VALF,  0.0,  HUGE_VALF },
	{ -HUGE_VALF, -0.0, -HUGE_VALF },
#endif
#ifdef	HUGE_VALL
	{  HUGE_VALL,  0.0,  HUGE_VALL },
	{ -HUGE_VALL, -0.0, -HUGE_VALL }
#endif
};

ATF_TC(test_modf2);
ATF_TC_HEAD(test_modf2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_modf2, tc)
{
	float fy, fi;
	double dy, di;
	long double ldy, ldi;
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fy = modff((float)t2table[i].x, &fi);
		ATF_CHECK(fpreal_equalf(fy, (float)t2table[i].y));
		ATF_CHECK(fpreal_equalf(fi, (float)t2table[i].iptr));

		/* double */
                dy = modf((double)t2table[i].x, &di);
                ATF_CHECK(fpreal_equal(dy, (double)t2table[i].y));
                ATF_CHECK(fpreal_equal(di, (double)t2table[i].iptr));

		/* long double */
#ifdef	HAVE_MODFL
                ldy = modfl((float)t2table[i].x, &ldi);
                ATF_CHECK(fpreal_equalf(ldy, t2table[i].y));
                ATF_CHECK(fpreal_equalf(ldi, t2table[i].iptr));
#endif
	}
}

/*
 * Test case 3 -- A little bit fuzzing
 */
ATF_TC(test_modf3);
ATF_TC_HEAD(test_modf3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check that integral and fractional parts have correct sign");
}
ATF_TC_BODY(test_modf3, tc)
{
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */

		/* double */

		/* long double */
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_modf1);
	ATF_TP_ADD_TC(tp, test_modf2);
	ATF_TP_ADD_TC(tp, test_modf3);

	return atf_no_error();
}
