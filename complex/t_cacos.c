#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <complex.h>
#include <math.h>

#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1
 */
ATF_TC(test_cacos1);
ATF_TC_HEAD(test_cacos1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check against a list of good (x, cacos(x)) pairs");
}
ATF_TC_BODY(test_cacos1, tc)
{
}

/*
 * Test case 2
 */
ATF_TC(test_cacos2);
ATF_TC_HEAD(test_cacos2, tc)
{
        atf_tc_set_md_var(tc,
            "descr",
	    "Check if result is in the interval [0, Pi] along the real axis.");
}
ATF_TC_BODY(test_cacos2, tc)
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
		fcy = cacosf(fcx);
		ATF_PASS_OR_BREAK(crealf(fcy) >= 0 && crealf(fcy) <= M_PI);

		/* double */
                dcx = random_double_complex(FP_NORMAL);
                dcy = cacos(dcx);
                ATF_PASS_OR_BREAK(creal(dcy) >= 0 && crealf(dcy) <= M_PI);

		/* long double */
#if defined(HAVE_CACOSL) && defined(HAVE_CREALL)
                ldcx = random_long_double_complex(FP_NORMAL);
                ldcy = cacosl(ldcx);
                ATF_PASS_OR_BREAK(creall(ldcy) >= 0 && creall(ldcy) <= M_PI);
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_cacos1);
	ATF_TP_ADD_TC(tp, test_cacos2);

	return atf_no_error();
}
