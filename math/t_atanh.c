#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <float.h>	/* {DBL, FLT, LDBL}_EPSILON */
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_atanh.h"

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

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equal(
			    atanh(t1dtable[i].x),
				  t1dtable[i].y));

	/* long double */
#ifdef  HAVE_ATANHL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equall(
			    atanhl(t1ldtable[i].x),
				   t1ldtable[i].y));
#endif
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
	ATF_CHECK(fpreal_equalf(atanhf( 0.0),  0.0));
	ATF_CHECK(fpreal_equalf(atanhf(-0.0), -0.0));

	ATF_CHECK(fpreal_equal(atanh( 0.0),  0.0));
	ATF_CHECK(fpreal_equal(atanh(-0.0), -0.0));

#ifdef	HAVE_ATANHL
	ATF_CHECK(fpreal_equall(atanhl( 0.0),  0.0));
	ATF_CHECK(fpreal_equall(atanhl(-0.0), -0.0));
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
	{ -1.0, -HUGE_VALL },
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
		ATF_CHECK(fpreal_equalf(atanhf(t3table[i].x),
					(float)t3table[i].y));
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		/* double */
		errno = 0;
		clear_exceptions();
		ATF_CHECK(fpreal_equal(atanh(t3table[i].x),
				      (double)t3table[i].y));
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));

		/* long double */
#ifdef	HAVE_ATANHL
		errno = 0;
		clear_exceptions();
		ATF_CHECK(fpreal_equall(atanhl(t3table[i].x),
					       t3table[i].y));
		ATF_CHECK(errno_equalto(ERANGE));
		ATF_CHECK(raised_exceptions(MY_FE_DIVBYZERO));
#endif
	}

	/* Revenge is a Dish Best Served Cold :) */
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
		ATF_CHECK_IFNAN(atanhf((float)t4table[i]));
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		ATF_CHECK_IFNAN(atanh((double)t4table[i]));
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef  HAVE_ATANHL
		errno = 0;
		clear_exceptions();
		ATF_CHECK_IFNAN(atanhl(t4table[i]));
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
	}

	/* Revenge is a Dish Best Served Cold :) */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);
}

/*
 * Test case 5
 *
 * If x is subnormal, a range error _may_ occur and x _should_ be
 * returned.
 */
ATF_TC(test_atanh5);
ATF_TC_HEAD(test_atanh5, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Input is subnormal");
}
ATF_TC_BODY(test_atanh5, tc)
{
#ifdef	HAVE_SUBNORMALS
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_SUBNORMAL);
		fy = atanhf(fx);
		ATF_PASS_OR_BREAK(fy == fx);

		/* double */
		dx = random_double(FP_SUBNORMAL);
		dy = atanh(dx);
		ATF_PASS_OR_BREAK(dy == dx);

		/* long double */
#ifdef	HAVE_ATANHL
		ldx = random_long_double(FP_SUBNORMAL);
		ldy = atanhl(ldx);
		ATF_PASS_OR_BREAK(ldy == ldx);
#endif
  }
#endif	/* HAVE_SUBNORMALS */
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_atanh1);
	ATF_TP_ADD_TC(tp, test_atanh2);
	ATF_TP_ADD_TC(tp, test_atanh3);
	ATF_TP_ADD_TC(tp, test_atanh4);
	ATF_TP_ADD_TC(tp, test_atanh5);

	return atf_no_error();
}
