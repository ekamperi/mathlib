#include <atf-c.h>
#include <fenv.h>
#include <limits.h>	/* For INT_MAX */
#include <stdio.h>
#include <string.h>

#include "../subr_combinatorics.h"

/*
 * Each FE_* symbol shall expand to an _integer constant_ expression,
 * so we can safely use them as initializers to an array holding integers.
 */
int extable[] = {
#ifdef	FE_DIVBYZERO
	FE_DIVBYZERO,
#endif
#ifdef	FE_INEXACT
	FE_INEXACT,
#endif
#ifdef	FE_INVALID
	FE_INVALID,
#endif
#ifdef	FE_OVERFLOW
	FE_OVERFLOW,
#endif
#ifdef	FE_UNDERFLOW
	FE_UNDERFLOW,
#endif
#ifdef	FE_ALL_EXCEPT
	FE_ALL_EXCEPT
#endif
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_feraiseexcept);
ATF_TC_HEAD(test_feraiseexcept, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality");
}
ATF_TC_BODY(test_feraiseexcept, tc)
{
	int comb, mth, nelms;
	int ex;

	nelms = sizeof(extable) / sizeof(extable[0]);
	FOREACH_BITWISE_COMBO(extable, nelms, &mth, &comb) {
		/*
		 * Clear all the floating-point exceptions If this is not
		 * met, further execution results cannot be trusted
		 */
		ATF_REQUIRE(feclearexcept(FE_ALL_EXCEPT) == 0);

		/* A little bit paranoid */
		ex = fetestexcept(FE_ALL_EXCEPT);
		ATF_REQUIRE((ex & FE_ALL_EXCEPT) == 0);

		/* Raise an exception */
		ATF_CHECK(feraiseexcept(comb) == 0);

		/* Trust noone -- check for yourself */
		ex = fetestexcept(comb);
		ATF_CHECK((ex & comb) == comb);
	}
}

/*
 * Test case 2 -- Edge cases
 */
ATF_TC(test_feraiseexcept2);
ATF_TC_HEAD(test_feraiseexcept2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_feraiseexcept2, tc)
{
	/* If the argument is zero [snip], feraisexcept() shall return zero */
	ATF_CHECK(feraiseexcept(0) == 0);
}

/*
 * Test case 3 -- Abuse `excepts' argument
 */
ATF_TC(test_feraiseexcept3);
ATF_TC_HEAD(test_feraiseexcept3, tc)
{
  atf_tc_set_md_var(tc,
		    "descr",
		    "Abuse `excepts' argument");
}
ATF_TC_BODY(test_feraiseexcept3, tc)
{
	/* Clear all exceptions */
        ATF_CHECK(feclearexcept(FE_ALL_EXCEPT) == 0);

	if (feraiseexcept(INT_MAX) == 0) {
		ATF_CHECK(fetestexcept(FE_ALL_EXCEPT) == FE_ALL_EXCEPT);
	} else {
		ATF_CHECK(fetestexcept(FE_ALL_EXCEPT) == 0);
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_feraiseexcept);
	ATF_TP_ADD_TC(tp, test_feraiseexcept2);
	ATF_TP_ADD_TC(tp, test_feraiseexcept3);

	return atf_no_error();
}
