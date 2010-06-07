#include <atf-c.h>
#include <fenv.h>
#include <limits.h>	/* For INT_MAX */
#include <math.h>
#include <signal.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_feclearexcept1);
ATF_TC_HEAD(test_feclearexcept1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality");
}
ATF_TC_BODY(test_feclearexcept1, tc)
{
	/* Raise all the exceptions */
	ATF_REQUIRE(feraiseexcept(FE_ALL_EXCEPT) == 0);

	/* Be paranoid and make sure that they were raised */
	ATF_REQUIRE(fetestexcept(FE_ALL_EXCEPT) == FE_ALL_EXCEPT);

	/* Now clear them */
	ATF_CHECK(feclearexcept(FE_ALL_EXCEPT) == 0);

	/* ... and make sure all exceptions were indeed cleared */
	ATF_CHECK(fetestexcept(FE_ALL_EXCEPT) == 0);
}

/*
 * Test case 2 -- Edge cases
 */
ATF_TC(test_feclearexcept2);
ATF_TC_HEAD(test_feclearexcept2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_feclearexcept2, tc)
{
	/* If the argument is zero [snip], feclearexcept() shall return zero */
	ATF_CHECK(feclearexcept(0) == 0);
}

/*
 * Test case 3 -- Abuse `excepts' argument
 */
ATF_TC(test_feclearexcept3);
ATF_TC_HEAD(test_feclearexcept3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Abuse `excepts' argument");
}
ATF_TC_BODY(test_feclearexcept3, tc)
{
	/* Raise all the exceptions */
	ATF_REQUIRE(feraiseexcept(FE_ALL_EXCEPT) == 0);

	/* Be paranoid and make sure that they were raised */
	ATF_REQUIRE(fetestexcept(FE_ALL_EXCEPT) == FE_ALL_EXCEPT);

	/*
	 * Now clear them, but use a superset of all FE_ALL_EXCEPT.
	 * If it fails, require that we didn't end up with half-baked
	 * exception set (POSIX doesn't require this, it's just us).
	 */
	if (feclearexcept(INT_MAX) == 0) {
		ATF_CHECK(fetestexcept(FE_ALL_EXCEPT) == 0);
	} else {
		ATF_CHECK(fetestexcept(FE_ALL_EXCEPT) == FE_ALL_EXCEPT);
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_feclearexcept1);
	ATF_TP_ADD_TC(tp, test_feclearexcept2);
	ATF_TP_ADD_TC(tp, test_feclearexcept3);

	return atf_no_error();
}
