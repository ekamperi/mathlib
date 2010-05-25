#include <atf-c.h>
#include <fenv.h>
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
	FE_UNDERFLOW
#endif
};

/*
 * fegetexceptflag()
 */
ATF_TC(test_fegetexceptflag);
ATF_TC_HEAD(test_fegetexceptflag, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "XXX");
}
ATF_TC_BODY(test_fegetexceptflag, tc)
{
	fexcept_t flag;
	int comb, mth, nelms;

	nelms = sizeof(extable) / sizeof(extable[0]);
	FOREACH_BITWISE_COMBO(extable, nelms, &mth, &comb) {
		ATF_CHECK(fegetexceptflag(&flag, comb) == 0);
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fegetexceptflag);

	return atf_no_error();
}
