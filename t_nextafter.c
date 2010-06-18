#define _XOPEN_SOURCE 600

#include "config.h"
#include "subr_errhandling.h"
#include "subr_random.h"

#include <atf-c.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef  HAS_MATH_ERREXCEPT
#include <fenv.h>
#endif  /* HAS_MATH_ERREXCEPT */

#include "subr_atf.h"

/*
 * Test case 1 -- Basic functionality
 */
struct tentry {
	double x;       /* Input */
	double y;       /* Input */
	double z;       /* nextafter() output */
} ttable[] = {
	{ 1.0,	2.0,			1.0 + DBL_EPSILON },
	{ 1.0,	1.0 + DBL_EPSILON,	1.0 + DBL_EPSILON },
	{ 1.0,	+INFINITY,		1.0 + DBL_EPSILON },
#ifdef	notyet
	{ 1.0,  -2.0,                   1.0 - DBL_EPSILON },
	{ 1.0,   1.0 - DBL_EPSILON,     1.0 - DBL_EPSILON },
	{ 1.0,  -INFINITY,              1.0 - DBL_EPSILON },
#endif
};

ATF_TC(test_nextafter1);
ATF_TC_HEAD(test_nextafter1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good "
	    "(x, y, nextafter(x, y)) pairs");
}
ATF_TC_BODY(test_nextafter1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(nextafter(ttable[i].x, ttable[i].y) == ttable[i].z);
	}
}

/*
 * Test case 2 -- Random walks
 */
#define NWALKS	10000	/* Number of walks */
#define WALKLEN	10000	/* Length of each walk in terms of nextafter() calls */

ATF_TC(test_nextafter2);
ATF_TC_HEAD(test_nextafter2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Random walk back and forth using nextafter()");
}
ATF_TC_BODY(test_nextafter2, tc)
{
	double start, next;
	size_t i, j;

	ATF_FOR_LOOP(i, NWALKS, i++) {
		start = random_double(FP_NORMAL);
		next = start;

		/* Go backwards */
		for (j = 0; j < WALKLEN; j++) {
			next = nextafter(next, -INFINITY);
		}

		/* Go forward */
		for (j = 0; j < WALKLEN; j++) {
			next = nextafter(next, +INFINITY);
		}

		/* We should be back to square 1 by now */
		ATF_PASS_OR_BREAK(next == start);
	}
}

/*
 * Test case 3 -- Edge cases
 */
ATF_TC(test_nextafter3);
ATF_TC_HEAD(test_nextafter3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_nextafter3, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		fx = random_float(FP_NORMAL);
		dx = random_double(FP_NORMAL);
		ldx = random_long_double(FP_NORMAL);

		/* If x == y, y (of the type x) shall be returned */
		ATF_PASS_OR_BREAK(nextafterf(fx, fx) == fx);
		ATF_PASS_OR_BREAK(nextafter(dx, dx) == dx);
		ATF_PASS_OR_BREAK(nextafterl(ldx, ldx) == ldx);

		/* If x or y is NaN, a NaN shall be returned */
#ifdef	NAN
		ATF_PASS_OR_BREAK(isnan(nextafterf(NAN, fx)));
		ATF_PASS_OR_BREAK(isnan(nextafterf(fx, NAN)));

                ATF_PASS_OR_BREAK(isnan(nextafter(NAN, dx)));
                ATF_PASS_OR_BREAK(isnan(nextafter(dx, NAN)));

                ATF_PASS_OR_BREAK(isnan(nextafterl(NAN, ldx)));
                ATF_PASS_OR_BREAK(isnan(nextafterl(ldx, NAN)));
#endif
	}
}

/*
 * Test case 4 -- Edge cases
 *
 * We put this checks in a separate test case, because we will strictly
 * require the existence of errno or fp exception error handling. And it
 * would be bad to block the other tests because the lack of latter.
 */
ATF_TC(test_nextafter4);
ATF_TC_HEAD(test_nextafter4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check more edge cases");
}
ATF_TC_BODY(test_nextafter4, tc)
{
	/*
	 * We can't proceed if there's no way to detect errors,
	 * especially overflows.
	 */
	int haserrexcept;
	int haserrno;

	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/*
	 * If x is finite and the correct function value would overflow,
	 * a range error shall occur and +-HUGE_VAL, +-HUGE_VALF,
	 * and +-HUGE_VALL (with the same sign as x) shall be returned as
	 * appropriate for the return type of the function.
	 */
#ifdef	INFINITY
	/* double */
	errno = 0;
	clear_exceptions();

	ATF_CHECK(nextafter(DBL_MAX, +INFINITY) == HUGE_VAL);
	ATF_CHECK(signbit(nextafter(DBL_MAX, +INFINITY)) == 0);

        ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));

	/* float */
	errno = 0;
	clear_exceptions();

	ATF_CHECK(nextafterf(FLT_MAX, +INFINITY) == HUGE_VALF);
	ATF_CHECK(signbit(nextafterf(FLT_MAX, +INFINITY)) == 0);

        ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));

	/* long double */
	errno = 0;
	clear_exceptions();

	ATF_CHECK(nextafterl(LDBL_MAX, +INFINITY) == HUGE_VALL);
	ATF_CHECK(signbit(nextafterl(LDBL_MAX, +INFINITY)) == 0);

        ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
#endif	/* INFINITY */
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_nextafter1);
	ATF_TP_ADD_TC(tp, test_nextafter2);
	ATF_TP_ADD_TC(tp, test_nextafter3);
	ATF_TP_ADD_TC(tp, test_nextafter4);

	return atf_no_error();
}
