#include <fenv.h>
#include <stdio.h>
#include <string.h>

#include <atf-c.h>

#pragma STDC FENV_ACCESS ON

/*
 * Test case 1 --
 */
ATF_TC(test_fenv1);
ATF_TC_HEAD(test_fenv1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "exception constants");
}
ATF_TC_BODY(test_fenv1, tc)
{
	int ex;

	/* Clear all the floating-point exceptions. */
	ATF_CHECK(feclearexcept(FE_ALL_EXCEPT) == 0);

	/* Make sure that no floating-point exception is set. */
	ex = fetestexcept(FE_ALL_EXCEPT);
	ATF_CHECK((ex & FE_ALL_EXCEPT) == 0);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fenv1);

	return atf_no_error();
}
