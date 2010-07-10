#include <atf-c.h>
#include <fenv.h>
#include <ieeefp.h>	/* XXX: Use autoconf to check if available */
#include <stdio.h>
#include <string.h>

#pragma STDC FENV_ACCESS ON

/*
 * Test case 1
 */
ATF_TC(test_fenv1);
ATF_TC_HEAD(test_fenv1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check whether fenv.h (standard) and ieeepfp.h (non-standard)"
	    "can peacefully co-exist");
}
ATF_TC_BODY(test_fenv1, tc)
{
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fenv1);

	return atf_no_error();
}
