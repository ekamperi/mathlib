#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>	/* {DBL, FLT, LDBL}_EPSILON */
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
#define	NAN_IF_AVAIL 0
#define TYPE_DBL  1
#define TYPE_FLT  2
#define TYPE_LDBL 3

struct t3entry {
	int type;
	long double x;
	long double y;
	int error;
	int except;
} t3table[] = {
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
	{ TYPE_LDBL,  1.0, -HUGE_VALL, ERANGE, FE_INVALID },
	{ TYPE_LDBL, -1.0,  HUGE_VALL, ERANGE, FE_INVALID},
#endif

	/*
	 * For finite |x|>1, a domain error shall occur, and either a NaN
	 * (if supported), or an implementation-defined value shall be returned.
	 */
	{ TYPE_DBL,   1.0 +  DBL_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_FLT,   1.0 +  FLT_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_LDBL,  1.0 + LDBL_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },

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
	{ TYPE_LDBL, -INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },
#endif
#ifdef  HUGE_VAL
	{ TYPE_DBL,  HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_DBL, -HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_FLT,  HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_FLT, -HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_LDBL,  HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_LDBL, -HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
#endif
#ifdef  HUGE_VALF
	{ TYPE_DBL,  HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_DBL, -HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_FLT,  HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_FLT, -HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_LDBL,  HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_LDBL, -HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },
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

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++) {
		/* Initialize error handling */
		clear_exceptions();
		clear_errno();

		/* Override NAN if avalaible */
#ifdef	NAN
		t3table[i].y = NAN;
#endif

		switch(t3table[i].type) {
		case TYPE_DBL:
			ldy = atanh((double)t3table[i].x);
			break;
		case TYPE_FLT:
			ldy = atanh((float)t3table[i].x);
			break;
		case TYPE_LDBL:
			ldy = atanh(t3table[i].x);
			break;
		}

		/* Check return value */
		if (t3table[i].y != NAN_IF_AVAIL) {
			printf("%f       %f   %f\n", t3table[i].x, ldy, t3table[i].y);
			ATF_CHECK(fpcmp_equal(ldy, t3table[i].y));
		}

		/* Check errno */
		ATF_CHECK(iserrno_equalto(t3table[i].error));

		/* Check floating-point exception */
		ATF_CHECK(raised_exceptions(t3table[i].except));
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
