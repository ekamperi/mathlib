#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_remquo1);
ATF_TC_HEAD(test_remquo1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of "
	    "good (x, y, remquo(x, y)) pairs");
}
ATF_TC_BODY(test_remquo1, tc)
{
}

/*
 * Test case 2 -- Edge cases
 */
ATF_TC(test_remquo2);
ATF_TC_HEAD(test_remquo2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x or y is NaN, a NaN shall be returned");
}
ATF_TC_BODY(test_remquo2, tc)
{
#ifdef	NAN
	float fx, fy, fz;
	double dx, dy, dz;
	long double ldx, ldy, ldz;
	long i, N;
	int quo;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = rand() % 2 ? random_float(FP_NORMAL) : NAN;
		fy = rand() % 2 ? random_float(FP_NORMAL) : NAN;
		fz = remquof(fx, fy, &quo);

		if (isnan(fx) || isnan(fy))
			ATF_PASS_OR_BREAK(isnan(fz));
		else
			ATF_PASS_OR_BREAK(!isnan(fz));

		/* double */
		dx = rand() % 2 ? random_double(FP_NORMAL) : NAN;
		dy = rand() % 2 ? random_double(FP_NORMAL) : NAN;
		dz = remquo(dx, dy, &quo);

		if (isnan(dx) || isnan(dy))
			ATF_PASS_OR_BREAK(isnan(dz));
		else
			ATF_PASS_OR_BREAK(!isnan(dz));

		/* long double */
#ifdef	HAVE_REMQUOL
		ldx = rand() % 2 ? random_long_double(FP_NORMAL) : NAN;
		ldy = rand() % 2 ? random_long_double(FP_NORMAL) : NAN;
		ldz = remquol(ldx, ldy, &quo);

		if (isnan(ldx) || isnan(ldy))
			ATF_PASS_OR_BREAK(isnan(ldz));
		else
			ATF_PASS_OR_BREAK(!isnan(ldz));
#endif	/* HAVE_REMQUOL */
	}
#endif	/* NAN */
}

/*
 * Test case 3 -- Edge cases
 *
 * If x is infinite or y is 0 and the other is non-NaN, a domain error
 * shall occur, and either a NaN (if supported), or an implementation-
 * defined value shall be returned.
 */
static const struct
t3entry {
	long double x;
	long double y;
} t3table[] = {
#ifdef  INFINITY
	{  INFINITY,  0.0 },
	{  INFINITY, -0.0 },
	{ -INFINITY,  0.0 },
	{ -INFINITY, -0.0 },
#endif
#ifdef  HUGE_VAL
	{  HUGE_VAL,  0.0 },
	{  HUGE_VAL, -0.0 },
	{ -HUGE_VAL,  0.0 },
	{ -HUGE_VAL, -0.0 },
#endif
#ifdef  HUGE_VALF
	{  HUGE_VALF,  0.0 },
	{  HUGE_VALF, -0.0 },
	{ -HUGE_VALF,  0.0 },
	{ -HUGE_VALF, -0.0 },
#endif
#ifdef  HUGE_VALL
	{  HUGE_VALL,  0.0 },
	{  HUGE_VALL, -0.0 },
	{ -HUGE_VALL,  0.0 },
	{ -HUGE_VALL, -0.0 }
#endif
};

ATF_TC(test_remquo3);
ATF_TC_HEAD(test_remquo3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "x is infinite or y is 0 and the other is non-NaN");

}
ATF_TC_BODY(test_remquo3, tc)
{
	float fx, fy, fz;
	double dx, dy, dz;
	long double ldx, ldy, ldz;
	long i, N, idx;
	int haserrexcept, haserrno, quo;

        N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	/* XXX: ATF_CHECK_IFNAN -> ATF_PASS_OR_BREAK_... */
	ATF_FOR_LOOP(i, N, i++) {
		idx = i % ( sizeof(t3table) / sizeof(t3table[0]) );

		/* float */
		errno = 0;
		clear_exceptions();
		fx = rand() % 2 ? (float)t3table[idx].x : random_float(FP_NORMAL);
		fy = rand() % 2 ? (float)t3table[idx].y : random_float(FP_NORMAL);
		fz = remquof(fx, fy, &quo);

		if (isinf(fx) || isinf(fy) || iszero(fx) || iszero(fy)) {
			ATF_CHECK_IFNAN(fx);
			ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
			ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));
		} else {
			ATF_CHECK_NOTNAN(fz);
		}

		/* double */
		errno = 0;
		clear_exceptions();
		dx = rand() % 2 ? (double)t3table[idx].x : random_double(FP_NORMAL);
		dy = rand() % 2 ? (double)t3table[idx].y : random_double(FP_NORMAL);
		dz = remquo(dx, dy, &quo);

		if (isinf(dx) || isinf(dy) || iszero(dx) || iszero(dy)) {
			ATF_CHECK_IFNAN(dz);
			ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
			ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));
		} else {
			ATF_CHECK_NOTNAN(dz);
		}

		/* long double */
#ifdef	HAVE_REMQUOL
		errno = 0;
		clear_exceptions();
		ldx = rand() % 2 ? t3table[idx].x : random_long_double(FP_NORMAL);
		ldy = rand() % 2 ? t3table[idx].y : random_long_double(FP_NORMAL);
		ldz = remquol(ldx, ldy, &quo);

		if (isinf(ldx) || isinf(ldy) || iszero(ldx) || iszero(ldy)) {
			ATF_CHECK_IFNAN(ldz);
			ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
			ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));
		} else {
			ATF_CHECK_NOTNAN(ldz);
		}
#endif
	}

	/*
	 * Revenge is a Dish Best Served Cold :)
	 *
	 * We put this check here and not in the beginning of the test case,
	 * because we don't want the lack of error handling support to
	 * block the check against HUGE_VAL*. Nor do we want to split the test
	 * case into 2.
	 */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_remquo1);
	ATF_TP_ADD_TC(tp, test_remquo2);
	ATF_TP_ADD_TC(tp, test_remquo3);

	return atf_no_error();
}
