#include <atf-c.h>
#include <tgmath.h>

#include "config.h"

/*
 * Test case 1 -- Basic functionality
 */
#define	ATF_CHECK_SIZE(g, fx, dx, ldx)			\
do {							\
	ATF_CHECK(sizeof(g(fx))  == sizeof(fx));	\
	ATF_CHECK(sizeof(g(dx))  == sizeof(dx));	\
	ATF_CHECK(sizeof(g(ldx)) == sizeof(ldx));	\
}  while(0)

ATF_TC(test_tgmath);
ATF_TC_HEAD(test_tgmath, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "");
}
ATF_TC_BODY(test_tgmath, tc)
{
	float fx;
	double dx;
	long double ldx;

#ifdef	HAVE_ACOS
	ATF_CHECK_SIZE(acos, fx, dx, ldx);
	acos(fx);
#endif
#ifdef	HAVE_ASIN
	ATF_CHECK_SIZE(asin, fx, dx, ldx);
#endif

#ifdef	HAVE_ATAN
#endif

#ifdef	HAVE_ACOSH
#endif

#ifdef	HAVE_ASINH
#endif

#ifdef	HAVE_ATANH
#endif

#ifdef	HAVE_COS
#endif

#ifdef  HAVE_SIN
#endif

#ifdef  HAVE_TAN
#endif

#ifdef  HAVE_COSH
#endif

#ifdef  HAVE_SINH
#endif

#ifdef  HAVE_TANH
#endif

#ifdef  HAVE_EXP
#endif

#ifdef  HAVE_LOG
#endif

#ifdef  HAVE_POW
#endif

#ifdef  HAVE_SQRT
#endif

#ifdef  HAVE_FABS
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_tgmath);

	return atf_no_error();
}
