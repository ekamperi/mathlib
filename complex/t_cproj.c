#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "lconstants.h"
#include "mytypes.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1
 */
ATF_TC(test_cproj1);
ATF_TC_HEAD(test_cproj1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check against a list of good (x, cproj(x)) pairs");
}
ATF_TC_BODY(test_cproj1, tc)
{
}

/*
 * Test case 2
 */
ATF_TC(test_cproj2);
ATF_TC_HEAD(test_cproj2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "z projects to z, if z doesn't have any infinite parts");
}
ATF_TC_BODY(test_cproj2, tc)
{
	float complex fcx;
	double complex dcx;
	long double complex ldcx;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fcx = random_float_complex(FP_NORMAL);
		ATF_PASS_OR_BREAK(cprojf(fcx) == fcx);

		/* double */
		dcx = random_double_complex(FP_NORMAL);
		ATF_PASS_OR_BREAK(cproj(dcx) == dcx);

		/* long double */
#if defined(HAVE_CPROJL)
		ldcx = random_long_double_complex(FP_NORMAL);
		ATF_PASS_OR_BREAK(cprojl(ldcx) == ldcx);
#endif
	}
}

/*
 * Test case 3
 * XXX: Add NaNs as well
 */
static const struct
t3entry {
	long double x;
	long double y;
} t3table[] = {
#ifdef  INFINITY
	{  INFINITY,  INFINITY },
	{  INFINITY, -INFINITY },
	{ -INFINITY,  INFINITY },
	{ -INFINITY, -INFINITY },
#endif
#ifdef  HUGE_VAL
	{  HUGE_VAL,  HUGE_VAL },
	{  HUGE_VAL, -HUGE_VAL },
	{ -HUGE_VAL,  HUGE_VAL },
	{ -HUGE_VAL, -HUGE_VAL },
#endif
#ifdef  HUGE_VALF
	{  HUGE_VALF,  HUGE_VALF },
	{  HUGE_VALF, -HUGE_VALF },
	{ -HUGE_VALF,  HUGE_VALF },
	{ -HUGE_VALF, -HUGE_VALF },
#endif
#ifdef  HUGE_VALL
	{  HUGE_VALL,  HUGE_VALL },
	{  HUGE_VALL, -HUGE_VALL },
	{ -HUGE_VALL,  HUGE_VALL },
	{ -HUGE_VALL, -HUGE_VALL }
#endif
};

ATF_TC(test_cproj3);
ATF_TC_HEAD(test_cproj3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check what happens when z has infinite parts");
}
ATF_TC_BODY(test_cproj3, tc)
{
	float complex fcx, fcy;
	float_complex ufcx;
	double complex dcx, dcy;
	double_complex udcx;
	long double complex ldcx, ldcy;
	long_double_complex uldcx;
	long i, N, idx;

	srand(time(NULL));

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		idx = i % ( sizeof(t3table) / sizeof(t3table[0]) );

		/* float */
		REAL_PART(ufcx) = rand() ? (float)t3table[idx].x :
					   random_float(FP_NORMAL);
		IMAG_PART(ufcx) = rand() ? (float)t3table[idx].y :
					   random_float(FP_NORMAL);
		fcx = ufcx.z;
		fcy = cprojf(fcx);
		if (isinf(crealf(fcx)) || isinf(cimagf(fcx))) {
			ATF_PASS_OR_BREAK(isinf(crealf(fcy)));
			ATF_PASS_OR_BREAK(cimagf(fcy) ==
					  copysignf(0.0, cimagf(fcx)));
		} else {
			ATF_PASS_OR_BREAK(fcy == fcx);
		}

		/* double */
		REAL_PART(udcx) = rand() ? (double)t3table[idx].x :
					   random_double(FP_NORMAL);
		IMAG_PART(udcx) = rand() ? (double)t3table[idx].y :
					   random_double(FP_NORMAL);
		dcx = udcx.z;
		dcy = cproj(dcx);
		if (isinf(creal(dcx)) || isinf(cimag(dcx))) {
			ATF_PASS_OR_BREAK(isinf(creal(dcy)));
			ATF_PASS_OR_BREAK(cimag(dcy) ==
					  copysign(0.0, cimagf(dcx)));
		} else {
			ATF_PASS_OR_BREAK(dcy == dcx);
		}

		/* long double */
#if defined(HAVE_CPROJL) && defined(HAVE_CREALL) && defined(HAVE_CIMAGL) && \
    defined(HAVE_COPYSIGNL)
		REAL_PART(uldcx) = rand() ? t3table[idx].x :
					    random_long_double(FP_NORMAL);
		IMAG_PART(uldcx) = rand() ? t3table[idx].y :
					    random_long_double(FP_NORMAL);
		ldcx = uldcx.z;
		ldcy = cprojl(ldcx);
		if (isinf(creall(ldcx)) || isinf(cimagl(ldcx))) {
			ATF_PASS_OR_BREAK(isinf(creall(ldcy)));
			ATF_PASS_OR_BREAK(cimagl(ldcy) ==
					  copysignl(0.0, cimagl(ldcx)));
		} else {
			ATF_PASS_OR_BREAK(ldcy == ldcx);
		}
#endif
	}	/* ATF_FOR_LOOP */
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_cproj1);
	ATF_TP_ADD_TC(tp, test_cproj2);
	ATF_TP_ADD_TC(tp, test_cproj3);

	return atf_no_error();
}
