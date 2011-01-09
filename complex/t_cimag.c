#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <complex.h>
#include <math.h>

#include "config.h"
#include "mytypes.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1
 */
ATF_TC(test_cimag1);
ATF_TC_HEAD(test_cimag1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check against a list of good (x, cimag(x)) pairs");
}
ATF_TC_BODY(test_cimag1, tc)
{
}

/*
 * Test case 2
 * Note: For more info on how this work, please look at include/mytypes.h
 */
ATF_TC(test_cimag2);
ATF_TC_HEAD(test_cimag2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check if cimag(z) works ok on random input");
}
ATF_TC_BODY(test_cimag2, tc)
{
	float complex fcx;
	float_complex ufcx;
	double complex dcx;
	double_complex udcx;
	long double complex ldcx;
	long_double_complex uldcx;
	long i, N;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fcx = random_float_complex(FP_NORMAL);
		ufcx.z = fcx;
		ATF_PASS_OR_BREAK(cimagf(fcx) == ufcx.parts[1]);

		/* double */
		dcx = random_double_complex(FP_NORMAL);
                udcx.z = dcx;
		ATF_PASS_OR_BREAK(cimag(dcx) == udcx.parts[1]);

		/* long double */
#if defined(HAVE_CIMAGL)
		ldcx = random_long_double_complex(FP_NORMAL);
		uldcx.z = ldcx;
		ATF_PASS_OR_BREAK(cimagl(ldcx) == uldcx.parts[1]);
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_cimag1);
	ATF_TP_ADD_TC(tp, test_cimag2);

	return atf_no_error();
}
