#include <atf-c.h>
#include <errno.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1 -- Basic functionality
 */
static const struct
t1entry {
	long double x;	/* Input */
	long double y;	/* Input */
	long double z;	/* Input */
	long double w;	/* fma output */
} t1table[] = {
};

ATF_TC(test_fma1);
ATF_TC_HEAD(test_fma1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, fma(x)) pairs");
}
ATF_TC_BODY(test_fma1, tc)
{
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++) {
	}
}

/*
 * Test case 2
 *
 * If x or y are NaN, a NaN shall be returned
 */
ATF_TC(test_fma2);
ATF_TC_HEAD(test_fma2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check what happens when x or y are NaN");
}
ATF_TC_BODY(test_fma2, tc)
{
#ifdef	NAN
	float fx, fz;
	double dx, dz;
	long double ldx, ldz;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		fz = random_float(FP_NORMAL);
		ATF_PASS_OR_BREAK(isnan(fmaf(NAN, fx, fz)));
		ATF_PASS_OR_BREAK(isnan(fmaf(fx, NAN, fz)));

		/* double */
		dx = random_double(FP_NORMAL);
		dz = random_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(isnan(fma(NAN, dx, dz)));
		ATF_PASS_OR_BREAK(isnan(fma(dx, NAN, dz)));

		/* long double */
		ldx = random_long_double(FP_NORMAL);
		ldz = random_long_double(FP_NORMAL);
		ATF_PASS_OR_BREAK(isnan(fmal(NAN, ldx, ldz)));
		ATF_PASS_OR_BREAK(isnan(fmal(ldx, NAN, ldz)));
	}
#endif
}

/*
 * Test case 3
 *
 * If x multiplied by y is an exact infinity and z is also an infinity but
 * with the opposite sign, a domain error shall occur, and either a NaN (if
 * supported), or an implementation-defined value shall be returned.
 */
static const long double
t3table[] =
{
#ifdef	INFINITY
	INFINITY,
#endif
#ifdef	HUGE_VAL
	HUGE_VAL,
#endif
#ifdef	HUGE_VALF
	HUGE_VALF,
#endif
#ifdef	HUGE_VALL
	HUGE_VALL
#endif
};

ATF_TC(test_fma3);
ATF_TC_HEAD(test_fma3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check what happens when x*y=+/-Inf, z=-/+Inf");
}
ATF_TC_BODY(test_fma3, tc)
{
	float fx, fy, fw;
	double dx, dy, dw;
	long double ldx, ldy, ldw;
	size_t i, N;

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		fy = (float)t3table[i];

		errno = 0;
		clear_exceptions();
		fw = fmaf(fx, fy, signbit(fy) ? fy : -fy);
		ATF_CHECK_IFNAN(fw);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		dx = random_double(FP_NORMAL);
		dy = (double)t3table[i];

		errno = 0;
		clear_exceptions();
		dw = fma(dx, dy, signbit(dy) ? dy : -dy);
		ATF_CHECK_IFNAN(dw);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_FMAL
		ldx = random_long_double(FP_NORMAL);
		ldy = t3table[i];

		errno = 0;
		clear_exceptions();
		ldw = fmal(ldx, ldy, signbit(ldy) ? ldy : -ldy);
		ATF_CHECK_IFNAN(ldw);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
	}
}

/*
 * Test case 4
 *
 * If one of x and y is infinite, the other is zero, and z is _not_ a NaN,
 * a domain error shall occur, and either a NaN (if supported), or an
 * implementation-defined value shall be returned.
 */
static const struct
t4entry {
	long double x;
	long double y;
} t4table[] =
{
#ifdef  INFINITY
	{ INFINITY,  0.0 },
	{ INFINITY, -0.0 },
	{  0.0, INFINITY },
	{ -0.0, INFINITY },
#endif
#ifdef  HUGE_VAL
	{ HUGE_VAL,  0.0 },
	{ HUGE_VAL, -0.0 },
	{  0.0, HUGE_VAL },
	{ -0.0, HUGE_VAL },
#endif
#ifdef  HUGE_VALF
	{ HUGE_VALF,  0.0 },
	{ HUGE_VALF, -0.0 },
	{  0.0, HUGE_VALF },
	{ -0.0, HUGE_VALF },
#endif
#ifdef  HUGE_VALL
	{ HUGE_VALL,  0.0 },
	{ HUGE_VALL, -0.0 },
	{  0.0, HUGE_VALL },
	{ -0.0, HUGE_VALL }
#endif
};

ATF_TC(test_fma4);
ATF_TC_HEAD(test_fma4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check what happens when x=Inf/0, y=0/Inf, z=not NAN");
}
ATF_TC_BODY(test_fma4, tc)
{
	float fw;
	double dw;
	long double ldw;
	size_t i, N;

	N = sizeof(t4table) / sizeof(t4table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		fw = fmaf((float)t4table[i].x,
			  (float)t4table[i].y,
			  random_float(FP_NORMAL));
		ATF_CHECK_IFNAN(fw);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* double */
		errno = 0;
		clear_exceptions();
		dw = fma((double)t4table[i].x,
			 (double)t4table[i].y,
			 random_double(FP_NORMAL));
		ATF_CHECK_IFNAN(dw);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));

		/* long double */
#ifdef	HAVE_FMAL
		errno = 0;
		clear_exceptions();
		ldw = fmal(t4table[i].x,
			   t4table[i].y,
			   random_long_double(FP_NORMAL));
		ATF_CHECK_IFNAN(ldw);
		ATF_CHECK(errno_equalto(EDOM));
		ATF_CHECK(raised_exceptions(MY_FE_INVALID));
#endif
	}
}

/*
 * Test case 5
 *
 * If one of x and y is infinite, the other is zero, and z is a NaN,
 * a NaN shall be returned and a domain error may occur.
 */
ATF_TC(test_fma5);
ATF_TC_HEAD(test_fma5, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check what happens when x=Inf/0, y=0/Inf, z=NAN");
}
ATF_TC_BODY(test_fma5, tc)
{
#ifdef	NAN
	float fw;
	double dw;
	size_t i, N;

	/* We are reusing the t4table[] of the 4th test case */
	N = sizeof(t4table) / sizeof(t4table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		errno = 0;
		clear_exceptions();
		fw = fmaf((float)t4table[i].x, (float)t4table[i].y, NAN);
		ATF_CHECK(isnan(fw));
		/* domain error is optional */
		if (errno) {
			ATF_CHECK(errno_equalto(EDOM));
		}
		if (raised_exceptions(MY_FE_ALL_EXCEPT)) {
			ATF_CHECK(raised_exceptions(MY_FE_INVALID));
		}

		/* double */
		errno = 0;
		clear_exceptions();
		dw = fma((double)t4table[i].x, (double)t4table[i].y, NAN);
		ATF_CHECK(isnan(dw));
		/* domain error is optional */
		if (errno) {
			ATF_CHECK(errno_equalto(EDOM));
		}
		if (raised_exceptions(MY_FE_ALL_EXCEPT)) {
			ATF_CHECK(raised_exceptions(MY_FE_INVALID));
		}

		/* long double */
#ifdef	HAVE_FMAL
		errno = 0;
		clear_exceptions();
		dw = fmal(t4table[i].x, t4table[i].y, NAN);
		ATF_CHECK(isnan(dw));
		/* domain error is optional */
		if (errno) {
			ATF_CHECK(errno_equalto(EDOM));
		}
		if (raised_exceptions(MY_FE_ALL_EXCEPT)) {
			ATF_CHECK(raised_exceptions(MY_FE_INVALID));
		}
#endif	/* HAVE_FMAL */
	}
#endif
}

/*
 * Test case 6
 *
 * If x*y is not 0*Inf nor Inf*0 and z is a NaN, a NaN shall be returned.
 */
ATF_TC(test_fma6);
ATF_TC_HEAD(test_fma6, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check what happens when x*y != 0*Inf nor Inf*0 and z=NAN");
}
ATF_TC_BODY(test_fma6, tc)
{
#ifdef	NAN
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fx = random_float(FP_NORMAL);
		fy = random_float(FP_NORMAL);
		ATF_CHECK(isnan(fmaf(fx, fy, NAN)));

		/* double */
		dx = random_double(FP_NORMAL);
		dy = random_double(FP_NORMAL);
		ATF_CHECK(isnan(fma(dx, dy, NAN)));

		/* long double */
#ifdef	HAVE_FMAL
		ldx = random_long_double(FP_NORMAL);
		ldy = random_long_double(FP_NORMAL);
		ATF_CHECK(isnan(fmal(ldx, ldy, NAN)));
#endif	/* HAVE_FMAL */
	}
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fma1);
	ATF_TP_ADD_TC(tp, test_fma2);
	ATF_TP_ADD_TC(tp, test_fma3);
	ATF_TP_ADD_TC(tp, test_fma4);
	ATF_TP_ADD_TC(tp, test_fma5);
	ATF_TP_ADD_TC(tp, test_fma6);

	return atf_no_error();
}
