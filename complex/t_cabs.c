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
ATF_TC(test_cabs1);
ATF_TC_HEAD(test_cabs1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check against a list of good (x, cabs(x)) pairs");
}
ATF_TC_BODY(test_cabs1, tc)
{
}

/*
 * Test case 2
 */
ATF_TC(test_cabs2);
ATF_TC_HEAD(test_cabs2, tc)
{
        atf_tc_set_md_var(tc,
            "descr",
	    "Check if cabs(z) and hypot(Re(z), Im(z)) agree");
}
ATF_TC_BODY(test_cabs2, tc)
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
		fcy = cabsf(fcx);
		ATF_PASS_OR_BREAK(fcy == hypotf(crealf(fcx), cimagf(fcx)));

		/* double */
                dcx = random_double_complex(FP_NORMAL);
                dcy = cabs(dcx);
                ATF_PASS_OR_BREAK(dcy == hypot(creal(dcx), cimag(dcx)));

		/* long double */
#if defined(HAVE_CABSL)  && defined(HAVE_HYPOTL) &&		\
    defined(HAVE_CREALL) && defined(HAVE_CIMAGL)
                ldcx = random_long_double_complex(FP_NORMAL);
                ldcy = cabsl(ldcx);
                ATF_PASS_OR_BREAK(ldcy == hypotl(creall(ldcx), cimagl(ldcx)));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_cabs1);
	ATF_TP_ADD_TC(tp, test_cabs2);

	return atf_no_error();
}
