#include <atf-c.h>
#include <math.h>

#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1 -- Basic functionality
 */
struct t1entry {
	long double x;	/* Input */
	long double z;	/* fdim output */
} t1table[] = {
};

ATF_TC(test_fdim1);
ATF_TC_HEAD(test_fdim1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, fdim(x)) pairs");
}
ATF_TC_BODY(test_fdim1, tc)
{
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++) {
	}
}

/*
 * Test case 2 -- Edge cases
 */
ATF_TC(test_fdim2);
ATF_TC_HEAD(test_fdim2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_fdim2, tc)
{
#ifdef	NAN
	float fx;
	double dx;
	long double ldx;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	/* If x or y is NaN, a NaN shall be returned */
	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		ATF_PASS_OR_BREAK(isnan(fdimf(fx, NAN)));
		ATF_PASS_OR_BREAK(isnan(fdimf(NAN, fx)));

		/* double */
		dx = random_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(isnan(fdim(dx, NAN)));
		ATF_PASS_OR_BREAK(isnan(fdim(NAN, dx)));

		/* long double */
		ldx = random_long_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(isnan(fdiml(ldx, NAN)));
		ATF_PASS_OR_BREAK(isnan(fdiml(NAN, ldx)));
	}
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fdim1);
	ATF_TP_ADD_TC(tp, test_fdim2);

	return atf_no_error();
}
