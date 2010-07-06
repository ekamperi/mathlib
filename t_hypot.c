#define	_XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <float.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_hypot.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_hypot1);
ATF_TC_HEAD(test_hypot1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good "
	    "((x,y), hypot(x,y)) pairs");
}
ATF_TC_BODY(test_hypot1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equal(
			    hypot(t1dtable[i].x, t1dtable[i].y),
				  t1dtable[i].z));
	}

	/* long double */
#ifdef	HAVE_HYPOTL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equall(
			    hypotl(t1ldtable[i].x, t1ldtable[i].y),
				   t1ldtable[i].z));
	}
#endif
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;	/* Input */
	long double y;	/* Input */
	long double z;	/* hypot() output */
} t2table[] = {
#ifdef	INFINITY
	/* If x is +-Inf, +Inf shall be returned */
	{  INFINITY,   0.0, INFINITY },
	{  INFINITY,   1.0, INFINITY },
	{  INFINITY,   2.0, INFINITY },
	{  INFINITY, 10E10, INFINITY },

	{ -INFINITY,   0.0, INFINITY },
	{ -INFINITY,   1.0, INFINITY },
	{ -INFINITY,   2.0, INFINITY },
	{ -INFINITY, 10E10, INFINITY },

	/* If y is +-Inf, +Inf shall be returned */
	{    0.0, INFINITY, INFINITY },
	{    1.0, INFINITY, INFINITY },
	{    2.0, INFINITY, INFINITY },
	{  10E10, INFINITY, INFINITY },

	{   0.0, -INFINITY, INFINITY },
	{   1.0, -INFINITY, INFINITY },
	{   2.0, -INFINITY, INFINITY },
	{ 10E10, -INFINITY, INFINITY },
#endif

	/* ... even if one of x or y is NaN */
#if defined(NAN) && defined(INFINITY)
	{ NAN,  INFINITY, INFINITY },
	{ NAN, -INFINITY, INFINITY },

	{  INFINITY, NAN, INFINITY },
	{ -INFINITY, NAN, INFINITY },
#endif

	/*
	 * If x or y is NaN, and the other is not +-Inf,
	 * a NaN shall be returned.
	 */
#if defined(NAN) && defined(INFINITY)
	{ NAN,   1.0, NAN },
	{ NAN,   2.0, NAN },
	{ NAN, 10E10, NAN },

	{   1.0, NAN, NAN },
	{   2.0, NAN, NAN },
	{ 10E10, NAN, NAN }
#endif
};

ATF_TC(test_hypot2);
ATF_TC_HEAD(test_hypot2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_hypot2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpcmp_equalf(
			    hypotf((float)t2table[i].x,
				   (float)t2table[i].y),
			    (float)t2table[i].z));

		/* double */
		ATF_CHECK(fpcmp_equal(
			    hypot((double)t2table[i].x,
				  (double)t2table[i].y),
			    (double)t2table[i].z));

		/* long double */
#ifdef	HAVE_HYPOTL
		ATF_CHECK(fpcmp_equall(
			    hypotl(t2table[i].x,
				   t2table[i].y),
			    t2table[i].z));
#endif
	}
}

/*
 * Test case 3 -- Check undue overflow
 */
ATF_TC(test_hypot3);
ATF_TC_HEAD(test_hypot3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "hypot() doesn't overflow in intermediate steps");
}
ATF_TC_BODY(test_hypot3, tc)
{
	int haserrexcept;
	int haserrno;

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/* This should NOT overflow */
	volatile double x, y;
	x = DBL_MAX / M_SQRT2;
	y = DBL_MAX / M_SQRT2;

	errno = 0;
	clear_exceptions();
	(void)hypot(x, y);
	ATF_CHECK(iserrno_equalto(0));
	ATF_CHECK(not_raised_exceptions(MY_FE_OVERFLOW));

	/* This should indeed overflow */
	x = DBL_MAX;
	y = DBL_MAX;

	clear_exceptions();
	(void)hypot(x, y);
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
}

/*
 * Test case 4 -- Check undue underflow
 */
ATF_TC(test_hypot4);
ATF_TC_HEAD(test_hypot4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "hypot() doesn't underflow in intermediate steps");
}
ATF_TC_BODY(test_hypot4, tc)
{
	int haserrexcept;
	int haserrno;

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	volatile double x, y;
	x = DBL_MIN;
	y = DBL_MIN;

	errno = 0;
	clear_exceptions();
	(void)hypot(x, y);
	ATF_CHECK(iserrno_equalto(0));
	ATF_CHECK(not_raised_exceptions(MY_FE_UNDERFLOW));
}

/*
 * Test case 5 -- Check some identities
 */
ATF_TC(test_hypot5);
ATF_TC_HEAD(test_hypot5, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some identities");
}
ATF_TC_BODY(test_hypot5, tc)
{
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	/*
	 * hypot(x,y), hypot(y,x), and hypot(x, -y) are equivalent
	 * hypot(x, +-0) is equivalent to fabs(x)
	 */
	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		fy = random_float(FP_NORMAL);
		ATF_PASS_OR_BREAK(hypotf(fx,  fy) == hypotf(fy, fx));
		ATF_PASS_OR_BREAK(hypotf(fx, -fy) == hypotf(fy, fx));

		ATF_PASS_OR_BREAK(hypotf(fx,  0.0) == fabs(fx));
		ATF_PASS_OR_BREAK(hypotf(fx, -0.0) == fabs(fx));

		/* double */
		dx = random_double(FP_NORMAL);
		dy = random_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(hypot(dx,  dy) == hypot(dy, dx));
		ATF_PASS_OR_BREAK(hypot(dx, -dy) == hypot(dy, dx));

		ATF_PASS_OR_BREAK(hypot(dx,  0.0) == fabs(dx));
		ATF_PASS_OR_BREAK(hypot(dx, -0.0) == fabs(dx));

		/* long double */
#ifdef	HAVE_HYPOTL
		ldx = random_long_double(FP_NORMAL);
		ldy = random_long_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(hypotl(ldx,  ldy) == hypotl(ldy, ldx));
		ATF_PASS_OR_BREAK(hypotl(ldx, -ldy) == hypotl(ldy, ldx));

		ATF_PASS_OR_BREAK(hypotl(ldx,  0.0) == fabsl(ldx));
		ATF_PASS_OR_BREAK(hypotl(ldx, -0.0) == fabsl(ldx));
#endif
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_hypot1);
	ATF_TP_ADD_TC(tp, test_hypot2);
	ATF_TP_ADD_TC(tp, test_hypot3);
	ATF_TP_ADD_TC(tp, test_hypot4);
	ATF_TP_ADD_TC(tp, test_hypot5);

	return atf_no_error();
}
