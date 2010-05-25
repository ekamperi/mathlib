#include <fenv.h>
#include <stdio.h>
#include <stdlib.h>

#include <atf-c.h>

/*
 * The <fenv.h> header shall define the following constants, if and only if the
 * implementation supports getting and setting the represented rounding
 * direction by means of the fegetround() and fesetround() functions. Therefore,
 * ifdef-guard the symbols.
 */
int rtable[] = {
#ifdef	FE_DOWNWARD
	FE_DOWNWARD,
#endif
#ifdef	FE_TONEAREST
	FE_TONEAREST,
#endif
#ifdef	FE_TOWARDZERO
	FE_TOWARDZERO,
#endif
#ifdef	FE_UPWARD
	FE_UPWARD,
#endif
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_fesetround);
ATF_TC_HEAD(test_fesetround, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_fesetround, tc)
{
#pragma STDC FENV_ACCESS ON
	size_t i, N;
	int rv, rounddir;

	N = sizeof(rtable) / sizeof(rtable[0]);
	for (i = 0; i < N; i++) {
		/* Set the rounding direction */
		rv = fesetround(rtable[i]);
		ATF_CHECK(rv == 0);

		/* ... and check if it was really set */
		if (rv == 0) {
			rounddir = fegetround();
			ATF_CHECK(rounddir >= 0);
			ATF_CHECK(rounddir == rtable[i]);
		}
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fesetround);

	return atf_no_error();
}
