#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <complex.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1
 */
ATF_TC(test_cacosh1);
ATF_TC_HEAD(test_cacosh1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check against a list of good (x, cacosh(x)) pairs");
}
ATF_TC_BODY(test_cacosh1, tc)
{
}

/*
 * Test case 2
 */
ATF_TC(test_cacosh2);
ATF_TC_HEAD(test_cacosh2, tc)
{
        atf_tc_set_md_var(tc,
            "descr",
	    "The range of a half-strip of non-negative values along the real "
	    "axis and in the interval [-i*Pi, +i*Pi] along the imaginary axis");
}
ATF_TC_BODY(test_cacosh2, tc)
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
		fcy = cacoshf(fcx);
		ATF_PASS_OR_BREAK(crealf(fcy) >= 0);
		ATF_PASS_OR_BREAK(cimagf(fcy) >= -I*M_PI
			       && cimagf(fcx) <=  I*M_PI);

		/* double */
                dcx = random_double_complex(FP_NORMAL);
                dcy = cacosh(dcx);
                ATF_PASS_OR_BREAK(creal(dcy) >= 0);

		/* long double */
#if defined(HAVE_CACOSHL) && defined(HAVE_CREALL)
                ldcx = random_long_double_complex(FP_NORMAL);
                ldcy = cacoshl(ldcx);
                ATF_PASS_OR_BREAK(creall(ldcy) >= 0);
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_cacosh1);
	ATF_TP_ADD_TC(tp, test_cacosh2);

	return atf_no_error();
}
