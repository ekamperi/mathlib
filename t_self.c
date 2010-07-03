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
	    "Check that our random generator outpus valid fp numbers");
}
ATF_TC_BODY(test_rand1, tc)
{
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	/*
	 * This check may look like totally pointless, but it's not.
	 * It really catches invalid floating point numbers.
	 */
	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		fy = random_float(FP_NORMAL);
		ATF_PASS_OR_BREAK(fx < fy || fx >= fy);
		ATF_PASS_OR_BREAK(fpclassify(1.0 * fx) == FP_NORMAL);
		ATF_PASS_OR_BREAK(fpclassify(1.0 * fy) == FP_NORMAL);

                /* double */
                dx = random_double(FP_NORMAL);
                dy = random_double(FP_NORMAL);
                ATF_PASS_OR_BREAK(dx < dy || dx >= dy);
                ATF_PASS_OR_BREAK(fpclassify(1.0 * dx) == FP_NORMAL);
                ATF_PASS_OR_BREAK(fpclassify(1.0 * dy) == FP_NORMAL);

                /* long double */
                ldx = random_long_double(FP_NORMAL);
                ldy = random_long_double(FP_NORMAL);
                ATF_PASS_OR_BREAK(ldx < ldy || ldx >= ldy);
                ATF_PASS_OR_BREAK(fpclassify(1.0 * ldx) == FP_NORMAL);
                ATF_PASS_OR_BREAK(fpclassify(1.0 * ldy) == FP_NORMAL);
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_rand1);

	return atf_no_error();
}
