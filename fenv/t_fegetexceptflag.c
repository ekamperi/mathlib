#include <atf-c.h>
#include <fenv.h>
#include <limits.h>	/* For INT_MAX */
#include <stdio.h>
#include <stdlib.h>

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
ATF_TC(test_fegetexceptflag1);
ATF_TC_HEAD(test_fegetexceptflag1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "XXX");
}
ATF_TC_BODY(test_fegetexceptflag1, tc)
{
	fexcept_t flag;
	int comb, mth, nelms;

	nelms = sizeof(extable) / sizeof(extable[0]);
	FOREACH_BITWISE_COMBO(extable, nelms, &mth, &comb) {
		ATF_CHECK(fegetexceptflag(&flag, comb) == 0);
	}
}

/*
 * Test case 2 -- Abuse `excepts' argument
 */
ATF_TC(test_fegetexceptflag2);
ATF_TC_HEAD(test_fegetexceptflag2, tc)
{
  atf_tc_set_md_var(tc,
		    "descr",
		    "Abuse `excepts' argument");
}
ATF_TC_BODY(test_fegetexceptflag2, tc)
{
	int ex1, ex2 = -12345;

	ATF_REQUIRE(fegetexceptflag(&ex1, FE_ALL_EXCEPT) == 0);

	if (fegetexceptflag(&ex2, INT_MAX) == 0) {
		ATF_CHECK(ex1 == ex2);
	} else {
		ATF_CHECK(ex2 == -12345);
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fegetexceptflag1);
	ATF_TP_ADD_TC(tp, test_fegetexceptflag2);

	return atf_no_error();
}
