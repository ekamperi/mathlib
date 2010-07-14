#define _XOPEN_SOURCE 600

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

#define ATF_CHECK_SIZE2(g, fx, fy, dx, dy, ldx, ldy)	\
do {							\
	ATF_CHECK(sizeof(g(fx, fy))  == sizeof(fx));	\
	ATF_CHECK(sizeof(g(dx, dy))  == sizeof(dx));	\
	ATF_CHECK(sizeof(g(ldx, ldy)) == sizeof(ldx));	\
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
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;

#ifdef	HAVE_ACOS
	ATF_CHECK_SIZE(acos, fx, dx, ldx);
#endif
#ifdef	HAVE_ASIN
	ATF_CHECK_SIZE(asin, fx, dx, ldx);
#endif
#ifdef	HAVE_ATAN
	ATF_CHECK_SIZE(atan, fx, dx, ldx);
#endif
#ifdef	HAVE_ACOSH
	ATF_CHECK_SIZE(acosh, fx, dx, ldx);
#endif
#ifdef	HAVE_ASINH
	ATF_CHECK_SIZE(asinh, fx, dx, ldx);
#endif
#ifdef	HAVE_ATANH
	ATF_CHECK_SIZE(atanh, fx, dx, ldx);
#endif
#ifdef	HAVE_COS
	ATF_CHECK_SIZE(cos, fx, dx, ldx);
#endif
#ifdef  HAVE_SIN
	ATF_CHECK_SIZE(sin, fx, dx, ldx);
#endif
#ifdef  HAVE_TAN
	ATF_CHECK_SIZE(tan, fx, dx, ldx);
#endif
#ifdef  HAVE_COSH
	ATF_CHECK_SIZE(cosh, fx, dx, ldx);
#endif
#ifdef  HAVE_SINH
	ATF_CHECK_SIZE(sinh, fx, dx, ldx);
#endif
#ifdef  HAVE_TANH
	ATF_CHECK_SIZE(tanh, fx, dx, ldx);
#endif
#ifdef  HAVE_EXP
	ATF_CHECK_SIZE(exp, fx, dx, ldx);
#endif
#ifdef  HAVE_LOG
	ATF_CHECK_SIZE(log, fx, dx, ldx);
#endif
#ifdef  HAVE_POW
	ATF_CHECK_SIZE2(pow, fx, fy, dx, dy, ldx, ldy);
#endif
#ifdef  HAVE_SQRT
	ATF_CHECK_SIZE(sqrt, fx, dx, ldx);
#endif
#ifdef  HAVE_FABS
	ATF_CHECK_SIZE(fabs, fx, dx, ldx);
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_tgmath);

	return atf_no_error();
}
