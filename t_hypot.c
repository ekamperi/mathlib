#define	_XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"

#ifdef  HAS_MATH_ERREXCEPT
#include <fenv.h>
#endif  /* HAS_MATH_ERREXCEPT */

struct tentry {
	double x;	/* Input */
	double y;	/* Input */
	double z;       /* hypot() output */
} t1table[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_hypot1);
ATF_TC_HEAD(test_hypot1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good "
	    "((x,y), hypot(x,y)) pairs");
}
ATF_TC_BODY(test_hypot1, tc)
{
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	/*ATF_REQUIRE(N > 0);*/

	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equal(
			    hypot(t1table[i].x, t1table[i].y), t1table[i].z));
	}
}

/*
 * Test case 2 -- Edge cases
 */
struct t2entry {
	double x;       /* Input */
	double y;	/* Input */
	double z;       /* hypot() output */
} t2table[] = {
#ifdef	INFINITY
	/* If x is +-Inf, +Inf shall be returned */
	{  INFINITY,   0.0, INFINITY },
	{  INFINITY,   1.0, INFINITY },
	{  INFINITY,   2.0, INFINITY },
	{  INFINITY, 10E10, INFINITY },

	{ -INFINITY,   0.0, INFINITY },
	{ -INFINITY,   1.0, INFINITY },
	{ -INFINITY,   2.0, INFINITY },
	{ -INFINITY, 10E10, INFINITY },

	/* If y is +-Inf, +Inf shall be returned */
	{    0.0, INFINITY, INFINITY },
	{    1.0, INFINITY, INFINITY },
	{    2.0, INFINITY, INFINITY },
	{  10E10, INFINITY, INFINITY },

	{   0.0, -INFINITY, INFINITY },
	{   1.0, -INFINITY, INFINITY },
	{   2.0, -INFINITY, INFINITY },
	{ 10E10, -INFINITY, INFINITY },
#endif

	/* ... even if one of x or y is NaN */
#if defined(NAN) && defined(INFINITY)
	{ NAN,  INFINITY, INFINITY },
	{ NAN, -INFINITY, INFINITY },

	{  INFINITY, NAN, INFINITY },
	{ -INFINITY, NAN, INFINITY },
#endif

	/*
	 * If x or y is NaN, and the other is not +-Inf,
	 * a NaN shall be returned.
	 */
#if defined(NAN) && defined(INFINITY)
	{ NAN,   1.0, NAN },
	{ NAN,   2.0, NAN },
	{ NAN, 10E10, NAN },

	{   1.0, NAN, NAN },
	{   2.0, NAN, NAN },
	{ 10E10, NAN, NAN }
#endif
};

ATF_TC(test_hypot2);
ATF_TC_HEAD(test_hypot2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_hypot2, tc)
{
	size_t i, N;
	double oval;	/* output value */

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		oval = hypot(t2table[i].x, t2table[i].y);
		ATF_CHECK(fpcmp_equal(oval, t2table[i].z));
	}
}

/*
 * Test case 3 -- Check undue overflow
 */
ATF_TC(test_hypot3);
ATF_TC_HEAD(test_hypot3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "hypot() doesn't overlow in intermediate steps");
}
ATF_TC_BODY(test_hypot3, tc)
{
	int hasfp = 0;		/* Whether exceptions are supported */
	int haserrno = 0;	/* Whether errno is supported */

	/*
	 * POSIX requires that at least one of the error reporting mechanisms
	 * is supported.
	 */
	query_errhandling(&hasfp, &haserrno);
	ATF_REQUIRE(hasfp || haserrno);

	volatile double x, y;
	x =  DBL_MAX / M_SQRT2;
	y =  DBL_MAX / M_SQRT2;

	if (hasfp) {
		int ex;

		/* Clear the overflow exception */
		ATF_REQUIRE(feclearexcept(FE_OVERFLOW) == 0);
		(void)hypot(x, y);

		/* ... and check if it was raised */
		ex = fetestexcept(FE_OVERFLOW);
		ATF_CHECK((ex & FE_OVERFLOW) == 0);
	}

	if (haserrno) {
		errno = 0;
		(void)hypot(x, y);
		ATF_CHECK(errno == 0);
	}

	/* This should indeed overflow */
	x = DBL_MAX;
	y = DBL_MAX;

	if (hasfp) {
		int ex;

		/* Clean all the exceptions */
                ATF_REQUIRE(feclearexcept(FE_ALL_EXCEPT) == 0);
                (void)hypot(x, y);

                /* ... and check if it was raised */
                ex = fetestexcept(FE_OVERFLOW);
                ATF_CHECK((ex & FE_OVERFLOW) == FE_OVERFLOW );
        }

        if (haserrno) {
                errno = 0;
                (void)hypot(x, y);
                ATF_CHECK(errno == 0);
        }
}

/*
 * Test case 4 -- Check undue underflow
 */
ATF_TC(test_hypot4);
ATF_TC_HEAD(test_hypot4, tc)
{
  atf_tc_set_md_var(tc,
		    "descr",
		    "hypot() doesn't underflow in intermediate steps");
}
ATF_TC_BODY(test_hypot4, tc)
{
	int hasfp = 0;          /* Whether exceptions are supported */
	int haserrno = 0;       /* Whether errno is supported */

	/*
	 * POSIX requires that at least one of the error reporting mechanisms
	 * is supported.
	 */
	query_errhandling(&hasfp, &haserrno);
	ATF_REQUIRE(hasfp || haserrno);

	volatile double x, y;
	x = DBL_MIN;
	y = DBL_MIN;

	if (hasfp) {
		int ex;

		/* Clear the underflow exception */
		ATF_REQUIRE(feclearexcept(FE_UNDERFLOW) == 0);
		(void)hypot(x, y);

		/* ... and check if it was raised */
		ex = fetestexcept(FE_UNDERFLOW);
		ATF_CHECK((ex & FE_UNDERFLOW) == 0);
	}

	if (haserrno) {
		errno = 0;
		(void)hypot(x, y);
		ATF_CHECK(errno == 0);
	}
}

/*
 * Test case 5 -- Check some identities
 */
ATF_TC(test_hypot5);
ATF_TC_HEAD(test_hypot5, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some identities");
}
ATF_TC_BODY(test_hypot5, tc)
{
	/* hypot(x,y), hypot(y,x), and hypot(x, -y) are equivalent */
	
	/* hypot(x, +-0) is equivalent to fabs(x) */
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_hypot1);
	ATF_TP_ADD_TC(tp, test_hypot2);
	ATF_TP_ADD_TC(tp, test_hypot3);
	ATF_TP_ADD_TC(tp, test_hypot4);
	ATF_TP_ADD_TC(tp, test_hypot5);

	return atf_no_error();
}
