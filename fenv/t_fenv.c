#include <fenv.h>
#include <stdio.h>
#include <string.h>

#include <atf-c.h>

/*
 * Test case 1
 */
ATF_TC(test_fenv1);
ATF_TC_HEAD(test_fenv1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "FE_ALL_EXCEPT is the bitwise-inclusive OR "
	    "of all floating-point exception constants");
}
ATF_TC_BODY(test_fenv1, tc)
{
	/*
	 * The <fenv.h> header shall define the following constants
	 * _if and only if_ the implementation supports getting and setting.
	 * So, protect them with #ifdefs.
	 */
#ifdef	FE_DIVBYZERO
	ATF_CHECK(FE_ALL_EXCEPT & FE_DIVBYZERO);
#endif
#ifdef	FE_INEXACT
	ATF_CHECK(FE_ALL_EXCEPT & FE_INEXACT);
#endif
#ifdef	FE_INVALID
	ATF_CHECK(FE_ALL_EXCEPT & FE_INVALID);
#endif
#ifdef	FE_OVERFLOW
	ATF_CHECK(FE_ALL_EXCEPT & FE_OVERFLOW);
#endif
#ifdef	FE_UNDERFLOW
	ATF_CHECK(FE_ALL_EXCEPT & FE_UNDERFLOW);
#endif

	/*
	 * Note: We should allow for more FE_* symbols.
	 * For example, in x86 the FE_DENORMAL exception is supported.
	 */
}

/*
 * Test case 2 --
 */
ATF_TC(test_fenv2);
ATF_TC_HEAD(test_fenv2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Rounding direction integer constants shall "
	    "be non-negative and distinct");
}
ATF_TC_BODY(test_fenv2, tc)
{
	/* These values are non-negative */
#ifdef	FE_DOWNWARD
	ATF_CHECK(FE_DOWNWARD >= 0);
#endif
#ifdef	FE_TONEAREST
	ATF_CHECK(FE_TONEAREST >= 0);
#endif
#ifdef	FE_TOWARDZERO
	ATF_CHECK(FE_TOWARDZERO >= 0);
#endif
#ifdef	FE_UPWARD
	ATF_CHECK(FE_UPWARD >= 0);
#endif

	/* These values shall be distinct */
#ifdef	FE_DOWNWARD
#ifdef	FE_TONEAREST
	ATF_CHECK(FE_DOWNWARD != FE_TONEAREST);
#endif
#ifdef	FE_TOWARDZERO
	ATF_CHECK(FE_DOWNWARD != FE_TOWARDZERO);
#endif
#ifdef	FE_UPWARD
	ATF_CHECK(FE_DOWNWARD != FE_UPWARD);
#endif
#endif	/* FE_DOWNWARD */

#ifdef	FE_TONEAREST
#ifdef	FE_TOWARDZERO
	ATF_CHECK(FE_TONEAREST != FE_TOWARDZERO);
#endif
#ifdef	FE_UPWARD
	ATF_CHECK(FE_TONEAREST != FE_UPWARD);
#endif
#endif	/* FE_TONEAREST */

#if defined(FE_TOWARDZERO) && defined(FE_UPWARD)
	ATF_CHECK(FE_TOWARDZERO != FE_UPWARD);
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fenv1);
	ATF_TP_ADD_TC(tp, test_fenv2);

	return atf_no_error();
}
