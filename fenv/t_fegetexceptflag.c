#include <atf-c.h>
#include <fenv.h>
#include <limits.h>	/* For INT_MAX */
#include <stdio.h>
#include <stdlib.h>
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
	fexcept_t ex1, ex2;
	unsigned char *p;
	size_t i;

	/*
	 * We are going to compare them later on, so make sure
	 * that they are indeed different at the first place.
	 */
	memset(&ex1, 0x00, sizeof(ex1));
	memset(&ex2, 0xFF, sizeof(ex2));

	ATF_REQUIRE(fegetexceptflag(&ex1, FE_ALL_EXCEPT) == 0);

	/* Same as before, but use a superset of FE_ALL_EXCEPT */
	if (fegetexceptflag(&ex2, INT_MAX) == 0) {
	  printf("0x%x\n0x%x\n", ex1, ex2);
		ATF_CHECK(memcmp(&ex1, &ex2, sizeof(ex1)) == 0);
	} else {
		p = (unsigned char *)&ex2;
		for (i = 0; i < sizeof(ex2); i++)
			ATF_CHECK(p[i] == 0xFF);
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fegetexceptflag1);
	ATF_TP_ADD_TC(tp, test_fegetexceptflag2);

	return atf_no_error();
}
