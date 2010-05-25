#include <atf-c.h>
#include <fenv.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

/*
 * Test case 1 -- fegetenv()
 */
ATF_TC(test_fegetenv);
ATF_TC_HEAD(test_fegetenv, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "fegetenv()");
}
ATF_TC_BODY(test_fegetenv, tc)
{
	fenv_t env;

	ATF_CHECK(fegetenv(&env) == 0);
}

/*
 *
 * Test case 2 -- fesetenv()
 */
ATF_TC(test_fesetenv);
ATF_TC_HEAD(test_fesetenv, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "fesetenv()");
}
ATF_TC_BODY(test_fesetenv, tc)
{
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fegetenv);
	ATF_TP_ADD_TC(tp, test_fesetenv);

	return atf_no_error();
}
