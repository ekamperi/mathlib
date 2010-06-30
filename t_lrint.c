#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <limits.h>
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
	double y;       /* lrint output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_lrint1);
ATF_TC_HEAD(test_lrint1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, lrint(x)) pairs");
}
ATF_TC_BODY(test_lrint1, tc)
{
}

/*
 * Test case 2 -- Domain error
 */
const long double
t2table[] = {
	/* NAN */
#ifdef	NAN
	NAN,
#endif

	/* x is +-Inf */
#ifdef	INFINITY
	INFINITY,
	-INFINITY,
#endif
#ifdef	HUGE_VAL
	HUGE_VAL,
	-HUGE_VAL,
#endif
#ifdef	HUGE_VALF
	HUGE_VALF,
	-HUGE_VALF,
#endif
#ifdef	HUGE_VALL
	HUGE_VALL,
	-HUGE_VALL
#endif
};
ATF_TC(test_lrint2);
ATF_TC_HEAD(test_lrint2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check that domain errors are properly generated");
}
ATF_TC_BODY(test_lrint2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);


	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		(void)lrintf((float)t2table[i]);
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		lrint((double)t2table[i]);
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_LRINTL
		errno = 0;
		clear_exceptions();
		lrintl(t2table[i]);
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
	}
}

/*
 * Test case 3
 *
 * If the correct value is positive and too large to represent as a long,
 * a domain error shall occur and an unspecified value is returned.
 *
 * If the correct value is negative and too large to represent as a long,
 * a domain error shall occur and an unspecified value is returned.
 */
ATF_TC(test_lrint3);
ATF_TC_HEAD(test_lrint3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check that domain errors are properly generated");
}
ATF_TC_BODY(test_lrint3, tc)
{
	double dx;
	long double ldx;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	/* Initialize random number generator */
	init_randgen();

	ATF_FOR_LOOP(i, N, i++) {
		/* double */
		do {
			dx = random_double(FP_NORMAL);
		} while (dx <= LONG_MAX);

		/* positive */
		errno = 0;
		clear_exceptions();
		(void)rint(dx);
		ATF_PASS_OR_BREAK(iserrno_equalto(ERANGE));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* negative */
		errno = 0;
		clear_exceptions();
		(void)rint(-dx);
		ATF_PASS_OR_BREAK(iserrno_equalto(ERANGE));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* long double */
		do {
			ldx = random_long_double(FP_NORMAL);
		} while (ldx <= LONG_MAX);

		/* positive */
		errno = 0;
		clear_exceptions();
		(void)rintl(ldx);
		ATF_PASS_OR_BREAK(iserrno_equalto(ERANGE));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* negative */
		errno = 0;
		clear_exceptions();
		(void)rintl(-ldx);
		ATF_PASS_OR_BREAK(iserrno_equalto(ERANGE));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_lrint1);
	ATF_TP_ADD_TC(tp, test_lrint2);
	ATF_TP_ADD_TC(tp, test_lrint3);

	return atf_no_error();
}
