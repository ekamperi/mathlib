#include "config.h"

#include <atf-c.h>
#ifdef	HAVE_FENV_H
#include <fenv.h>
#endif
#ifdef	HAVE_IEEEFP_H
#include <ieeefp.h>
#endif

#pragma STDC FENV_ACCESS ON

/*
 * Test case 1
 */
ATF_TC(test_ieeefp1);
ATF_TC_HEAD(test_ieeefp1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check whether fenv.h (standard) and ieeefp.h (non-standard) "
	    "can peacefully co-exist");
}
ATF_TC_BODY(test_ieeefp1, tc)
{
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_ieeefp1);

	return atf_no_error();
}
