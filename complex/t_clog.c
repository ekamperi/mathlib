#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <complex.h>
#include <math.h>

#include "config.h"
#include "lconstants.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1
 */
ATF_TC(test_clog1);
ATF_TC_HEAD(test_clog1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check against a list of good (x, clog(x)) pairs");
}
ATF_TC_BODY(test_clog1, tc)
{
}

/*
 * Test case 2
 */
ATF_TC(test_clog2);
ATF_TC_HEAD(test_clog2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check if result is in the interval [-iPi, +iPi] "
	    "along the imaginary axis");
}
ATF_TC_BODY(test_clog2, tc)
{
	float complex fcx, fcy;
	double complex dcx, dcy;
	long double complex ldcx, ldcy;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fcx = random_float_complex(FP_NORMAL);
		fcy = clogf(fcx);
		ATF_PASS_OR_BREAK(cimagf(fcy) >= -(float)M_PI &&
				  cimagf(fcy) <=  (float)M_PI);

		/* double */
		dcx = random_double_complex(FP_NORMAL);
		dcy = clog(dcx);
		ATF_PASS_OR_BREAK(cimag(dcy) >= -M_PI &&
				  cimag(dcy) <= M_PI);

		/* long double */
#if defined(HAVE_CLOGL) && defined(HAVE_CIMAGL)
		ldcx = random_long_double_complex(FP_NORMAL);
		ldcy = clogl(ldcx);
		ATF_PASS_OR_BREAK(cimagl(ldcy) >= -M_PIl &&
				  cimagl(ldcy) <=  M_PIl);
#endif
	}
}

/*
 * Test case 3 -- Edge cases
 */
#define DONT_CARE 1.0

static const struct
t3entry {
	long double real_input;
	long double imag_input;
	long double real_expected;
	long double imag_expected;
} t3table[] = {
#ifdef	INFINITY
	{ -INFINITY, INFINITY,  INFINITY, DONT_CARE },
	{  INFINITY, INFINITY,  INFINITY, DONT_CARE },
	{      -0.0,      0.0, -INFINITY, DONT_CARE },
	{       0.0,      0.0, -INFINITY, DONT_CARE },
#endif
#ifdef  HUGE_VAL
	{ -HUGE_VAL, HUGE_VAL,  HUGE_VAL, DONT_CARE },
	{  HUGE_VAL, HUGE_VAL,  HUGE_VAL, DONT_CARE },
	{      -0.0,      0.0, -HUGE_VAL, DONT_CARE },
	{       0.0,      0.0, -HUGE_VAL, DONT_CARE },
#endif
#ifdef  HUGE_VALF
	{ -HUGE_VALF, HUGE_VALF, HUGE_VALF, DONT_CARE },
	{  HUGE_VALF, HUGE_VALF, HUGE_VALF, DONT_CARE },
	{      -0.0,      0.0,  -HUGE_VALF, DONT_CARE },
	{       0.0,      0.0,  -HUGE_VALF, DONT_CARE },
#endif
#ifdef  HUGE_VALL
	{ -HUGE_VALL, HUGE_VALL, HUGE_VALL, DONT_CARE },
	{  HUGE_VALL, HUGE_VALL, HUGE_VALL, DONT_CARE },
	{      -0.0,      0.0,  -HUGE_VALL, DONT_CARE },
	{       0.0,      0.0,  -HUGE_VALL, DONT_CARE },
#endif

#ifdef	NAN
	{ NAN, NAN, NAN, NAN }
#endif
};

ATF_TC(test_clog3);
ATF_TC_HEAD(test_clog3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_clog3, tc)
{
	float complex fcx;
	double complex dcx;

	size_t i, N;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */

		/* double */

		/* long double */
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_clog1);
	ATF_TP_ADD_TC(tp, test_clog2);
	ATF_TP_ADD_TC(tp, test_clog3);

	return atf_no_error();
}
