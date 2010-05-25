#include <fenv.h>
#include <stdio.h>
#include <string.h>

#include <atf-c.h>

/*
 * Test case 1 -- 
 */
ATF_TC(test_fenv1);
ATF_TC_HEAD(test_fenv1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "FE_ALL_EXCEPT is the bitwise-inclusive OR of all floating-point "
	    "exception constants");
}
ATF_TC_BODY(test_fenv1, tc)
{
	ATF_CHECK(FE_ALL_EXCEPT & FE_DIVBYZERO);
        ATF_CHECK(FE_ALL_EXCEPT & FE_INEXACT);
        ATF_CHECK(FE_ALL_EXCEPT & FE_INVALID);
        ATF_CHECK(FE_ALL_EXCEPT & FE_OVERFLOW);
        ATF_CHECK(FE_ALL_EXCEPT & FE_UNDERFLOW);

	/* XXX: Should we allow for more FE_* symbols ?  */
	ATF_CHECK((FE_ALL_EXCEPT
		& ~(FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW \
		    | FE_UNDERFLOW)) == 0);
}

/*
 * Test case 2 -- 
 */
ATF_TC(test_fenv2);
ATF_TC_HEAD(test_fenv2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Rounding direction integer constants shall be non-negative and distinct");
}
ATF_TC_BODY(test_fenv2, tc)
{
	/* These values are non-negative */
        ATF_CHECK(FE_DOWNWARD >= 0);
        ATF_CHECK(FE_TONEAREST >= 0);
        ATF_CHECK(FE_TOWARDZERO >= 0);
        ATF_CHECK(FE_UPWARD >= 0);

	/* These values shall be distinct */
	ATF_CHECK(FE_DOWNWARD != FE_TONEAREST);
	ATF_CHECK(FE_DOWNWARD != FE_TOWARDZERO);
	ATF_CHECK(FE_DOWNWARD != FE_UPWARD);

	ATF_CHECK(FE_TONEAREST != FE_TOWARDZERO);
	ATF_CHECK(FE_TONEAREST != FE_UPWARD);

	ATF_CHECK(FE_TOWARDZERO != FE_UPWARD);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fenv1);
	ATF_TP_ADD_TC(tp, test_fenv2);

	return atf_no_error();
}
