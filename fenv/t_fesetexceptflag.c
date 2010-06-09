#include <atf-c.h>
#include <fenv.h>
#include <limits.h>
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
#ifdef FE_ALL_EXCEPT
	FE_ALL_EXCEPT
#endif
};

/*
 * Test case 1 -- basic functionality
 */
ATF_TC(test_fesetexceptflag1);
ATF_TC_HEAD(test_fesetexceptflag1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check basic functionality");
}
ATF_TC_BODY(test_fesetexceptflag1, tc)
{
	fexcept_t flag;
	int comb, mth, nelms;

	/*
	 * fesetexceptflag() shall only set the status flags. It shall not raise
	 * any exceptions. So, clear all the exceptions and see if any was
	 * raised later on.
	 */
	ATF_REQUIRE(feclearexcept(FE_ALL_EXCEPT) == 0);

	nelms = sizeof(extable) / sizeof(extable[0]);
	FOREACH_BITWISE_COMBO(extable, nelms, &mth, &comb) {
		ATF_CHECK(fegetexceptflag(&flag, comb) == 0);
		ATF_CHECK(fesetexceptflag(&flag, comb) == 0);
	}

	ATF_CHECK((fetestexcept(FE_ALL_EXCEPT) & FE_ALL_EXCEPT) == 0);
}

/*
 * Test case 2 -- handle a special case
 */
ATF_TC(test_fesetexceptflag2);
ATF_TC_HEAD(test_fesetexceptflag2, tc)
{
  atf_tc_set_md_var(tc,
		    "descr",
		    "Handle a special case");
}
ATF_TC_BODY(test_fesetexceptflag2, tc)
{
	fexcept_t flag;

	/*
	 * If the excepts argument is zero, fesetexceptflag() shall return zero.
	 *
	 * We purposefully don't set the value of `flag1' by a previous call to
	 * fegetexceptflag(). Also, just like before check whether a spurious
	 * exception is raised.
	 */
	ATF_REQUIRE(feclearexcept(FE_ALL_EXCEPT) == 0);

	ATF_CHECK(fesetexceptflag(&flag, 0) == 0);

	ATF_CHECK((fetestexcept(FE_ALL_EXCEPT) & FE_ALL_EXCEPT) == 0);
}

/*
 *
 * Test case 3 -- Abuse `excepts' argument
 */
ATF_TC(test_fesetexceptflag3);
ATF_TC_HEAD(test_fesetexceptflag3, tc)
{
  atf_tc_set_md_var(tc,
		    "descr",
		    "Abuse `excepts' argument");
}
ATF_TC_BODY(test_fesetexceptflag3, tc)
{
	fexcept_t flag1, flag2;
	unsigned char *p;
	size_t i;

	/*
	 * We are going to compare them later on, so make sure
	 * that they are indeed different at the first place.
	 */
	memset(&flag1, 0x00, sizeof(flag1));
	memset(&flag2, 0xFF, sizeof(flag2));

	/*
	 * Pass a superset of FE_ALL_EXCEPT to fesetexceptflag()
	 * and see what happens.
	 */
	ATF_REQUIRE(feclearexcept(FE_ALL_EXCEPT) == 0);

	ATF_REQUIRE(fegetexceptflag(&flag1, FE_ALL_EXCEPT) == 0);
	if (fesetexceptflag(&flag1, INT_MAX) == 0) {
		ATF_REQUIRE(fegetexceptflag(&flag2, FE_ALL_EXCEPT) == 0);
		ATF_CHECK(memcmp(&flag1, &flag2, sizeof(flag1)) == 0);
	} else {
		p = (unsigned char *)&flag2;
		for (i = 0; i < sizeof(flag2); i++)
			ATF_CHECK(p[i] == 0xFF);
	}

	ATF_CHECK((fetestexcept(FE_ALL_EXCEPT) & FE_ALL_EXCEPT) == 0);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fesetexceptflag1);
	ATF_TP_ADD_TC(tp, test_fesetexceptflag2);
	ATF_TP_ADD_TC(tp, test_fesetexceptflag3);

	return atf_no_error();
}
