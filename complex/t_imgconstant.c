#include <atf-c.h>
#include <complex.h>
#include <math.h>

#include "subr_fpcmp.h"

/*
 * Test case 1
 *
 *  Quoting comment in freebsd/lib/msun/src/math_private.h:
 *
 * x+I*y is unusable in general,since gcc introduces many overflow,
 * underflow, sign and efficiency bugs by rewriting I*y as (0.0+I)*(y+0.0*I)
 * and laboriously computing the full complex product.
 *
 * In particular, I*Inf is corrupted to NaN+I*Inf, and I*-0 is corrupted to
 *  -0.0+I*0.0.
 */
static const long double
t1table[] =
{
#ifdef  INFINITY
	INFINITY,
#endif
#ifdef  HUGE_VAL
        HUGE_VAL,
#endif
#ifdef  HUGE_VALF
        HUGE_VALF,
#endif
#ifdef  HUGE_VALL
	HUGE_VALL,
#endif
};

ATF_TC(test_imgconstant1);
ATF_TC_HEAD(test_imgconstant1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check if I*Inf is corrupted");
}
ATF_TC_BODY(test_imgconstant1, tc)
{
#if !defined(__sun__)
	float complex fcx;
	double complex dcx;
	long double complex ldcx;
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		fcx = I*((float)t1table[i]);
		ATF_CHECK(fpreal_equalf(
			    crealf(fcx),
			    0.0));

		/* double */
		dcx = I*((double)t1table[i]);
		ATF_CHECK(fpreal_equal(
			    creal(dcx),
			    0.0));

		/* long double */
#if defined(HAVE_CREALL)
		ldcx = I*t1table[i];
		ATF_CHECK(fpreal_equall(
			    creall(ldcx),
			    0.0));
#endif
	}
#endif	/* !defined(__sun__) */
}

/*
 * and I*-0.0 is corrupted to -0.0+I*0.0
 */
ATF_TC(test_imgconstant2);
ATF_TC_HEAD(test_imgconstant2, tc)
{
        atf_tc_set_md_var(tc,
            "descr",
            "Check if Inf-0.0 is corrupted");
}
ATF_TC_BODY(test_imgconstant2, tc)
{
#if	!defined(__sun__)
	float complex fcx;

	/* float */
	fcx = I*(-0.0);
	ATF_CHECK(fpreal_equalf(creal (fcx), -0.0) == 0);
	ATF_CHECK(fpreal_equalf(cimagf(fcx),  0.0) == 0);

	/* double */

	/* long double */
#endif	/* !defined(__sun__) */
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_imgconstant1);
	ATF_TP_ADD_TC(tp, test_imgconstant2);

	return atf_no_error();
}
