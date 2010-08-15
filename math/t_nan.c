#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <stdlib.h>

#include "config.h"
#include "subr_fpcmp.h"

/*
 * Test case 1
 *
 * The function call nan("n-char-sequence") shall be equivalent to:
 * strtod("NAN(n-char-sequence)", (char **) NULL);
 */
ATF_TC(test_nan1);
ATF_TC_HEAD(test_nan1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality of nan*()");
}
ATF_TC_BODY(test_nan1, tc)
{
	/* float */
	ATF_CHECK(fpreal_equalf(
		    nanf("n-char-sequence"),
		    strtof("NAN(n-char-sequence)", (char **) NULL)));

	/* double */
	ATF_CHECK(fpreal_equal(
		    nan("n-char-sequence"),
		    strtod("NAN(n-char-sequence)", (char **) NULL)));

	/* long double */
#if defined(HAVE_NANL) && defined(HAVE_STRTOLD)
	ATF_CHECK(fpreal_equall(
		    nanl("n-char-sequence"),
		    strtold("NAN(n-char-sequence)", (char **) NULL)));
#endif
}

/*
 * Test case 2
 *
 * The function call nan("") shall be equivalent to:
 * strtod("NAN()", (char **) NULL)
 */
ATF_TC(test_nan2);
ATF_TC_HEAD(test_nan2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_nan2, tc)
{
	/* float */
	ATF_CHECK(fpreal_equalf(
		    nanf(""),
		    strtof("NAN()", (char **) NULL)));

	/* double */
	ATF_CHECK(fpreal_equal(
		    nan(""),
		    strtod("NAN()", (char **) NULL)));

	/* long double */
#if defined(HAVE_NANL) && defined(HAVE_STRTOLD)
	ATF_CHECK(fpreal_equall(
		    nanl(""),
		    strtold("NAN()", (char **) NULL)));
#endif
}


/*
 * Test case 3
 *
 * If tagp does not point to an n- char sequence or an empty string,
 * the function call shall be equivalent to:
 * strtod("NAN", (char **) NULL)
 */
ATF_TC(test_nan3);
ATF_TC_HEAD(test_nan3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_nan3, tc)
{
	/* float */
	ATF_CHECK(fpreal_equalf(
		    nanf(NULL),
		    strtof("NAN", (char **) NULL)));

	/* double */
	ATF_CHECK(fpreal_equal(
		    nan(NULL),
		    strtod("NAN", (char **) NULL)));

	/* long double */
#if defined(HAVE_NANL) && defined(HAVE_STRTOLD)
	ATF_CHECK(fpreal_equall(
		    nanl(NULL),
		    strtold("NAN", (char **) NULL)));
#endif
}

/*
 * Test case 4
 */
ATF_TC(test_nan4);
ATF_TC_HEAD(test_nan4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check that NAN is ... NAN");
}
ATF_TC_BODY(test_nan4, tc)
{
	ATF_CHECK(isnan(NAN));
	ATF_CHECK(isnan(nan("")));
	ATF_CHECK(isnan(nanf("")));
#ifdef	HAVE_NANL
	ATF_CHECK(isnan(nanl("")));
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_nan1);
	ATF_TP_ADD_TC(tp, test_nan2);
	ATF_TP_ADD_TC(tp, test_nan3);
	ATF_TP_ADD_TC(tp, test_nan4);

	return atf_no_error();
}
