#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <float.h>	/* {DBL, FLT, LDBL}_EPSILON */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"

static const struct
tentry {
	long double x;       /* Input */
	long double y;       /* atanh output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_atanh1);
ATF_TC_HEAD(test_atanh1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, atanh(x)) pairs");
}
ATF_TC_BODY(test_atanh1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equal(atanh(ttable[i].x), ttable[i].y));
}

/*
 * Test case 2 -- Edge cases
 */
ATF_TC(test_atanh2);
ATF_TC_HEAD(test_atanh2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_atanh2, tc)
{
	/* If x is NaN, a NaN shall be returned */
	ATF_CHECK_IFNAN(atanhf(NAN));
	ATF_CHECK_IFNAN(atanh(NAN));
#ifdef	HAVE_ATANHL
	ATF_CHECK_IFNAN(atanhl(NAN));
#endif

	/* If x is +-0, x shall be returned */
	ATF_CHECK(fpcmp_equalf(atanhf( 0.0),  0.0));
	ATF_CHECK(fpcmp_equalf(atanhf(-0.0), -0.0));

	ATF_CHECK(fpcmp_equal(atanh( 0.0),  0.0));
	ATF_CHECK(fpcmp_equal(atanh(-0.0), -0.0));

#ifdef	HAVE_ATANHL
	ATF_CHECK(fpcmp_equall(atanhl( 0.0),  0.0));
	ATF_CHECK(fpcmp_equall(atanhl(-0.0), -0.0));
#endif
}

/*
 * Test case 3 -- Errors
 */
static const struct
t3entry {
	long double x;
	long double y;
} t3table[] = {
	/*
	 * If x is +-1, a pole error shall occur, and atanh(), atanhf(), and
	 * atanhl() shall return the value of the macro HUGE_VAL, HUGE_VALF,
	 * and HUGE_VALL, respectively, with the same sign as the correct
	 * value of the function.
	 */
#ifdef	HUGE_VAL
	{  1.0,  HUGE_VAL },
	{ -1.0, -HUGE_VAL },
#endif
#ifdef	HUGE_VALF
	{  1.0,  HUGE_VALF },
	{ -1.0, -HUGE_VALF },
#endif
#ifdef	HUGE_VALL
	{  1.0,  HUGE_VALL },
	{ -1.0, -HUGE_VALL},
#endif
};

ATF_TC(test_atanh3);
ATF_TC_HEAD(test_atanh3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Pole errors");
}
ATF_TC_BODY(test_atanh3, tc)
{
	int haserrexcept;
	int haserrno;
	size_t i, N;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
#ifdef	HUGE_VALF
		ATF_CHECK(fpcmp_equalf(atanhf(t3table[i].x), HUGE_VALF));
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		/* double */
		errno = 0;
		clear_exceptions();
#ifdef	HUGE_VAL
		ATF_CHECK(fpcmp_equal(atanh(t3table[i].x), HUGE_VAL));
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		    /* long double */
#ifdef	HAVE_ATANHL
		errno = 0;
		clear_exceptions();
#ifdef	HUGE_VALL
		ATF_CHECK(fpcmp_equall(atanhl(t3table[i].x), HUGE_VALL));
#endif
		ATF_CHECK(iserrno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));
#endif	/* HAVE_ATANHL */
	}

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);
}

/*
 * Test case 4 -- Domain errors
 */
static const long double
t4table[] = {
	/*
	 * For finite |x|>1, a domain error shall occur, and either a NaN
	 * (if supported), or an implementation-defined value shall be returned.
	 */
	 1.0 +  DBL_EPSILON,
	 1.0 +  FLT_EPSILON,
	 1.0 + LDBL_EPSILON,
	-1.0 -  DBL_EPSILON,
	-1.0 -  FLT_EPSILON,
	-1.0 - LDBL_EPSILON,

	/*
	 * If x is +-Inf, a domain error shall occur, and either a NaN
	 * (if supported), or an implementation-defined value shall be returned.
	 */
#ifdef	INFINITY
	  INFINITY
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
	 -HUGE_VALL,
#endif
};

ATF_TC(test_atanh4);
ATF_TC_HEAD(test_atanh4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Domain errors");
}
ATF_TC_BODY(test_atanh4, tc)
{
	int haserrexcept;
	int haserrno;
	size_t i, N;

	N = sizeof(t4table) / sizeof(t4table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		ATF_CHECK_IFNAN(atanhf(t3table[i].x));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		ATF_CHECK_IFNAN(atanh(t3table[i].x));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef  HAVE_ATANHL
		errno = 0;
		clear_exceptions();
		ATF_CHECK_IFNAN(atanhl(t3table[i].x));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
	}

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_atanh1);
	ATF_TP_ADD_TC(tp, test_atanh2);
	ATF_TP_ADD_TC(tp, test_atanh3);
	ATF_TP_ADD_TC(tp, test_atanh4);

	return atf_no_error();
}
