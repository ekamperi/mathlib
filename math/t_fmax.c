#include <atf-c.h>
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1 -- Basic functionality
 */
static const struct
t1entry {
	double x;       /* Input */
	double y;	/* Input */
	int z;		/* fmax output (0 for x>y, 1 otherwise */
} t1table[] = {
	/* -1 digit */
	{ 1.1234567890123456, 1.1234567890123455, 0 },
	{ 1.1234567890123455, 1.1234567890123456, 1 },

	/* -2 */
	{ 1.1234567890123465, 1.1234567890123456, 0 },
	{ 1.1234567890123456, 1.1234567890123465, 1 },

	/* -3 */
	{ 1.1234567890123654, 1.1234567890123456, 0 },
	{ 1.1234567890123456, 1.1234567890123654, 1 },

	/* -4 */
	{ 1.1234567890126453, 1.1234567890125634, 0 },
};

ATF_TC(test_fmax1);
ATF_TC_HEAD(test_fmax1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good "
	    "(x, y, fmax(x,y)) pairs");
}
ATF_TC_BODY(test_fmax1, tc)
{
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++) {
		if (t1table[i].x == t1table[i].y) {
			atf_tc_fail_nonfatal(
				"Identical value (%.16e) at array position %u",
				    t1table[i].x, i);
		}

		ATF_CHECK(fmax(t1table[i].x, t1table[i].y)
		    == t1table[i].z ? t1table[i].x : t1table[i].y);
		ATF_CHECK(fmin(t1table[i].x, t1table[i].y)
		    == t1table[i].z ? t1table[i].y : t1table[i].x);
	}
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;	/* Input */
	long double y;	/* Input */
	long double z;	/* fmax() output */
} t2table[] = {
#ifdef	NAN
	/*
	 * If just one argument is a NaN, the other argument
	 * shall be returned.
	 */
	{ NAN, +0.0, +0.0 },
	{ NAN, -0.0, -0.0 },

	{ +0.0, NAN, +0.0 },
	{ -0.0, NAN, -0.0 },

#ifdef	INFINITY
	{ NAN, +INFINITY, +INFINITY },
	{ NAN, -INFINITY, -INFINITY },

	{ +INFINITY, NAN, +INFINITY },
	{ -INFINITY, NAN, -INFINITY },
#endif	/* INFINITY */
#ifdef	HUGE_VAL
	{ NAN, +HUGE_VAL, +HUGE_VAL },
	{ NAN, -HUGE_VAL, -HUGE_VAL },

	{ +HUGE_VAL, NAN, +HUGE_VAL },
	{ -HUGE_VAL, NAN, -HUGE_VAL },
#endif  /* HUGE_VAL */
#ifdef  HUGE_VALF
	{ NAN, +HUGE_VALF, +HUGE_VALF },
	{ NAN, -HUGE_VALF, -HUGE_VALF },

	{ +HUGE_VALF, NAN, +HUGE_VALF },
	{ -HUGE_VALF, NAN, -HUGE_VALF },
#endif  /* HUGE_VALF */
#ifdef  HUGE_VALL
	{ NAN, +HUGE_VALL, +HUGE_VALL },
	{ NAN, -HUGE_VALL, -HUGE_VALL },

	{ +HUGE_VALL, NAN, +HUGE_VALL },
	{ -HUGE_VALL, NAN, -HUGE_VALL },
#endif  /* HUGE_VALL */

	/* If x and y are NaN, a NaN shall be returned */
	{ NAN, NAN, NAN }
#endif	/* NAN */
};

ATF_TC(test_fmax2);
ATF_TC_HEAD(test_fmax2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_fmax2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpreal_equalf(
			    fmaxf((float)t2table[i].x,
				  (float)t2table[i].y),
			    (float)t2table[i].z));

		/* double */
		ATF_CHECK(fpreal_equal(
			    fmax((double)t2table[i].x,
				 (double)t2table[i].y),
			    (double)t2table[i].z));

		/* long double */
#ifdef	HAVE_FMAXL
		ATF_CHECK(fpreal_equall(
			    fmaxl(t2table[i].x,
				  t2table[i].y),
			    t2table[i].z));
#endif
	}
}

/*
 * Test case 3 - Some random input
 */
ATF_TC(test_fmax3);
ATF_TC_HEAD(test_fmax3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_fmax3, tc)
{
	double x, y;
	long i, N;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		do {
			x = random_double(FP_NORMAL);
			y = random_double(FP_NORMAL);
		} while (x == y);

		ATF_PASS_OR_BREAK((fmax(x, y) == x) || (fmax(x, y) == y));
		ATF_PASS_OR_BREAK((fmin(x, y) == x) || (fmin(x, y) == y));
		ATF_PASS_OR_BREAK((fmax(x, y) != fmin(x, y)));

		/*
		 * Since we are here, check again that if just one argument is a
		 * NaN, the other argument shall be returned. But this time do
		 * it with some random input and also try with subnormals.
		 */
#ifdef	NAN
		ATF_CHECK(fmax(NAN, x) == x);
		ATF_CHECK(fmax(NAN, y) == y);

		ATF_CHECK(fmax(x, NAN) == x);
		ATF_CHECK(fmax(y, NAN) == y);
#endif
	}
}

/*
 * Test case 4 - Live on the nextafter() edge
 */
ATF_TC(test_fmax4);
ATF_TC_HEAD(test_fmax4, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_fmax4, tc)
{
#ifdef	INFINITY
	float fx, fy, fz;
	double dx, dy, dz;
	long double ldx, ldy, ldz;
	long i, N;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	/* x < y < z */
	ATF_FOR_LOOP(i, N, i++) {
		/* float */
		fy = random_float(FP_NORMAL);
		fx = nextafterf(fy, -INFINITY);
		fz = nextafterf(fy, +INFINITY);
		ATF_PASS_OR_BREAK(fmaxf(fx, fy) == fy);
		ATF_PASS_OR_BREAK(fmaxf(fy, fz) == fz);
		ATF_PASS_OR_BREAK(fminf(fx, fy) == fx);
		ATF_PASS_OR_BREAK(fminf(fy, fz) == fy);

		/* double */
		dy = random_double(FP_NORMAL);
		dx = nextafter(dy, -INFINITY);
		dz = nextafter(dy, +INFINITY);
		ATF_PASS_OR_BREAK(fmax(dx, dy) == dy);
		ATF_PASS_OR_BREAK(fmax(dy, dz) == dz);
		ATF_PASS_OR_BREAK(fmin(dx, dy) == dx);
		ATF_PASS_OR_BREAK(fmin(dy, dz) == dy);

		/* long double */
#if defined(HAVE_FMAXL) && defined(HAVE_FMINL) && defined(HAVE_NEXTAFTERL)
		ldy = random_long_double(FP_NORMAL);
		ldx = nextafterl(ldy, -INFINITY);
		ldz = nextafterl(ldy, +INFINITY);
		ATF_PASS_OR_BREAK(fmaxl(ldx, ldy) == ldy);
		ATF_PASS_OR_BREAK(fmaxl(ldy, ldz) == ldz);
		ATF_PASS_OR_BREAK(fminl(ldx, ldy) == ldx);
		ATF_PASS_OR_BREAK(fminl(ldy, ldz) == ldy);
#endif
	}
#endif	/* INFINITY */
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fmax1);
	ATF_TP_ADD_TC(tp, test_fmax2);
	ATF_TP_ADD_TC(tp, test_fmax3);
	ATF_TP_ADD_TC(tp, test_fmax4);

	return atf_no_error();
}
