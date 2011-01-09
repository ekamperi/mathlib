#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <float.h>	/* DBL_MIN */
#include <math.h>

#include "config.h"
#include "subr_atf.h"
#include "subr_errhandling.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/* Don't be too harsh with symbols */
#ifndef M_PI_2
#define M_PI_2	1.57079632679489661923
#endif

/*
 * Test case 1 -- Basic functionality
 */
static const struct
t1entry {
	long double x;       /* Input */
	long double y;       /* erf output */
} t1table[] = {
	{-3.6429676573671860277333252763566885170,
	 -0.999999742186593521256845384304858556 },
	{-3.5998187484153906557512669848690729575,
	 -0.999999643655543023325143010487631159 },
	{-3.5871572999351670840406524063399347788,
	 -0.999999608420889411388769399163705485 },
	{-3.3454221797526240340098277865999862746,
	 -0.999997767390869483174635983631520264 },
	{-2.3941995847254499591662475176531302648,
	 -0.999290572307433910511567989690249400 },
	{-1.8190324797597378247738316630327062608,
	 -0.989903316169081293276835167427073756 },
	{-1.7445140197622020681114008416264823419,
	 -0.986379354153791969112941724918139729 },
	{-1.6368400771607998981195791570800243336,
	 -0.979378388686160896088193467134280915 },
	{-1.4065414735185131559385130393473881844,
	 -0.953315354218609138352551702320848737 },
	{-0.57437624514868349303377928670959528831,
	 -0.5833751404313424987670344072126874598 },
	{-0.073568785558911362274221592336149959999,
	 -0.08286396137711909831042082075498454337 },
	{ 0.028461827663256208808977584673202002879,
	  0.03210706344208566343705350699500313889 },
	{ 0.33428589857755334456284439160243780651,
	  0.3636096284546384720460023816875616105 },
	{ 0.99573933163446438798104944044804906767,
	  0.840924611319043491939088487949621471 },
	{ 1.3245537872123985233341699841508657526,
	  0.938959494070806285257388622681005190 },
	{ 1.6278059701408413484550683419001194931,
	  0.978668508936865328449354348238439554 },
	{ 1.9270374754242170597881953609075271405,
	  0.993574573155461809992204497698093482 },
	{ 2.5340066894404276757750648963687307071,
	  0.999661143627193766816855503615316639 },
	{ 2.5391417128280854273196058147704511207,
	  0.999670447579748988505790854060559791 },
	{ 3.8598786069328465059030649224182659356,
	  0.9999999520346834390316947427766402686}
};

ATF_TC(test_erf1);
ATF_TC_HEAD(test_erf1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, erf(x)) pairs");
}
ATF_TC_BODY(test_erf1, tc)
{
	size_t i, N;

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpreal_equalf(
			    erff((float)t1table[i].x),
				 (float)t1table[i].y));

		/* double */
		ATF_CHECK(fpreal_equal(
			    erf((double)t1table[i].x),
				(double)t1table[i].y));

		/* long double */
#ifdef	HAVE_ERFL
		ATF_CHECK(fpreal_equall(
			    erfl(t1table[i].x),
				 t1table[i].y));
#endif
	}
}

/*
 * Test case 2 -- Edge cases
 */
static const struct
t2entry {
	long double x;	/* Input */
	long double y;	/* erf output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN, NAN },
#endif

	/* If x is +-0, +-0 shall be returned */
	{  0.0,  0.0 },
	{ -0.0, -0.0 },

	/* If x is +-Inf, +-1 shall be returned */
#ifdef	INFINITY
	{  INFINITY,  1.0 },
	{ -INFINITY, -1.0 },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL,  1.0 },
	{ -HUGE_VAL, -1.0 },
#endif
#ifdef  HUGE_VALF
	{  HUGE_VALF,  1.0 },
	{ -HUGE_VALF, -1.0 },
#endif
#ifdef  HUGE_VALL
	{  HUGE_VAL,  1.0 },
	{ -HUGE_VAL, -1.0 }
#endif
};

ATF_TC(test_erf2);
ATF_TC_HEAD(test_erf2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_erf2, tc)
{
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		ATF_CHECK(fpreal_equalf(
			    erff((float)t2table[i].x),
				 (float)t2table[i].y));

		/* double */
		ATF_CHECK(fpreal_equal(
			    erf((double)t2table[i].x),
				(double)t2table[i].y));

		/* long double */
#ifdef	HAVE_ERFL
		ATF_CHECK(fpreal_equall(
			    erfl(t2table[i].x),
				 t2table[i].y));
#endif
	}
}

ATF_TC(test_erf3);
ATF_TC_HEAD(test_erf3, tc)
{
  atf_tc_set_md_var(tc,
		    "descr",
		    "Check for underflow");
}
ATF_TC_BODY(test_erf3, tc)
{
	float fx;
	double dx;
	long double ldx;
	long i, N;
	int haserrexcept;
	int haserrno;

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	/*
	 * Underflow occurs when |x| < DBL_MIN * (sqrt(Pi)/2)
	 * And if this happen, it _may_ result to a range error.
	 * Check the ~ of this instead.
	 */
	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	for (i = 0; i < N; i++) {
		/* float */
		do {
			fx = random_float(FP_NORMAL);
		} while (fabsf(fx) < DBL_MIN * (sqrt(M_PI)/2));
		errno = 0;
		clear_exceptions();
		(void)erff(fx);
		ATF_CHECK(errno_equalto(0));
		ATF_CHECK(not_raised_exceptions(MY_FE_ALL_EXCEPT));

		/* double */
		do {
			dx = random_double(FP_NORMAL);
		} while (fabs(dx) < DBL_MIN * (sqrt(M_PI)/2));
		errno = 0;
		clear_exceptions();
		(void)erf(dx);
		ATF_CHECK(errno_equalto(0));
		ATF_CHECK(not_raised_exceptions(MY_FE_ALL_EXCEPT));

		/* long double */
#ifdef	HAVE_ERFL
		do {
			ldx = random_long_double(FP_NORMAL);
		} while (fabsf(ldx) < DBL_MIN * (sqrt(M_PI)/2));
		errno = 0;
		clear_exceptions();
		(void)erfl(ldx);
		ATF_CHECK(errno_equalto(0));
		ATF_CHECK(not_raised_exceptions(MY_FE_ALL_EXCEPT));
#endif
	}

	/* Ok, couldn't resist */
	/* float */
	errno = 0;
	clear_exceptions();
	(void)erff(0.5 * DBL_MIN);
	ATF_CHECK(errno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_UNDERFLOW));

	/* double */
	errno = 0;
	clear_exceptions();
	(void)erf(0.5 * DBL_MIN);
	ATF_CHECK(errno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_UNDERFLOW));

	/* long double */
#ifdef	HAVE_ERFL
	errno = 0;
	clear_exceptions();
	(void)erfl(0.5 * DBL_MIN);
	ATF_CHECK(errno_equalto(ERANGE));
	ATF_CHECK(raised_exceptions(MY_FE_UNDERFLOW));
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_erf1);
	ATF_TP_ADD_TC(tp, test_erf2);
	ATF_TP_ADD_TC(tp, test_erf3);

	return atf_no_error();
}
