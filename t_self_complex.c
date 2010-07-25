#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdio.h>

#include "subr_atf.h"
#include "subr_random.h"

ATF_TC(test_rand1);
ATF_TC_HEAD(test_rand1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check that our comparison functions work fine "
	    "for complex arithmetic");
}
ATF_TC_BODY(test_rand1, tc)
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
		ATF_PASS_OR_BREAK(
			fpcomplex_equalf(fcx, fcx));

		/* double */
		dcx = random_double_complex(FP_NORMAL);
		ATF_PASS_OR_BREAK(
			fpcomplex_equal(dcx, dcx));

		/* long double */
		ldcx = random_long_double_complex(FP_NORMAL);
		ATF_PASS_OR_BREAK(
			fpcomplex_equall(ldcx, ldcx));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_rand1);

	return atf_no_error();
}
