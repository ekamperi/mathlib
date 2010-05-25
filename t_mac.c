#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "subr_errhandling.h"
#include "subr_random.h"

/*
 * Test case 1 -- fpclassify()
 */
ATF_TC(test_fpclassify);
ATF_TC_HEAD(test_fpclassify, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "fpclassify");
}
ATF_TC_BODY(test_fpclassify, tc)
{
	/* FP_INFINITE */
	ATF_CHECK(fpclassify(HUGE_VALF) == FP_INFINITE);
	ATF_CHECK(fpclassify((float)HUGE_VAL) == FP_INFINITE);
	ATF_CHECK(fpclassify((float)HUGE_VALL) == FP_INFINITE);

	ATF_CHECK(fpclassify(HUGE_VAL) == FP_INFINITE);
	ATF_CHECK(fpclassify((double)HUGE_VALF) == FP_INFINITE);
	ATF_CHECK(fpclassify((double)HUGE_VALL) == FP_INFINITE);

	ATF_CHECK(fpclassify(HUGE_VALL) == FP_INFINITE);
	ATF_CHECK(fpclassify((float)HUGE_VALL) == FP_INFINITE);
	ATF_CHECK(fpclassify((double)HUGE_VALL) == FP_INFINITE);


	/* FP_NAN */
#ifdef	NAN
	ATF_CHECK(fpclassify(NAN) == FP_NAN);
	ATF_CHECK(fpclassify((double)NAN) == FP_NAN);
	ATF_CHECK(fpclassify((long double)NAN) == FP_NAN);
#endif


	/* FP_NORMAL */
	ATF_CHECK(fpclassify((float)1) == FP_NORMAL);
	ATF_CHECK(fpclassify((double)1) == FP_NORMAL);
	ATF_CHECK(fpclassify((long double)1) == FP_NORMAL);


	/* FP_SUBNORMAL */


	/* FP_ZERO */
	ATF_CHECK(fpclassify((float)0.0) == FP_ZERO);
	ATF_CHECK(fpclassify((float)-0.0) == FP_ZERO);

	ATF_CHECK(fpclassify((double)0.0) == FP_ZERO);
	ATF_CHECK(fpclassify((double)-0.0) == FP_ZERO);

	ATF_CHECK(fpclassify((long double)0.0) == FP_ZERO);
	ATF_CHECK(fpclassify((long double)-0.0) == FP_ZERO);
}

/*
 * Test case 2 -- isfinite()
 */
ATF_TC(test_isfinite);
ATF_TC_HEAD(test_isfinite, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "isfinite()");
}
ATF_TC_BODY(test_isfinite, tc)
{
	ATF_CHECK(!isfinite(HUGE_VALF));
	ATF_CHECK(!isfinite(HUGE_VAL));
	ATF_CHECK(!isfinite(HUGE_VALL));
#ifdef	INFINITY
	ATF_CHECK(!isfinite(INFINITY));
#endif
}

/*
 * Test case 3 -- isinf()
 */
ATF_TC(test_isinf);
ATF_TC_HEAD(test_isinf, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "isinf()");
}
ATF_TC_BODY(test_isinf, tc)
{
	ATF_CHECK(isinf(HUGE_VALF));
	ATF_CHECK(isinf(HUGE_VAL));
	ATF_CHECK(isinf(HUGE_VALL));
#ifdef  INFINITY
	ATF_CHECK(isinf(INFINITY));
#endif
}

/*
 * Test case 4 -- isnan()
 */
ATF_TC(test_isnan);
ATF_TC_HEAD(test_isnan, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "isnan()");
}
ATF_TC_BODY(test_isnan, tc)
{
#ifdef	NAN
	ATF_CHECK(isnan(NAN));
#endif
}

/*
 * Test case 5 -- isnormal()
 */
ATF_TC(test_isnormal);
ATF_TC_HEAD(test_isnormal, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "isnormal()");
}
ATF_TC_BODY(test_isnormal, tc)
{
	size_t i;
	float fx;
	double dy;
	long double ldz;

#define	NITERATIONS 10000000
	for (i = 0; i < NITERATIONS; i++) {
		fx = random_float(FP_NORMAL);
		ATF_CHECK(isnormal(fx));

		dy = random_double(FP_NORMAL);
		ATF_CHECK(isnormal(dy));

		ldz = random_long_double(FP_NORMAL);
		ATF_CHECK(isnormal(ldz));
	}
}

/*
 * Test case 6 -- signbit()
 */
ATF_TC(test_signbit);
ATF_TC_HEAD(test_signbit, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "signbit()");
}
ATF_TC_BODY(test_signbit, tc)
{
	size_t i;
	float fx;
	double dy;
	long double ldz;

	for (i = 0; i < 1000000; i++) {
		/* Float */
		do {
			fx = random_float(FP_NORMAL);
		} while (fx < 0.0);
		ATF_CHECK(signbit(fx) == 0);

		do {
			fx = random_float(FP_NORMAL);
		} while (fx > 0.0);
		ATF_CHECK(signbit(fx) != 0);

		/* Double */
		do {
			dy = random_double(FP_NORMAL);
		} while (dy < 0.0);
		ATF_CHECK(signbit(dy) == 0);

		do {
			dy = random_double(FP_NORMAL);
		} while (dy > 0.0);
		ATF_CHECK(signbit(dy) != 0);

		/* Long double */
		do {
			ldz = random_long_double(FP_NORMAL);
		} while (ldz < 0.0);
		ATF_CHECK(signbit(ldz) == 0);

		do {
			ldz = random_long_double(FP_NORMAL);
		} while (ldz > 0.0);
		ATF_CHECK(signbit(ldz) != 0);
	}

	/* Also check some special cases */
	ATF_CHECK(signbit(+0.0) == 0);
	ATF_CHECK(signbit(-0.0) != 0);

	ATF_CHECK(signbit(HUGE_VAL) == 0);
	ATF_CHECK(signbit(-HUGE_VAL) != 0);

	ATF_CHECK(signbit(HUGE_VALF) == 0);
	ATF_CHECK(signbit(-HUGE_VALF) != 0);

	ATF_CHECK(signbit(HUGE_VALL) == 0);
	ATF_CHECK(signbit(-HUGE_VALL) != 0);

	/* XXX: Should we require signbit(INFINITY) == 0 ? */
}

/*
 * Test case 7 -- isgreater()
 */
ATF_TC(test_isgreater);
ATF_TC_HEAD(test_isgreater, tc)
{
  atf_tc_set_md_var(tc,
		    "descr",
		    "isgreater()");
}
ATF_TC_BODY(test_isgreater, tc)
{
	size_t i;
	float fx, fy;
	double dx, dy;
	long double ldx, ldy;

	for (i = 0; i < 100000; i++) {
		/* Float */
		fx = random_float(FP_NORMAL);
		fy = random_float(FP_NORMAL);
		if (fx > fy) {
			ATF_CHECK(isgreater(fx, fy));
			ATF_CHECK(isless(fy, fx));

			ATF_CHECK(islessgreater(fx, fy));
			ATF_CHECK(islessgreater(fy, fx));
		} else if (fx < fy) {
			ATF_CHECK(isgreater(fy, fx));
			ATF_CHECK(isless(fx, fy));

			ATF_CHECK(islessgreater(fx, fy));
			ATF_CHECK(islessgreater(fy, fx));
		} else {
			ATF_CHECK(!islessgreater(fx, fy));
			ATF_CHECK(!islessgreater(fy, fx));
		}

		/* Double */
		dx = random_double(FP_NORMAL);
		dy = random_double(FP_NORMAL);
		if (dx > dy) {
			ATF_CHECK(isgreater(dx, dy));
			ATF_CHECK(isless(dy, dx));

			ATF_CHECK(islessgreater(dx, dy));
			ATF_CHECK(islessgreater(dy, dx));
		} else if (dx < dy) {
			ATF_CHECK(isgreater(dy, dx));
			ATF_CHECK(isless(dx, dy));

			ATF_CHECK(islessgreater(dx, dy));
			ATF_CHECK(islessgreater(dy, dx));
		} else {
			ATF_CHECK(!islessgreater(fx, fy));
			ATF_CHECK(!islessgreater(fy, fx));
		}

		/* Long double */
		ldx = random_long_double(FP_NORMAL);
		ldy = random_long_double(FP_NORMAL);
		if (ldx > ldy) {
			ATF_CHECK(isgreater(ldx, ldy));
			ATF_CHECK(isless(ldy, ldx));

			ATF_CHECK(islessgreater(ldx, ldy));
			ATF_CHECK(islessgreater(ldy, ldx));
		} else if (ldx < ldy) {
			ATF_CHECK(isgreater(ldy, ldx));
			ATF_CHECK(isless(ldx, ldy));

			ATF_CHECK(islessgreater(ldx, ldy));
			ATF_CHECK(islessgreater(ldy, ldx));
		} else {
			ATF_CHECK(!islessgreater(fx, fy));
			ATF_CHECK(!islessgreater(fy, fx));
		}
	}

	/*
	 * If x or y is NaN, 0 shall be returned
	 *
	 * Also, unlike (x) > (y), isgreater(x, y) shall not raise the invalid
	 * floating-point exception when x and y are unordered. So if the host
	 * supports floating point exceptions in libm, check against it.
	 */
	clear_exceptions();

#ifdef	NAN
	/* isgreater() */
	ATF_CHECK(isgreater(NAN, fx) == 0);
	ATF_CHECK(isgreater(fx, NAN) == 0);

	ATF_CHECK(isgreater(NAN, dx) == 0);
	ATF_CHECK(isgreater(dx, NAN) == 0);

	ATF_CHECK(isgreater(NAN, ldx) == 0);
	ATF_CHECK(isgreater(ldx, NAN) == 0);

	/* isless() */
	ATF_CHECK(isless(NAN, fx) == 0);
	ATF_CHECK(isless(fx, NAN) == 0);

	ATF_CHECK(isless(NAN, dx) == 0);
	ATF_CHECK(isless(dx, NAN) == 0);

	ATF_CHECK(isless(NAN, ldx) == 0);
	ATF_CHECK(isless(ldx, NAN) == 0);

	/* islessgreater() */
	ATF_CHECK(islessgreater(NAN, fx) == 0);
	ATF_CHECK(islessgreater(fx, NAN) == 0);

	ATF_CHECK(islessgreater(NAN, dx) == 0);
	ATF_CHECK(islessgreater(dx, NAN) == 0);

	ATF_CHECK(islessgreater(NAN, ldx) == 0);
	ATF_CHECK(islessgreater(ldx, NAN) == 0);

	/* islessequal() */
	ATF_CHECK(islessequal(NAN, fx) == 0);
	ATF_CHECK(islessequal(fx, NAN) == 0);

	ATF_CHECK(islessequal(NAN, dx) == 0);
	ATF_CHECK(islessequal(dx, NAN) == 0);

	ATF_CHECK(islessequal(NAN, ldx) == 0);
	ATF_CHECK(islessequal(ldx, NAN) == 0);

	/* isgreaterequal() */
	ATF_CHECK(isgreaterequal(NAN, fx) == 0);
	ATF_CHECK(isgreaterequal(fx, NAN) == 0);

	ATF_CHECK(isgreaterequal(NAN, dx) == 0);
	ATF_CHECK(isgreaterequal(dx, NAN) == 0);

	ATF_CHECK(isgreaterequal(NAN, ldx) == 0);
	ATF_CHECK(isgreaterequal(ldx, NAN) == 0);
#endif	/* NAN */

	/* XXX: Should we refine it to only check against FE_INVALID ? */
	ATF_CHECK(!raised_exceptions());
}

/*
 * Test case 8 -- isunordered()
 */
ATF_TC(test_isunordered);
ATF_TC_HEAD(test_isunordered, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "isunordered()");
}
ATF_TC_BODY(test_isunordered, tc)
{
	size_t i;
	float fx;
	double dx;
	long double ldx;

	for (i = 0; i < 1000; i++) {
		fx = random_float(FP_NORMAL);
		dx = random_double(FP_NORMAL);
		ldx = random_long_double(FP_NORMAL);

		/* float - */
		ATF_CHECK(!isunordered(fx, fx));
		ATF_CHECK(!isunordered(fx, dx));
		ATF_CHECK(!isunordered(fx, ldx));

		/* double - */
		ATF_CHECK(!isunordered(dx, dx));
		ATF_CHECK(!isunordered(dx, fx));
		ATF_CHECK(!isunordered(dx, ldx));

		/* long double - */
		ATF_CHECK(!isunordered(ldx, ldx));
		ATF_CHECK(!isunordered(ldx, fx));
		ATF_CHECK(!isunordered(ldx, dx));
	}

#ifdef	NAN
	clear_exceptions();

	/* float */
	ATF_CHECK(isunordered(NAN, fx));
	ATF_CHECK(isunordered(fx, NAN));

	/* double */
	ATF_CHECK(isunordered(NAN, dx));
	ATF_CHECK(isunordered(dx, NAN));

	/* long double */
	ATF_CHECK(isunordered(NAN, ldx));
	ATF_CHECK(isunordered(ldx, NAN));

	ATF_CHECK(isunordered(NAN, NAN));

	/* XXX: Should we refine it to only check against FE_INVALID ? */
	ATF_CHECK(!raised_exceptions());
#endif	/* NAN */
}

/* XXX: islessequal(), isgreatequal() etc */

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fpclassify);
	ATF_TP_ADD_TC(tp, test_isfinite);
	ATF_TP_ADD_TC(tp, test_isinf);
	ATF_TP_ADD_TC(tp, test_isnan);
	ATF_TP_ADD_TC(tp, test_isnormal);
	ATF_TP_ADD_TC(tp, test_signbit);
	ATF_TP_ADD_TC(tp, test_isgreater);
	ATF_TP_ADD_TC(tp, test_isunordered);

	return atf_no_error();
}
