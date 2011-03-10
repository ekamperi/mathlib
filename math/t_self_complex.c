#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

ATF_TC(test_self_complex1);
ATF_TC_HEAD(test_self_complex1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check that our comparison functions work fine "
	    "for complex arithmetic");
}
ATF_TC_BODY(test_self_complex1, tc)
{
	float complex fcx;
	double complex dcx;
	long double complex ldcx;
	long i, N;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
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

ATF_TC(test_self_complex2);
ATF_TC_HEAD(test_self_complex2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check that our random generators work fine "
	    "for complex numbers");
}
ATF_TC_BODY(test_self_complex2, tc)
{
	float complex fcx;
	double complex dcx;
	long double complex ldcx;
	long i, N;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fcx = random_float_complex(FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(crealf(fcx)) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(cimagf(fcx)) == FP_NORMAL);

		/* double */
		dcx = random_double_complex(FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(creal(dcx)) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(cimag(dcx)) == FP_NORMAL);

		/* long double */
#if defined(HAVE_CREALL) && defined(HAVE_CIMAGL)
		ldcx = random_long_double_complex(FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(creall(ldcx)) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(cimagl(ldcx)) == FP_NORMAL);
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_self_complex1);
	ATF_TP_ADD_TC(tp, test_self_complex2);

	return atf_no_error();
}
