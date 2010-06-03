#define _XOPEN_SOURCE 600

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

#include <atf-c.h>
#include <errno.h>
#include <float.h>	/* for DBL_MAX */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef  HAS_MATH_ERREXCEPT
#include <fenv.h>
#endif  /* HAS_MATH_ERREXCEPT */

struct tentry {
	double x;       /* Input */
	double y;       /* tgamma() output */
} ttable[] = {
	{ 2.533333333333333E0, 1.361235032094808E0  },
	{ 5.060779216362095E0, 2.631137787195338E1  },
	{ 8.235382907247958E0, 8.129537123812583E3  },
	{ 1.193333333333333E1, 3.392481203047053E7  },
	{ 1.608307453166540E1, 1.642436513346779E12 },
	{ 2.063632614803888E1, 8.137058099116109E17 },
	{ 2.555764434627589E1, 3.716363528115563E24 },
	{ 3.081956706423009E1, 1.432426052161009E32 },
	{ 3.640000000000000E1, 4.318219406300267E40 },
	{ 4.228066525535389E1, 9.527369014812556E49 }
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_tgamma1);
ATF_TC_HEAD(test_tgamma1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, tgamma(x)) pairs");
}
ATF_TC_BODY(test_tgamma1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(FPCMP_EQUAL(tgamma(ttable[i].x), ttable[i].y));
}

/*
 * Test case 2 -- Identities
 */
ATF_TC(test_tgamma2);
ATF_TC_HEAD(test_tgamma2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check if G(z+1) = zG(z) identity holds for random input");
}
ATF_TC_BODY(test_tgamma2, tc)
{
	long i, N, cnt;

	/*
	 * We can't proceed if there's no way to detect errors,
	 * especially overflows.
	 */
	int haserrexcept;
	int haserrno;

	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/* First for real numbers */
	srand48(time(NULL));

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	cnt = 0;
	ATF_FOR_LOOP(i, N, i++) {
		/* Initialize error handling */
		clear_exceptions();
		clear_errno();

		double z, g0, g1;
		z = random_double(FP_NORMAL);

		g0 = tgamma(z);
		g1 = tgamma(z+1);

		/* If there was an error, just skip this value */
		if (raised_exceptions(FE_ALL_EXCEPT) || set_errno())
			continue;

		ATF_PASS_OR_BREAK(FPCMP_EQUAL(g1, z*g0));
		++cnt;
	}
	ATF_CHECK(cnt > i/2);
}

/*
 * Test case 3 -- Deliberate overflow
 */
ATF_TC(test_tgamma3);
ATF_TC_HEAD(test_tgamma3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "For IEEE Std 754-1985 double, overflow happens when "
	    "0 < x < 1/DBL_MAX, and 171.7 < x");
}
ATF_TC_BODY(test_tgamma3, tc)
{
	double x;
	size_t i;

	/*
	 * We can't proceed if there's no way to detect errors,
	 * especially overflows.
	 */
	int haserrexcept;
	int haserrno;

	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/* Initialize error handling */
	clear_exceptions();
	clear_errno();

	for (i = 0; i < 1000000; i++) {
		do {
			x = drand48();
		} while (x == 0.0);

		x += 171.7;
	}
}


/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_tgamma1);
	ATF_TP_ADD_TC(tp, test_tgamma2);
	ATF_TP_ADD_TC(tp, test_tgamma3);

	return atf_no_error();
}
