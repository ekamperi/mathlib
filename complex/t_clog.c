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

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_clog1);
	ATF_TP_ADD_TC(tp, test_clog2);

	return atf_no_error();
}
