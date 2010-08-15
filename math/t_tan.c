#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <errno.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "t_tan.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_tan1);
ATF_TC_HEAD(test_tan1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, tan(x)) pairs");
}
ATF_TC_BODY(test_tan1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equal(
			    tan(t1dtable[i].x),
				t1dtable[i].y));

	/* long double */
#ifdef	HAVE_TANL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equall(
			    tanl(t1ldtable[i].x),
				 t1ldtable[i].y));
#endif
}

/*
 * Test case 2 -- Edge cases
 */
ATF_TC(test_tan2);
ATF_TC_HEAD(test_tan2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_tan2, tc)
{
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	ATF_CHECK(isnan(tan(NAN)));
	ATF_CHECK(isnan(tanf(NAN)));
#ifdef	HAVE_TANL
	ATF_CHECK(isnan(tanl(NAN)));
#endif
#endif

	/* If x is +-0, x shall be returned */
	ATF_CHECK(iszero(tan( 0.0)));
	ATF_CHECK(iszero(tan(-0.0)));

	ATF_CHECK(iszero(tanf( 0.0)));
	ATF_CHECK(iszero(tanf(-0.0)));

#ifdef	HAVE_ATANL
	ATF_CHECK(iszero(tanl( 0.0)));
	ATF_CHECK(iszero(tanl(-0.0)));
#endif
}

/*
 * Test case 3 -- Edge cases
 *
 * If x is +-Inf, a domain error shall occur, and either a NaN (if
 * supported), or an implementation-defined value shall be returned.
 */
static const long double
t3table[] = {
#ifdef  INFINITY
	INFINITY,
	-INFINITY,
#endif
#ifdef  HUGE_VAL
	HUGE_VAL,
	-HUGE_VAL,
#endif
#ifdef  HUGE_VALF
	HUGE_VALF,
	-HUGE_VALF,
#endif
#ifdef  HUGE_VALL
	HUGE_VALL,
	-HUGE_VALL
#endif
};

ATF_TC(test_tan3);
ATF_TC_HEAD(test_tan3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases on infinities");
}
ATF_TC_BODY(test_tan3, tc)
{
	size_t i, N;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		ATF_CHECK_IFNAN(tanf((float)t3table[i]));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		ATF_CHECK_IFNAN(tan((double)t3table[i]));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_TANL
		errno = 0;
		clear_exceptions();
		ATF_CHECK_IFNAN(tanl(t3table[i]));
		ATF_CHECK(iserrno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
	}
}

/*
 * XXX: Test case 4 -- I couldn't reproduce an overflow no matter what
 *
 * If the correct value would cause overflow, a range error shall occur
 * and tan(), tanf(), and tanl() shall return +-HUGE_VAL, +-HUGE_VALF, and
 * +-HUGE_VALL, respectively, with the same sign as the correct value of
 * the function.
 */

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_tan1);
	ATF_TP_ADD_TC(tp, test_tan2);
	ATF_TP_ADD_TC(tp, test_tan3);

	return atf_no_error();
}
