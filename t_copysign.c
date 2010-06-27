#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>

#include "subr_atf.h"
#include "subr_random.h"

static const struct
tentry {
	double x;       /* Input */
	double y;       /* copysign output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_copysign1);
ATF_TC_HEAD(test_copysign1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, copysign(x)) pairs");
}
ATF_TC_BODY(test_copysign1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);

	for (i = 0; i < N; i++) {
	}
}

/*
 * Test case 2 -- Here comes the fuzzing
 */
ATF_TC(test_copysign2);
ATF_TC_HEAD(test_copysign2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Fuzz testing");
}
ATF_TC_BODY(test_copysign2, tc)
{
	float fx, fy, fz;
	double dx, dy, dz;
	long double ldx, ldy, ldz;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		dx = random_double(FP_NORMAL);
		dy = random_double(FP_NORMAL);

		fx = random_double(FP_NORMAL);
		fy = random_double(FP_NORMAL);

		ldx = random_double(FP_NORMAL);
		ldy = random_double(FP_NORMAL);

		/*
		 * Upon successful completion, these functions shall return a
		 * value with the magnitude of x and the sign of y.
		 */
		dz = copysignf(dx, dy);
		ATF_PASS_OR_BREAK(fabsf(dz) == fabsf(dx));
		ATF_PASS_OR_BREAK(signbit(dz) == signbit(dy));

		fz = copysign(fx, fy);
		ATF_PASS_OR_BREAK(fabs(fz) == fabs(fx));
		ATF_PASS_OR_BREAK(signbit(fz) == signbit(fy));

		ldz = copysignl(ldx, ldy);
		ATF_PASS_OR_BREAK(fabsl(ldz) == fabsl(ldx));
		ATF_PASS_OR_BREAK(signbit(ldz) == signbit(ldy));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_copysign1);
	ATF_TP_ADD_TC(tp, test_copysign2);

	return atf_no_error();
}
