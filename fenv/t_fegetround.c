#include <atf-c.h>
#include <fenv.h>
#include <stdio.h>
#include <stdlib.h>

#pragma STDC FENV_ACCESS ON

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_fegetground);
ATF_TC_HEAD(test_fegetground, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_fegetground, tc)
{
	int rounddir;
	rounddir = fegetround();
	ATF_CHECK(rounddir >= 0);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fegetground);

	return atf_no_error();
}
