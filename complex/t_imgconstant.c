#include <atf-c.h>
#include <complex.h>

/*
 * Test case 1
 *
 * There are some old references stating that x+I*y is unusable in general,
 * since gcc introduces many overflow, underflow, sign and efficiency bugs
 * by rewriting I*y as (0.0+I)*(y+0.0*I) and laboriously computing the full
 * complex product. In particular, I*Inf is corrupted to NaN+I*Inf, and I*-0
 * is corrupted.
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

ATF_TC(test_imgconstant);
ATF_TC_HEAD(test_imgconstant, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "");
}
ATF_TC_BODY(test_imgconstant, tc)
{
#if !defined(__sun__)
	float complex fcx;
	double complex dcx;
	long double complex ldcx;
	size_t i, N;

	for (i = 0; i < N; i++) {
		/* float */
		fcx = I*t1table;
		ATF_CHECK(fpcmp_equalf(
			    creal(fcx),
			    0.0);

		/* double */
		dcx = I*t1table;

		/* long double */
		ldcx = I*t1table;
	}
#endif	/* !defined(__sun__) */
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_imgconstant);

	return atf_no_error();
}
