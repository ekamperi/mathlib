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
ATF_TC(test_conj1);
ATF_TC_HEAD(test_conj1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check against a list of good (x, conj(x)) pairs");
}
ATF_TC_BODY(test_conj1, tc)
{
}

/*
 * Test case 2
 */
ATF_TC(test_conj2);
ATF_TC_HEAD(test_conj2, tc)
{
        atf_tc_set_md_var(tc,
            "descr",
	    "Check if result is in the right half-plane "
	    "(including the imaginary axis)");
}
ATF_TC_BODY(test_conj2, tc)
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
		ATF_PASS_OR_BREAK(conjf(conjf(fcx)) == fcx);

		/* double */
                dcx = random_double_complex(FP_NORMAL);
                ATF_PASS_OR_BREAK(conj(conj(dcx)) == dcx);

		/* long double */
#if defined(HAVE_CONJL)
                ldcx = random_long_double_complex(FP_NORMAL);
		ATF_PASS_OR_BREAK(conjl(conjl(ldcx)) == ldcx);
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_conj1);
	ATF_TP_ADD_TC(tp, test_conj2);

	return atf_no_error();
}
