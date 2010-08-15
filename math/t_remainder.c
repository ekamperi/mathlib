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
#include "t_remainder.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_remainder1);
ATF_TC_HEAD(test_remainder1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of "
	    "good (x, y, remainder(x, y)) pairs");
}
ATF_TC_BODY(test_remainder1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equal(
			    remainder(t1dtable[i].x, t1dtable[i].y),
				      t1dtable[i].z));

	/* long double */
#ifdef  HAVE_REMAINDERL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpreal_equall(
			    remainderl(t1ldtable[i].x, t1ldtable[i].y),
				       t1ldtable[i].z));
#endif
}

/*
 * Test case 2 -- Edge cases
 */
ATF_TC(test_remainder2);
ATF_TC_HEAD(test_remainder2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x or y is NaN, a NaN shall be returned");
}
ATF_TC_BODY(test_remainder2, tc)
{
#ifdef	NAN
	float fx, fy, fz;
	double dx, dy, dz;
	long double ldx, ldy, ldz;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = rand() % 2 ? random_float(FP_NORMAL) : NAN;
		fy = rand() % 2 ? random_float(FP_NORMAL) : NAN;
		fz = remainderf(fx, fy);

		if (isnan(fx) || isnan(fy))
			ATF_PASS_OR_BREAK(isnan(fz));
		else
			ATF_PASS_OR_BREAK(!isnan(fz));

		/* double */
		dx = rand() % 2 ? random_double(FP_NORMAL) : NAN;
		dy = rand() % 2 ? random_double(FP_NORMAL) : NAN;
		dz = remainder(dx, dy);

		if (isnan(dx) || isnan(dy))
			ATF_PASS_OR_BREAK(isnan(dz));
		else
			ATF_PASS_OR_BREAK(!isnan(dz));

		/* long double */
#ifdef	HAVE_REMAINDERL
		ldx = rand() % 2 ? random_long_double(FP_NORMAL) : NAN;
		ldy = rand() % 2 ? random_long_double(FP_NORMAL) : NAN;
		ldz = remainderl(ldx, ldy);

		if (isnan(ldx) || isnan(ldy))
			ATF_PASS_OR_BREAK(isnan(ldz));
		else
			ATF_PASS_OR_BREAK(!isnan(ldz));
#endif	/* HAVE_REMAINDERL */
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

ATF_TC(test_remainder3);
ATF_TC_HEAD(test_remainder3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "x is infinite or y is 0 and the other is non-NaN");

}
ATF_TC_BODY(test_remainder3, tc)
{
	float fx, fy, fz;
	double dx, dy, dz;
	long double ldx, ldy, ldz;
	size_t i, N;
	int haserrexcept, haserrno;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	/* XXX: ATF_FOR_LOOP() */
	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		fx = rand() % 2 ? (float)t3table[i].x : random_float(FP_NORMAL);
		fy = rand() % 2 ? (float)t3table[i].y : random_float(FP_NORMAL);
		fz = remainderf(fx, fy);

		if (isinf(fx) || isinf(fy) || iszero(fx) || iszero(fy)) {
			ATF_CHECK_IFNAN(fz);
			ATF_CHECK(errno_equalto(EDOM));
			ATF_CHECK(raised_exceptions(MY_FE_INVALID));
		} else {
			ATF_CHECK_NOTNAN(fz);
		}

		/* double */
		errno = 0;
		clear_exceptions();
		dx = rand() % 2 ? (double)t3table[i].x : random_double(FP_NORMAL);
		dy = rand() % 2 ? (double)t3table[i].y : random_double(FP_NORMAL);
		dz = remainder(dx, dy);

		if (isinf(dx) || isinf(dy) || iszero(dx) || iszero(dy)) {
			ATF_CHECK_IFNAN(dz);
			ATF_CHECK(errno_equalto(EDOM));
			ATF_CHECK(raised_exceptions(MY_FE_INVALID));
		} else {
			ATF_CHECK_NOTNAN(dz);
		}

		/* long double */
#ifdef	HAVE_REMAINDERL
		errno = 0;
		clear_exceptions();
		ldx = rand() % 2 ? t3table[i].x : random_long_double(FP_NORMAL);
		ldy = rand() % 2 ? t3table[i].y : random_long_double(FP_NORMAL);
		ldz = remainderl(ldx, ldy);

		if (isinf(ldx) || isinf(ldy) || iszero(ldx) || iszero(ldy)) {
			ATF_CHECK_IFNAN(ldz);
			ATF_CHECK(errno_equalto(EDOM));
			ATF_CHECK(raised_exceptions(MY_FE_INVALID));
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
	ATF_TP_ADD_TC(tp, test_remainder1);
	ATF_TP_ADD_TC(tp, test_remainder2);
	ATF_TP_ADD_TC(tp, test_remainder3);

	return atf_no_error();
}
