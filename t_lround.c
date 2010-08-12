#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <limits.h>	/* LONG_MAX */
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

static const struct
tentry {
	double x;       /* Input */
	double y;       /* lround output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_lround1);
ATF_TC_HEAD(test_lround1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, lround(x)) pairs");
}
ATF_TC_BODY(test_lround1, tc)
{
}

/*
 * Test case 2 -- Domain errors
 */
static const long double
t2table[] = {
	/*
	 * If x is NaN, a domain error shall occur and
	 * an unspecified value is returned
	 */
#ifdef	NAN
	NAN,
#endif

	/*
	 * If x is +-Inf, a domain error shall occur
	 * and an unspecified value is returned
	 */
#ifdef  INFINITY
        INFINITY,
	-INFINITY,
#endif
#ifdef  HUGE_VAL
        HUGE_VAL,
	-HUGE_VAL,
#endif
#ifdef  HUGE_VALF
        HUGE_VALF,
	-HUGE_VALF,
#endif
#ifdef  HUGE_VALL
        HUGE_VALL,
	-HUGE_VALL
#endif

	/*
	 * If the correct value is positive and too large to represent
	 * as a long, a domain error shall occur and an unspecified
	 * value is returned
	 */
	+LONG_MAX + 1,

	/*
	 * If the correct value is negative and too large to represent
	 * as a long, a domain error shall occur and an unspecified
	 * value is returned
	 */
	-LONG_MAX - 1
};

ATF_TC(test_lround2);
ATF_TC_HEAD(test_lround2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check that domain errors are properly generated");
}
ATF_TC_BODY(test_lround2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		(void)lroundf((float)t2table[i]);
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		(void)lround((double)t2table[i]);
                ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_LROUNDL
		errno = 0;
		clear_exceptions();
		(void)lroundl(t2table[i]);
                ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_lround1);
	ATF_TP_ADD_TC(tp, test_lround2);

	return atf_no_error();
}
