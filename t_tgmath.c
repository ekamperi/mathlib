#define _XOPEN_SOURCE 600

#include <atf-c.h>

#include "config.h"
#include "complex.h"
#ifdef	HAVE_TGMATH_H
#include <tgmath.h>
#endif

/*
 * Test case 1 -- Basic functionality
 */
#define	ATF_CHECK_SIZE(g, fx, dx, ldx, fcx, dcx, ldcx)		\
do {								\
	ATF_CHECK(sizeof(g(fx))   == sizeof(fx));		\
	ATF_CHECK(sizeof(g(dx))   == sizeof(dx));		\
	ATF_CHECK(sizeof(g(ldx))  == sizeof(ldx));		\
	ATF_CHECK(sizeof(g(fcx))  == sizeof(fcx));		\
	ATF_CHECK(sizeof(g(dcx))  == sizeof(dcx));		\
	ATF_CHECK(sizeof(g(ldcx)) == sizeof(ldcx));		\
}  while(0)

#define ATF_CHECK_SIZE2(g, fx,  fy,  dx,  dy,  ldx,  ldy,	\
	    fcx, fcy, dcx, dcy, ldcx, ldcy)			\
do {								\
	ATF_CHECK(sizeof(g(  fx,   fy)) == sizeof(  fx));	\
	ATF_CHECK(sizeof(g(  dx,   dy)) == sizeof(  dx));	\
	ATF_CHECK(sizeof(g( ldx,  ldy)) == sizeof( ldx));	\
	ATF_CHECK(sizeof(g( fcx,  fcy)) == sizeof( fcx));	\
	ATF_CHECK(sizeof(g( dcx,  dcy)) == sizeof( dcx));	\
	ATF_CHECK(sizeof(g(ldcx, ldcy)) == sizeof(ldcx));	\
}  while(0)

ATF_TC(test_tgmath);
ATF_TC_HEAD(test_tgmath, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check basic functionality");
}
ATF_TC_BODY(test_tgmath, tc)
{
	float fx, fy;
	float complex fcx, fcy;
	double dx, dy;
	double complex dcx, dcy;
	long double ldx, ldy;
	long double complex ldcx, ldcy;

#ifdef	HAVE_ACOS
	ATF_CHECK_SIZE(acos, fx,  dx,  ldx, fcx, dcx, ldcx);
#endif
#ifdef	HAVE_ASIN
	ATF_CHECK_SIZE(asin, fx,  dx,  ldx, fcx, dcx, ldcx);
#endif
#ifdef	HAVE_ATAN
	ATF_CHECK_SIZE(atan, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef	HAVE_ACOSH
	ATF_CHECK_SIZE(acosh, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef	HAVE_ASINH
	ATF_CHECK_SIZE(asinh, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef	HAVE_ATANH
	ATF_CHECK_SIZE(atanh, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef	HAVE_COS
	ATF_CHECK_SIZE(cos, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef  HAVE_SIN
	ATF_CHECK_SIZE(sin, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef  HAVE_TAN
	ATF_CHECK_SIZE(tan, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef  HAVE_COSH
	ATF_CHECK_SIZE(cosh, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef  HAVE_SINH
	ATF_CHECK_SIZE(sinh, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef  HAVE_TANH
	ATF_CHECK_SIZE(tanh, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef  HAVE_EXP
	ATF_CHECK_SIZE(exp, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef  HAVE_LOG
	ATF_CHECK_SIZE(log, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef  HAVE_POW
	ATF_CHECK_SIZE2(pow, fx, fy, dx, dy, ldx, ldy,	\
			fcx, fcy, dcx, dcy, ldcx, ldcy);
#endif
#ifdef  HAVE_SQRT
	ATF_CHECK_SIZE(sqrt, fx, dx, ldx, fcx, dcx, ldcx);
#endif
#ifdef  HAVE_FABS
	ATF_CHECK_SIZE(fabs, fx, dx, ldx, fcx, dcx, ldcx);
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_tgmath);

	return atf_no_error();
}
