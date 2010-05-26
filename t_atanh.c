#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "subr_errhandling.h"
#include "subr_fpcmp.h"

struct tentry {
	double x;       /* Input */
	double y;       /* atanh output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_atanh1);
ATF_TC_HEAD(test_atanh1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, atanh(x)) pairs");
}
ATF_TC_BODY(test_atanh1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equal(atanh(ttable[i].x), ttable[i].y));
	}
}

/*
 * Test case 2 -- Edge cases
 */
ATF_TC(test_atanh2);
ATF_TC_HEAD(test_atanh2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_atanh2, tc)
{
	/* If x is NaN, a NaN shall be returned */
#ifdef  NAN
	ATF_CHECK(fpcmp_equal(atanh(NAN), NAN));
#endif

	/* If x is +-0, x shall be returned */
	ATF_CHECK(fpcmp_equal(atanh( 0.0),  0.0));
	ATF_CHECK(fpcmp_equal(atanh(-0.0), -0.0));
}

/*
 * Test case 3 -- Errors
 */
#define	NAN_IF_AVAIL

struct tentry {
	int type;
	long double x;
	long double y;
	int error;
	int except;
} ttable[] = {
	/*
	 * If x is +-1, a pole error shall occur, and atanh(), atanhf(), and
	 * atanhl() shall return the value of the macro HUGE_VAL, HUGE_VALF,
	 * and HUGE_VALL, respectively, with the same sign as the correct
	 * value of the function.
	 */
#ifdef	HUGE_VAL
	{ TYPE_DBL,  1.0,  HUGE_VAL, ERANGE, FE_INVALID },
	{ TYPE_DBL, -1.0, -HUGE_VAL, ERANGE, FE_INVALID },
#endif
#ifdef	HUGE_VALF
	{ TYPE_FLT,  1.0,  HUGE_VALF, ERANGE, FE_INVALID },
	{ TYPE_FLT, -1.0, -HUGE_VALF, ERANGE, FE_INVALID },
#endif
#ifdef	HUGE_VALL
	{ TYPE_LDBL,  1.0, -HUGE_VALL, ERANGE, HUGE_VALL },
	{ TYPE_LDBL, -1.0,  HUGE_VALL, ERANGE, HUGE_VALL },
#endif

	/*
	 * For finite |x|>1, a domain error shall occur, and either a NaN
	 * (if supported), or an implementation-defined value shall be returned.
	 */
	{ TYPE_DBL,   1.0 +   DBL_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_FLT,   1.0 +   FLT_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_LDBL,  1.0 + LDLBL_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	/*
	 * If x is +-Inf, a domain error shall occur, and either a NaN
	 * (if supported), or an implementation-defined value shall be returned.
	 */
#ifdef	INFINITY
	{ TYPE_DBL,  INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_DBL, -INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_FLT,  INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_FLT, -INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_LDBL,  INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_LDBL, -INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID }
#endif
#ifdef  HUGE_VAL
        { TYPE_DBL,  HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
        { TYPE_DBL, -HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },

        { TYPE_FLT,  HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
        { TYPE_FLT, -HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },

        { TYPE_LDBL,  HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
        { TYPE_LDBL, -HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID }
#endif
#ifdef  HUGE_VALF
        { TYPE_DBL,  HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },
        { TYPE_DBL, -HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },

        { TYPE_FLT,  HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },
        { TYPE_FLT, -HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },

        { TYPE_LDBL,  HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },
        { TYPE_LDBL, -HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID }
#endif
#ifdef  HUGE_VALL
        { TYPE_DBL,  HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
        { TYPE_DBL, -HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID },

        { TYPE_FLT,  HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
        { TYPE_FLT, -HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID },

        { TYPE_LDBL,  HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
        { TYPE_LDBL, -HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID }
#endif
};

ATF_TC(test_atanh3);
ATF_TC_HEAD(test_atanh3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check if errors are generated where due");
}
ATF_TC_BODY(test_atanh3, tc)
{
	long double ldy;
        int haserrexcept;
        int haserrno;
	size_t i, N;

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	N = sizeof(ttable) / sizeof(ttable[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++) {
		/* Initialize error handling */
		clear_exceptions();
		clear_errno();

		switch(ttable[i].type) {
		case TYPE_DBL:
			ldy = atanh((double)ttable[i].x);
			break;
		case TYPE_FLT:
			ldy = atanh((float)ttable[i].x);
			break;
		case TYPE_LDBL:
			ldy = atanh(ttable[i].x);
			break;
		}

		/* Check return value */
		if (ttable[i].y == NAN_IF_AVAIL) {
#ifdef	NAN
			ATF_CHECK(isnan(ldy));
#endif
		} else {
			ATF_CHECK(fmpcmp_equal(ldy, ttable[i].y));
		}

		/* Check errno */
		ATF_CHECK(iserrno_equalto(ttable[i].error));

		/* Check floating-point exception */
		ATF_CHECK(raised_exceptions(FE_INVALID));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_atanh1);
	ATF_TP_ADD_TC(tp, test_atanh2);
	ATF_TP_ADD_TC(tp, test_atanh3);

	return atf_no_error();
}
