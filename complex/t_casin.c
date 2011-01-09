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
ATF_TC(test_casin1);
ATF_TC_HEAD(test_casin1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check against a list of good (x, casin(x)) pairs");
}
ATF_TC_BODY(test_casin1, tc)
{
}

/*
 * Test case 2
 */
ATF_TC(test_casin2);
ATF_TC_HEAD(test_casin2, tc)
{
        atf_tc_set_md_var(tc,
            "descr",
	    "Check if result is in the interval [-Pi/2, Pi/2] "
	    "along the real axis.");
}
ATF_TC_BODY(test_casin2, tc)
{
	float complex fcx, fcy;
	double complex dcx, dcy;
	long double complex ldcx, ldcy;
	long i, N;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fcx = random_float_complex(FP_NORMAL);
		fcy = casinf(fcx);
		ATF_PASS_OR_BREAK(crealf(fcy) >= -(float)M_PI_2
			       && crealf(fcy) <=  (float)M_PI_2);

		/* double */
                dcx = random_double_complex(FP_NORMAL);
                dcy = casin(dcx);
                ATF_PASS_OR_BREAK(creal(dcy) >= -M_PI_2
			       && creal(dcy) <=  M_PI_2);

		/* long double */
#if defined(HAVE_CASINL) && defined(HAVE_CREALL)
                ldcx = random_long_double_complex(FP_NORMAL);
                ldcy = casinl(ldcx);
                ATF_PASS_OR_BREAK(creall(ldcy) >= -M_PI_2l
			       && creall(ldcy) <=  M_PI_2l);
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_casin1);
	ATF_TP_ADD_TC(tp, test_casin2);

	return atf_no_error();
}
