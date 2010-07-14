#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <float.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_nextafter.h"

/*
 * Test case 1 -- Basic functionality
 */
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

	/* float */
	N = sizeof(t1ftable) / sizeof(t1ftable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(nextafterf(t1ftable[i].x, t1ftable[i].y)
		    == t1ftable[i].z);

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(nextafter(t1dtable[i].x, t1dtable[i].y)
		    == t1dtable[i].z);

	/* long double */
#ifdef	HAVE_NEXTAFTERL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(nextafterl(t1ldtable[i].x, t1ldtable[i].y)
		    == t1ldtable[i].z);
#endif
}

/*
 * Test case 2 -- Random walks
 */
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
#ifdef	INFINITY
	float startf, nextf;
	double startd, nextd;
	long double startld, nextld;
	size_t i, j;
	long nwalks;

	nwalks = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(nwalks > 0);

	/*
	 * If at any time we hit an infinity while walking, skip the check.
	 * For example this could happen if startd = -DBL_MAX, NWALKS > 0.
	 * Be indiscriminate and skip all tests, even if only one is bad.
	 */
	ATF_FOR_LOOP(i, nwalks, i++) {
		startf  = random_float(FP_NORMAL);
		startd  = random_double(FP_NORMAL);
		startld = random_long_double(FP_NORMAL);

		nextf  = startf;
		nextd  = startd;
		nextld = startld;

		/* Go backwards */
		for (j = 0; j < WALKLEN; j++) {
			nextf  = nextafterf(nextf,  -INFINITY);
			nextd  = nextafter (nextd,  -INFINITY);
#ifdef	HAVE_NEXTAFTERL
			nextld = nextafterl(nextld, -INFINITY);
#endif
			if (isinf(nextf) || isinf(nextd) || isinf(nextld))
				goto SKIP_CHECKS;
		}

		/* Go forward */
		for (j = 0; j < WALKLEN; j++) {
			nextf  = nextafterf(nextf,  +INFINITY);
			nextd  = nextafter (nextd,  +INFINITY);
#ifdef	HAVE_NEXTAFTERL
			nextld = nextafterl(nextld, +INFINITY);
#endif
			if (isinf(nextf) || isinf(nextd) || isinf(nextld))
				goto SKIP_CHECKS;
		}

		/* We should be back to square 1 by now */
		ATF_PASS_OR_BREAK(nextf  == startf );
		ATF_PASS_OR_BREAK(nextd  == startd );
#ifdef	HAVE_NEXTAFTERL
		ATF_PASS_OR_BREAK(nextld == startld);
#endif
SKIP_CHECKS:;
	}
#endif	/* INFINITY */
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
#ifdef	HAVE_NEXTAFTERL
		ATF_PASS_OR_BREAK(nextafterl(ldx, ldx) == ldx);
#endif

		/* If x or y is NaN, a NaN shall be returned */
#ifdef	NAN
		ATF_PASS_OR_BREAK(isnan(nextafterf(NAN, fx)));
		ATF_PASS_OR_BREAK(isnan(nextafterf(fx, NAN)));

		ATF_PASS_OR_BREAK(isnan(nextafter(NAN, dx)));
		ATF_PASS_OR_BREAK(isnan(nextafter(dx, NAN)));

#ifdef	HAVE_NEXTAFTERL
		ATF_PASS_OR_BREAK(isnan(nextafterl(NAN, ldx)));
		ATF_PASS_OR_BREAK(isnan(nextafterl(ldx, NAN)));
#endif	/* HAVE_NEXTAFTERL */
#endif
	}
}

/*
 * Test case 4 -- Edge cases
 *
 * If x is finite and the correct function value would overflow, a range error
 * shall occur and +-HUGE_VAL, +-HUGE_VALF, and +-HUGE_VALL (with the same sign
 * as x) shall be returned as appropriate for the return type of the function.
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
#ifdef	INFINITY
	int haserrexcept;
	int haserrno;

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/* float */
	errno = 0;
	clear_exceptions();

	ATF_CHECK(fpcmp_equalf(
		    nextafterf(FLT_MAX, +INFINITY),
		    HUGE_VALF));

	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));

	/* double */
	errno = 0;
	clear_exceptions();
	ATF_CHECK(fpcmp_equal(
		    nextafter(DBL_MAX, +INFINITY),
		    HUGE_VAL));

	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));

	/* long double */
#ifdef	HAVE_NEXTAFTERL
	errno = 0;
	clear_exceptions();

	ATF_CHECK(fpcmp_equall(
		    nextafterl(LDBL_MAX, +INFINITY),
		    HUGE_VALL));

	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
#endif	/* HAVE_NEXTAFTERL */
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
