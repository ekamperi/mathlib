#include <atf-c.h>
#include <math.h>

#include "config.h"
#include "subr_fpcmp.h"
#include "t_cbrt.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_cbrt1);
ATF_TC_HEAD(test_cbrt1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, cbrt(x)) pairs");
}
ATF_TC_BODY(test_cbrt1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fpreal_equal(
			    cbrt(t1dtable[i].x),
				 t1dtable[i].y));
	}

	/* long double */
#ifdef	HAVE_CBRTL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	ATF_REQUIRE(N > 0);
        for (i = 0; i < N; i++) {
                ATF_CHECK(fpreal_equall(
			    cbrtl(t1ldtable[i].x),
				  t1ldtable[i].y));
        }
#endif
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;
	long double y;
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, x shall be returned */
	{  0.0,  0.0 },
	{ -0.0, -0.0 },

	/* If x is +-Inf, x shall be returned */
#ifdef	INFINITY
	{  INFINITY,  INFINITY },
	{ -INFINITY, -INFINITY },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL,  HUGE_VAL },
	{ -HUGE_VAL, -HUGE_VAL },
#endif
#ifdef	HUGE_VALF
	{  HUGE_VALF,  HUGE_VALF },
	{ -HUGE_VALF, -HUGE_VALF },
#endif
#ifdef	HUGE_VALL
	{  HUGE_VALL,  HUGE_VALL },
	{ -HUGE_VALL, -HUGE_VALL }
#endif
};

ATF_TC(test_cbrt2);
ATF_TC_HEAD(test_cbrt2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_cbrt2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpreal_equalf(
			    cbrtf((float)t2table[i].x),
				  (float)t2table[i].y));

		/* double */
		ATF_CHECK(fpreal_equal(
			    cbrt((double)t2table[i].x),
				 (double)t2table[i].y));

		/* long double */
#ifdef	HAVE_CBRTL
		ATF_CHECK(fpreal_equall(
			    cbrtl(t2table[i].x),
				  t2table[i].y));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_cbrt1);
	ATF_TP_ADD_TC(tp, test_cbrt2);

	return atf_no_error();
}
