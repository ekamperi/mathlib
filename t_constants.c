#define	_XOPEN_SOURCE 600

#include <atf-c.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>

#include "subr_fpcmp.h"

/* Test case 1 -- various constants availibility */
ATF_TC(test_const_avail);
ATF_TC_HEAD(test_const_avail, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Test availability of various math constants");
}
ATF_TC_BODY(test_const_avail, tc)
{
	int isdefined;

	/* Value of e */
	isdefined = 0;
#ifdef	M_E
	ATF_CHECK(fpcmp_equal(M_E, 2.71828182845904523536));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of log2e */
	isdefined = 0;
#ifdef  M_LOG2E
	ATF_CHECK(fpcmp_equal(M_LOG2E, 1.44269504088896340736));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of log10e */
	isdefined = 0;
#ifdef  M_LOG10E
	ATF_CHECK(fpcmp_equal(M_LOG10E, 0.434294481903251827651));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of loge2 */
	isdefined = 0;
#ifdef  M_LN2
	ATF_CHECK(fpcmp_equal(M_LN2, 0.693147180559945309417));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of loge10 */
	isdefined = 0;
#ifdef  M_LN10
	ATF_CHECK(fpcmp_equal(M_LN2, 2.30258509299404568402));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of pi */
	isdefined = 0;
#ifdef  M_PI
	ATF_CHECK(fpcmp_equal(M_PI, 3.141592653589793238462643));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of pi/2 */
	isdefined = 0;
#ifdef  M_PI_2
	ATF_CHECK(fpcmp_equal(M_PI_2, 1.57079632679489661923));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of pi/4 */
	isdefined = 0;
#ifdef  M_PI_4
	ATF_CHECK(fpcmp_equal(M_PI_4, 0.785398163397448309616));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of 1/pi */
	isdefined = 0;
#ifdef  M_1_PI
		ATF_CHECK(fpcmp_equal(M_1_PI, 0.318309886183790671538));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of 2/pi */
	isdefined = 0;
#ifdef  M_2_PI
	ATF_CHECK(fpcmp_equal(M_2_PI, 0.636619772367581343076));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of 2/sqrt(pi) */
	isdefined = 0;
#ifdef  M_2_SQRTPI
	ATF_CHECK(fpcmp_equal(M_2_SQRTPI, 1.12837916709551257390));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of sqrt(2) */
	isdefined = 0;
#ifdef  M_SQRT2
	ATF_CHECK(fpcmp_equal(M_SQRT2, 1.41421356237309504880));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* Value of 1/sqrt(2) */
	isdefined = 0;
#ifdef  M_SQRT1_2
	ATF_CHECK(fpcmp_equal(M_SQRT1_2, 0.707106781186547524401));
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);
}

/* Test case 2 */
ATF_TC(test_const_avail2);
ATF_TC_HEAD(test_const_avail2, tc)
{
  atf_tc_set_md_var(tc,
		    "descr",
		    "The value of FP_ILOGB0 shall be either {INT_MIN} or -"
		    "{INT_MAX}. The value of FP_ILOGBNAN shall be either "
		    "{INT_MAX} or {INT_MIN}");
}
ATF_TC_BODY(test_const_avail2, tc)
{
#if defined(MATH_ILOGB0) && defined(INT_MIN) && defined(INT_MAX)
	ATF_CHECK(MATH_ILOGB0 == INTMIN || MAT_ILOGB0 == INTMAX);
#endif

#if defined(MATH_ILOGBNAN) && defined(INT_MIN) && defined(INT_MAX)
	ATF_CHECK(MATH_ILOGBNAN == INT_MAX || MATH_ILOGBNAN == INT_MIN);
#endif
}

/* Test case 3 */
ATF_TC(test_const_avail3);
ATF_TC_HEAD(test_const_avail3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Test availibility of various math constants");
}
ATF_TC_BODY(test_const_avail3, tc)
{
#ifdef	MATH_ERRNO
	ATF_CHECK(MATH_ERRNO == 1);
#endif
#ifdef	MATH_ERREXCEPT
	ATF_CHECK(MATH_ERREXCEPT == 2);
#endif
}

/*
 * Test case 3
 */
ATF_TC(test_math_errhandling);
ATF_TC_HEAD(test_math_errhandling, tc)
{
        atf_tc_set_md_var(tc,
            "descr",
	    "math_errhandling shall expand to an expression that has type int "
	    "and the value MATH_ERRNO, MATH_ERREXCEPT, or the bitwise-inclusive "
	    "OR of both");
}
ATF_TC_BODY(test_math_errhandling, tc)
{
#if defined(math_errhandling) && (defined(MATH_ERRNO) || defined(MATH_ERREXCEPT))
	ATF_CHECK(sizeof(math_errhandling) == sizeof(int));
	ATF_CHECK(math_errhandling == MATH_ERRNO
	       || math_errhandling == MATH_ERREXCEPT
	       || math_errhandling == (MATH_ERRNO | MATH_ERREXCEPT));
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_const_avail);
	ATF_TP_ADD_TC(tp, test_const_avail2);
	ATF_TP_ADD_TC(tp, test_const_avail3);
	ATF_TP_ADD_TC(tp, test_math_errhandling);

	return atf_no_error();
}
