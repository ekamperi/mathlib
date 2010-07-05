#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <math.h>
#include <errno.h>
#include <float.h>
#include <stdio.h>	/* XXX: Remove test case 1 is fixed */
#include <stdlib.h>	/* RAND_MAX */

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"
#include "t_pow.h"

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_pow1);
ATF_TC_HEAD(test_pow1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, pow(x)) pairs");
}
ATF_TC_BODY(test_pow1, tc)
{
	size_t i, N;

	/* double */
	N = sizeof(t1dtable) / sizeof(t1dtable[0]);
	for (i = 0; i < N; i++) {
		if (!fpcmp_equal(
			    pow(t1dtable[i].x, t1dtable[i].x),
				t1dtable[i].z)) {
			printf("computed = %e,  exact = %e\n",
			    pow(t1dtable[i].x, t1dtable[i].x),
			    t1dtable[i].z);
		}

		ATF_CHECK(fpcmp_equal(
			    pow(t1dtable[i].x, t1dtable[i].x),
				t1dtable[i].z));
	}

	/* long double */
#ifdef	HAVE_POWL
	N = sizeof(t1ldtable) / sizeof(t1ldtable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equall(
			    powl(t1ldtable[i].x, t1ldtable[i].y),
				 t1ldtable[i].z));
	}
#endif
}

/*
 * Test case 2 -- Domain error
 *
 * For finite values of x < 0, and finite non-integer values of y,
 * a domain error shall occur and either a NaN (if representable),
 * or an implementation-defined value shall be returned.
 */
ATF_TC(test_pow2);
ATF_TC_HEAD(test_pow2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check domain error");
}
ATF_TC_BODY(test_pow2, tc)
{
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		do {
			fx = random_float(FP_NORMAL);
			fy = random_float(FP_NORMAL);
		} while (fx >= 0 || floorf(fy) == fy);
#ifdef	NAN
		ATF_PASS_OR_BREAK(isnan(powf(fx, fy)));
#endif
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		do {
			dx = random_double(FP_NORMAL);
			dy = random_double(FP_NORMAL);
		} while (dx >= 0 || floor(dy) == dy);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(pow(dx, dy)));
#endif
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_POWL
		errno = 0;
		clear_exceptions();
		do {
			ldx = random_long_double(FP_NORMAL);
			ldy = random_long_double(FP_NORMAL);
		} while (ldx >= 0 || floorl(ldy) == ldy);
#ifdef  NAN
		ATF_PASS_OR_BREAK(isnan(powl(ldx, ldy)));
#endif
		ATF_PASS_OR_BREAK(iserrno_equalto(EDOM));
		ATF_PASS_OR_BREAK(raised_exceptions(MY_FE_INVALID));
#endif
	}
}

/*
 * Test case 3 -- Overflow
 *
 * If the correct value would cause overflow, a range error shall occur and
 * pow(), powf(), and powl() shall return +-HUGE_VAL, +-HUGE_VALF, and
 * +-HUGE_VALL, respectively, with the same sign as the correct value of the
 * function.
 */
ATF_TC(test_pow3);
ATF_TC_HEAD(test_pow3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check overflow");
}
ATF_TC_BODY(test_pow3, tc)
{
	/* float */
#if defined(FLT_MAX) && defined(HUGE_VALF)
	errno = 0;
	clear_exceptions();
	ATF_CHECK(powf(FLT_MAX, FLT_MAX) == HUGE_VALF);
	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
#endif

	/* double */
#if defined(DBL_MAX) && defined(HUGE_VAL)
	errno = 0;
	clear_exceptions();
	ATF_CHECK(pow(DBL_MAX, DBL_MAX) == HUGE_VAL);
	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
#endif

	/* long double */
#if defined(HAVE_POWL) && defined(LDBL_MAX) && defined(HUG_VALL)
	errno = 0;
	clear_exceptions();
	ATF_CHECK(powl(LDBL_MAX, LDBL_MAX) == HUGE_VALL);
	ATF_CHECK(iserrno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
#endif
}

/*******************************************************************************
 *				Test case(s) 4
 ******************************************************************************/
static const struct
t4entry {
	long double x;
	long double y;
} t4table[] = {
#ifdef  INFINITY
	{  INFINITY,  INFINITY },
#endif
#ifdef  HUGE_VAL
	{  HUGE_VAL,  HUGE_VAL },
#endif
#ifdef  HUGE_VALF
	{  HUGE_VALF,  HUGE_VALF },
#endif
#ifdef  HUGE_VALL
	{  HUGE_VALL,  HUGE_VALL },
#endif
};

static const size_t t4tablesize =
    sizeof(t4table) / sizeof(t4table[0]);

/* Test case 4-1 */
ATF_TC(test_pow41);
ATF_TC_HEAD(test_pow41, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If x is -1, and y is +-Inf, 1.0 shall be returned");
}
ATF_TC_BODY(test_pow41, tc)
{
	size_t i;

	/* Only because it's the first test case to use it */
	ATF_REQUIRE(t4tablesize > 0);

	for (i = 0; i < t4tablesize; i++) {
		/* float */
		ATF_CHECK(powf(-1.0,  (float)t4table[i].y) == 1.0);
		ATF_CHECK(powf(-1.0, -(float)t4table[i].y) == 1.0);

		/* double */
		ATF_CHECK(pow(-1.0,  (double)t4table[i].y) == 1.0);
		ATF_CHECK(pow(-1.0, -(double)t4table[i].y) == 1.0);

		/* long double */
#ifdef	HAVE_POWL
		ATF_CHECK(powl(-1.0,  t4table[i].y) == 1.0);
		ATF_CHECK(powl(-1.0, -t4table[i].y) == 1.0);
#endif
	}
}

/* Test case 4-2 */
ATF_TC(test_pow42);
ATF_TC_HEAD(test_pow42, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "For |x| < 1, if y is -Inf, +Inf shall be returned");
}
ATF_TC_BODY(test_pow42, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;
	size_t j;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		for (j = 0; j < t4tablesize; j++) {
			/* float */
			do {
				fx = random_float(FP_NORMAL);
			} while (fx <= -1.0 || fx >= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equalf(
				    powf(fx, -(float)t4table[j].y),
					      (float)t4table[j].y));

			/* double */
			do {
				dx = random_long_double(FP_NORMAL);
			} while (dx <= -1.0 || dx >= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equal(
				    pow(dx, -(double)t4table[j].y),
					     (double)t4table[j].y));

			/* long double */
#ifdef	HAVE_POWL
			do {
				ldx = random_long_double(FP_NORMAL);
			} while (ldx <= -1.0 || ldx >= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equall(
				    powl(ldx, -t4table[j].y),
					       t4table[j].y));
#endif
		}
	}
}

/* Test case 4-3 */
ATF_TC(test_pow43);
ATF_TC_HEAD(test_pow43, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "For |x| > 1, if y is -Inf, +0 shall be returned");
}
ATF_TC_BODY(test_pow43, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;
	size_t j;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		for (j = 0; j < t4tablesize; j++) {
			/* float */
			do {
				fx = random_float(FP_NORMAL);
			} while (-1.0 <= fx && fx <= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equalf(
				    powf(fx, -(float)t4table[j].y),
				    0.0));

			/* double */
			do {
				dx = random_double(FP_NORMAL);
			} while (-1.0 <= dx && dx <= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equal(
				    pow(dx, -(double)t4table[j].y),
				    0.0));

			/* long double */
#ifdef  HAVE_POWL
			do {
				ldx = random_long_double(FP_NORMAL);
			} while (-1.0 <= ldx && ldx <= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equall(
				    powl(ldx, -t4table[j].y),
				    0.0));
#endif
		}
	}
}

/* Test case 4-4 */
ATF_TC(test_pow44);
ATF_TC_HEAD(test_pow44, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "For |x| < 1, if y is +Inf, +0 shall be returned");
}
ATF_TC_BODY(test_pow44, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;
	size_t j;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		for (j = 0; j < t4tablesize; j++) {
			/* float */
			do {
				fx = random_float(FP_NORMAL);
			} while (fx <= -1.0 || fx >= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equalf(
				    powf(fx, (float)t4table[j].y),
				    0.0));

			/* double */
			do {
				dx = random_long_double(FP_NORMAL);
			} while (dx <= -1.0 || dx >= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equal(
				    pow(dx, (double)t4table[j].y),
				    0.0));

			/* long double */
#ifdef  HAVE_POWL
			do {
				ldx = random_long_double(FP_NORMAL);
			} while (ldx <= -1.0 || ldx >= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equall(
				    powl(ldx, t4table[j].y),
				    0.0));
#endif
		}
	}
}

/* Test case 4-5 */
ATF_TC(test_pow45);
ATF_TC_HEAD(test_pow45, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "For |x| > 1, if y is +Inf, +Inf shall be returned");

}
ATF_TC_BODY(test_pow45, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;
	size_t j;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		for (j = 0; j < t4tablesize; j++) {
			/* float */
			do {
				fx = random_float(FP_NORMAL);
			} while (-1.0 <= fx && fx <= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equalf(
				    powf(fx, (float)t4table[j].y),
					     (float)t4table[j].y));

			/* double */
			do {
				dx = random_double(FP_NORMAL);
			} while (-1.0 <= dx && dx <= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equal(
				    pow(dx, (double)t4table[j].y),
					    (double)t4table[j].y));

			/* long double */
#ifdef  HAVE_POWL
			do {
				ldx = random_long_double(FP_NORMAL);
			} while (-1.0 <= ldx && ldx <= 1.0);
			ATF_PASS_OR_BREAK(fpcmp_equall(
				    powl(ldx, t4table[j].y),
					      t4table[j].y));
#endif
		}
	}
}

/* Test case 4-6 */
ATF_TC(test_pow46);
ATF_TC_HEAD(test_pow46, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "For y an odd integer < 0, if x is -Inf, -0 shall be returned");
}
ATF_TC_BODY(test_pow46, tc)
{
	long i, N, y;
	size_t j;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		for (j = 0; j < t4tablesize; j++) {
			do {
				y = rand();
			} while (y % 2 == 0);
			y = -y;

			/* float */
			ATF_PASS_OR_BREAK(fpcmp_equalf(
				    powf(-(float)t4table[j].x, y),
					-0.0));

			/* double */
			ATF_PASS_OR_BREAK(fpcmp_equal(
				    pow(-(double)t4table[j].x, y),
					-0.0));

			/* long double */
#ifdef  HAVE_POWL
			ATF_PASS_OR_BREAK(fpcmp_equall(
				    powl(t4table[j].x, y),
					-0.0));
#endif
		}
	}
}

/* Test case 4-7 */
ATF_TC(test_pow47);
ATF_TC_HEAD(test_pow47, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "For y < 0 and not an odd integer, if x is -Inf, "
	    "+0 shall be returned");
}
ATF_TC_BODY(test_pow47, tc)
{
	float fy;
	double dy;
	long double ldy;
	long i, N;
	size_t j;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		for (j = 0; j < t4tablesize; j++) {
			/* float */
			do {
				fy = random_float(FP_NORMAL);
			} while (fy > 0.0 || (floorf(fy) == fy && ((long)fy %  2)));
			ATF_PASS_OR_BREAK(fpcmp_equalf(
				    powf(-(float)t4table[j].y, fy),
					 0.0));

			/* double */
			do {
				dy = random_float(FP_NORMAL);
			} while (dy > 0.0 || (floor(dy) == dy && ((long)dy %  2)));
			ATF_PASS_OR_BREAK(fpcmp_equal(
				    pow(-(double)t4table[j].y, dy),
					0.0));

			/* long double */
#ifdef  HAVE_POWL
			do {
				ldy = random_float(FP_NORMAL);
			} while (ldy > 0.0 || (floorl(ldy) == ldy && ((long)ldy %  2)));
			ATF_PASS_OR_BREAK(fpcmp_equall(
				    powf(-t4table[j].y, ldy),
					 0.0));
#endif
		}
	}
}

/* Test case 4-8 */
ATF_TC(test_pow48);
ATF_TC_HEAD(test_pow48, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "For y an odd integer > 0, if x is -Inf, -Inf shall be returned");
}
ATF_TC_BODY(test_pow48, tc)
{
	long i, N, y;
	size_t j;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		for (j = 0; j < t4tablesize; j++) {
			do {
				y = rand();
			} while (y % 2 == 0);

			/* float */
			ATF_PASS_OR_BREAK(fpcmp_equalf(
				    powf(-(float)t4table[j].x, y),
					 -(float)t4table[j].x));

			/* double */
			ATF_PASS_OR_BREAK(fpcmp_equal(
				    pow(-(double)t4table[j].x, y),
					-(double)t4table[j].x));

			/* long double */
#ifdef  HAVE_POWL
			ATF_PASS_OR_BREAK(fpcmp_equall(
				    powl(-t4table[j].x, y),
					 -t4table[j].x));
#endif
		}
	}
}

/* Test case 4-9 */
ATF_TC(test_pow49);
ATF_TC_HEAD(test_pow49, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "For y > 0 and not an odd integer, if x is -Inf, "
	    "+Inf shall be returned");
}
ATF_TC_BODY(test_pow49, tc)
{
	float fy;
	double dy;
	long double ldy;
	long i, N;
	size_t j;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		for (j = 0; j < t4tablesize; j++) {
			/* float */
			do {
				fy = random_float(FP_NORMAL);
			} while (fy <= 0.0 || (floorf(fy) == fy && ((long)fy %  2)));
			ATF_PASS_OR_BREAK(fpcmp_equalf(
				    powf(-(float)t4table[j].x, fy),
					  (float)t4table[j].x));

			/* double */
			do {
				dy = random_float(FP_NORMAL);
			} while (dy <= 0.0 || (floor(dy) == dy && ((long)dy %  2)));
			ATF_PASS_OR_BREAK(fpcmp_equal(
				    pow(-(double)t4table[j].x, dy),
					 (double)t4table[j].x));

			/* long double */
#ifdef  HAVE_POWL
			do {
				ldy = random_float(FP_NORMAL);
			} while (ldy > 0.0 || (floorl(ldy) == ldy && ((long)ldy %  2)));
			ATF_PASS_OR_BREAK(fpcmp_equall(
				    powl(-t4table[j].x, ldy),
					  t4table[j].x));
#endif
		}
	}
}

/*
	For y > 0 and not an odd integer, if x is -Inf, +Inf shall be returned.
	For y < 0, if x is +Inf, +0 shall be returned.
	For y > 0, if x is +Inf, +Inf shall be returned.
*/

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_pow1);
	ATF_TP_ADD_TC(tp, test_pow2);
	ATF_TP_ADD_TC(tp, test_pow3);
	ATF_TP_ADD_TC(tp, test_pow41);
	ATF_TP_ADD_TC(tp, test_pow42);
	ATF_TP_ADD_TC(tp, test_pow43);
	ATF_TP_ADD_TC(tp, test_pow44);
	ATF_TP_ADD_TC(tp, test_pow45);
	ATF_TP_ADD_TC(tp, test_pow46);
	ATF_TP_ADD_TC(tp, test_pow47);
	ATF_TP_ADD_TC(tp, test_pow48);
	ATF_TP_ADD_TC(tp, test_pow49);

	return atf_no_error();
}
