#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <float.h>	/* DBL_MIN */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/*
 * Test case 1 -- Basic functionality
 */
struct t1entry {
	long double x;       /* Input */
	long double y;       /* exp output */
} t1table[] = {
	{ -669.86565745420164154359690448573599,
	  1.20515068276875584077262168370669E-291 },
	{ -667.37208077714926248004837538083115,
	  1.4587737581331577677556571239512E-290 },
	{ -665.35410461512442910299085918026279,
	  1.0974477538644934064213794397156E-289 },
	{ -658.40990616960178053650046660798442,
	  1.1381799217695547608032982104839E-286 },
	{ -542.63014267626334388335307917064914,
	  2.1813397894331936983165067230354E-236 },
	{ -413.44185503184253839225986871320975,
	  2.7828113872919225277806532413817E-180 },
	{ -362.58886005184033550581322347614043,
	  3.3857811275961864696881484716445E-158 },
	{ -327.15436368585940401539910686704952,
	  8.2921112604441479123622153306195E-143 },
	{ -313.96313051683952979736487351282978,
	  4.4416557488898838817632615268454E-137 },
	{ -239.52423189976936095805676162864345,
	  9.4612344359177985236345053985301E-105 },
	{ -196.11093126872063636618744757946702,
	  6.7624599971899249039399779582529E-86 },
	{ -181.79958919074766364064214662213645,
	  1.11030322731798892253075505490668E-79 },
	{ -114.22715929006284223850891228792910,
	  2.46476226750148281646439268394618E-50 },
	{ -96.530257859573998039827012936055014,
	  1.19520300088743151560301063276600E-42 },
	{ -80.064120896327856153452522743589485,
	  1.69275496497866305481281473615739E-35 },
	{ -43.978087430419291974882006626175013,
	  7.95351865977073910477048553648174E-20 },
	{ 66.255444516237708379921650348483305,
	  5.94804094502211309672837306589275E28 },
	{ 330.67477338493571742473461151937567,
	  4.0759550477831344578748721222465E143 },
	{ 446.75354903811257543812295107762869,
	  1.05341892892593909811669807823641E194 },
	{ 623.71455204831765145413054367251437,
	  7.5125649087611384231587402640172E270 }
};

ATF_TC(test_exp1);
ATF_TC_HEAD(test_exp1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, exp(x)) pairs");
}
ATF_TC_BODY(test_exp1, tc)
{
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++) {
		printf("%.16e %.16e\n", expf((float)t1table[i].x), (float)t1table[i].y);
		fflush(NULL);
		ATF_CHECK(fpcmp_equal(
			    expf((float)t1table[i].x),
				 (float)t1table[i].y));

		ATF_CHECK(fpcmp_equal(
			    exp((double)t1table[i].x),
				(double)t1table[i].y));

		ATF_CHECK(fpcmp_equal(
			    expl(t1table[i].x),
				 t1table[i].y));
	}
}

/*
 * Test case 2 -- Edge cases
 */
struct t2entry {
	long double x;	/* Input */
	long double y;	/* exp output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, 1 shall be returned */
	{  0.0, 1.0 },
	{ -0.0, 1.0 },

	/* If x is -Inf, +0 shall be returned */
	/* If x is +Inf, x shall be returned */
#ifdef	INFINITY
	{ -INFINITY, 0.0 },
	{  INFINITY, INFINITY },
#endif
#ifdef	HUGE_VAL
	{ -HUGE_VAL, 0.0 },
	{ HUGE_VAL, HUGE_VAL },
#endif
#ifdef	HUGE_VALF
	{ -HUGE_VALF, 0.0 },
	{  HUGE_VALF, HUGE_VALF },
#endif
#ifdef	HUGE_VALL
	{ -HUGE_VALL, 0.0 },
	{  HUGE_VALL, HUGE_VALL }
#endif
};

ATF_TC(test_exp2);
ATF_TC_HEAD(test_exp2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_exp2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		ATF_CHECK(fpcmp_equal(
			    expf((float)t2table[i].x),
			    t2table[i].y));

		ATF_CHECK(fpcmp_equal(
			    exp((double)t2table[i].x),
			    t2table[i].y));

		ATF_CHECK(fpcmp_equal(
			    expl(t2table[i].x),
			    t2table[i].y));
	}
}

ATF_TC(test_exp3);
ATF_TC_HEAD(test_exp3, tc)
{
  atf_tc_set_md_var(tc,
		    "descr",
		    "Check for undexplow");
}
ATF_TC_BODY(test_exp3, tc)
{
	double dx;
	long i, N;
	int haserrexcept;
	int haserrno;

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/*
	 * For IEEE Std 754-1985 double, 709.8 < x implies exp(x) has overflowed.
	 * The value x < -708.4 implies exp(x) has underflowed.
	 */
	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		do {
			dx = random_double(FP_NORMAL);
		} while (dx > -708.4 && dx < 709.8);

		errno = 0;
		clear_exceptions();
		(void)exp(dx);

		ATF_CHECK(iserrno_equalto(ERANGE));
		if (dx < 0.0)
			ATF_CHECK(raised_exceptions(MY_FE_UNDERFLOW));
		else
			ATF_CHECK(raised_exceptions(MY_FE_OVERFLOW));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_exp1);
	ATF_TP_ADD_TC(tp, test_exp2);
	ATF_TP_ADD_TC(tp, test_exp3);

	return atf_no_error();
}
