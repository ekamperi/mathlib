#define _XOPEN_SOURCE 600

#include "subr_fpcmp.h"
#include "subr_random.h"

#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Don't be too harsh with symbols */
#ifndef	M_PI
#define	M_PI	3.141592653589793238462643
#endif

#ifndef M_PI_2
#define M_PI_2	1.5707963267948966192313217
#endif

#define	ITERATIONS	10	/* XXX */

/*
 * Test case 1 -- Symmetry
 */
ATF_TC(test_symmetry);
ATF_TC_HEAD(test_symmetry, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Reflect trig functions from certain angles");
}
ATF_TC_BODY(test_symmetry, tc)
{
	double x;
	size_t i;
	srand48(time(NULL));
	for (i = 0; i < ITERATIONS; i++) {
		do {
			x = random_double(FP_NORMAL);
		} while (fabs(x) > 1);

		/* Reflected in x = 0 */
		ATF_CHECK(FPCMP_EQUAL(sin(-x), -sin(x)));
		ATF_CHECK(FPCMP_EQUAL(cos(-x), cos(x)));

		/* Reflected in x = pi/2 */
		ATF_CHECK(FPCMP_EQUAL(sin(M_PI_2 - x), cos(x)));
		ATF_CHECK(FPCMP_EQUAL(cos(M_PI_2 - x), sin(x)));

		/* Reflected in x = pi */
		ATF_CHECK(FPCMP_EQUAL(sin(M_PI - x), sin(x)));
		ATF_CHECK(FPCMP_EQUAL(cos(M_PI - x), -cos(x)));
	}
}

/*
 * Test case 2 -- Symmetry
 */
ATF_TC(test_shift);
ATF_TC_HEAD(test_shift, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "");
}
ATF_TC_BODY(test_shift, tc)
{
	double x;
	size_t i;
	srand48(time(NULL));

	for (i = 0; i < ITERATIONS; i++) {
		do {
			x = random_double(FP_NORMAL);
		} while (fabs(x) < 10E2);

		/* Shift by pi/2 */
		ATF_CHECK(FPCMP_EQUAL(sin(x + M_PI_2), cos(x)));
		ATF_CHECK(FPCMP_EQUAL(cos(x + M_PI_2), -sin(x)));

		/* Shift by pi */
		ATF_CHECK(FPCMP_EQUAL(sin(x + M_PI), -sin(x)));
		ATF_CHECK(FPCMP_EQUAL(cos(x + M_PI), -cos(x)));

		/* Shift by 2pi */
		ATF_CHECK(FPCMP_EQUAL(sin(x + 2*M_PI), sin(x)));
		ATF_CHECK(FPCMP_EQUAL(cos(x + 2*M_PI), cos(x)));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_symmetry);
//	ATF_TP_ADD_TC(tp, test_shift);

	return atf_no_error();
}
