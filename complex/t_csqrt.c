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
ATF_TC(test_csqrt1);
ATF_TC_HEAD(test_csqrt1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check against a list of good (x, csqrt(x)) pairs");
}
ATF_TC_BODY(test_csqrt1, tc)
{
}

/*
 * Test case 2
 */
ATF_TC(test_csqrt2);
ATF_TC_HEAD(test_csqrt2, tc)
{
        atf_tc_set_md_var(tc,
            "descr",
	    "Check if result is in the right half-plane "
	    "(including the imaginary axis)");
}
ATF_TC_BODY(test_csqrt2, tc)
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
		fcy = csqrtf(fcx);
		ATF_PASS_OR_BREAK(crealf(fcy) >= 0.0);

		/* double */
                dcx = random_double_complex(FP_NORMAL);
                dcy = csqrt(dcx);
                ATF_PASS_OR_BREAK(creal(dcy) >= 0.0);

		/* long double */
#if defined(HAVE_CSQRTL) && defined(HAVE_CREALL)
                ldcx = random_long_double_complex(FP_NORMAL);
                ldcy = csqrtl(ldcx);
                ATF_PASS_OR_BREAK(creall(ldcy) >= 0.0);
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_csqrt1);
	ATF_TP_ADD_TC(tp, test_csqrt2);

	return atf_no_error();
}
