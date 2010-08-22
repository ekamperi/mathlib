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

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_cproj1);
	ATF_TP_ADD_TC(tp, test_cproj2);

	return atf_no_error();
}
