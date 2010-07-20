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
ATF_TC(test_catan1);
ATF_TC_HEAD(test_catan1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check against a list of good (x, catan(x)) pairs");
}
ATF_TC_BODY(test_catan1, tc)
{
}

/*
 * Test case 2
 */
ATF_TC(test_catan2);
ATF_TC_HEAD(test_catan2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "The range in the interval [-Pi/2, +Pi/2] "
	    "along the real axis");
}
ATF_TC_BODY(test_catan2, tc)
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
		fcy = catanf(fcx);
		ATF_PASS_OR_BREAK(crealf(fcy) >= -(float)M_PI_2
			       && crealf(fcx) <=  (float)M_PI_2);

		/* double */
		dcx = random_double_complex(FP_NORMAL);
		dcy = catan(dcx);
		ATF_PASS_OR_BREAK(creal(dcy) >= -M_PI_2
			       && creal(dcx) <=  M_PI_2);

		/* long double */
#if defined(HAVE_CATANL) && defined(HAVE_CREALL)
		ldcx = random_long_double_complex(FP_NORMAL);
		ldcy = catanl(ldcx);
		ATF_PASS_OR_BREAK(creall(ldcy) >= -M_PI_2l
			       && creall(ldcx) <=  M_PI_2l);
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_catan1);
	ATF_TP_ADD_TC(tp, test_catan2);

	return atf_no_error();
}
