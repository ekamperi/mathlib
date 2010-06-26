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
	long double y;	/* Input */
	long double z;	/* fdim output */
} t1table[] = {
};

ATF_TC(test_fdim1);
ATF_TC_HEAD(test_fdim1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, fdim(x)) pairs");
}
ATF_TC_BODY(test_fdim1, tc)
{
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++) {
	}
}

/*
 * Test case 2 -- Edge cases
 */
ATF_TC(test_fdim2);
ATF_TC_HEAD(test_fdim2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_fdim2, tc)
{
#ifdef	NAN
	float fx;
	double dx;
	long double ldx;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	/* If x or y is NaN, a NaN shall be returned */
	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		ATF_PASS_OR_BREAK(isnan(fdimf(fx, NAN)));
		ATF_PASS_OR_BREAK(isnan(fdimf(NAN, fx)));

		/* double */
		dx = random_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(isnan(fdim(dx, NAN)));
		ATF_PASS_OR_BREAK(isnan(fdim(NAN, dx)));

		/* long double */
		ldx = random_long_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(isnan(fdiml(ldx, NAN)));
		ATF_PASS_OR_BREAK(isnan(fdiml(NAN, ldx)));
	}
#endif
}

/*
 * Test case 3 -- Overflow/Underflow
 */
static const long double
t3table[] = {
#ifdef	INFINITY
	INFINITY,
#endif
#ifdef	HUGE_VAL
	HUGE_VAL,
#endif
#ifdef	HUGE_VALF
	HUGE_VALF,
#endif
#ifdef	HUGE_VALL
	HUGE_VALL
#endif
};

ATF_TC(test_fdim3);
ATF_TC_HEAD(test_fdim3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Deliberate overflow/underflow");
}
ATF_TC_BODY(test_fdim3, tc)
{
	float fy;
	double dy;
	long double ldy;
	int haserrexcept;
	int haserrno;
	size_t i, N;

	/* We need at least one way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/*
	 * If x-y is positive and overflows, a range error shall occur and
	 * fdim(), fdimf(), and fdiml() shall return the value of the macro
	 * HUGE_VAL, HUGE_VALF, and HUGE_VALL, respectively.
	 */
	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		fy = fdimf((float)t3table[i], 0.0);
#ifdef	HUGE_VALF
		ATF_CHECK(fpcmp_equalf(fy, HUGE_VALF));
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));

		/* double */
		errno = 0;
		clear_exceptions();
		dy = fdim((double)t3table[i], 0.0);
#ifdef	HUGE_VAL
		ATF_CHECK(fpcmp_equal(dy, HUGE_VAL));
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));

		/* long double */
#ifdef	HAVE_FDIML
		errno = 0;
		clear_exceptions();
		ldy = fdiml(t3table[i], 0.0);
#ifdef	HUGE_VALL
		ATF_CHECK(fpcmp_equall(ldy, HUGE_VALL));
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
#endif	/* HAVE_FDIML */
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fdim1);
	ATF_TP_ADD_TC(tp, test_fdim2);
	ATF_TP_ADD_TC(tp, test_fdim3);

	return atf_no_error();
}
