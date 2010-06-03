#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "subr_fpcmp.h"

struct tentry {
	double x;	/* Input */
	double exp;	/* Input */
	double z;       /* ldexp() output */
} ttable[] = {
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_ldexp1);
ATF_TC_HEAD(test_ldexp1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, ldexp(x)) pairs");
}
ATF_TC_BODY(test_ldexp1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++) {
		ATF_CHECK(fabs(ldexp(ttable[i].x, ttable[i].exp) - ttable[i].z)
		    < 1E-5);
	}
}

/*
 * Test case 2 -- Edge cases
 */
struct t2entry {
	double x;       /* Input */
	double exp;	/* Input */
	double z;       /* ldexp() output */
} t2table[] = {
	/* If x is NaN, a NaN shall be returned */
#ifdef	NAN
	{ NAN,    0, NAN },
	{ NAN,    1, NAN },
	{ NAN,    2, NAN },
	{ NAN,   10, NAN },
	{ NAN,  100, NAN },
	{ NAN, 1000, NAN },
	{ NAN, 1E50, NAN },
#endif	/* NAN */

	/* If x is +-0 or +-Inf, x shall be returned */
	{ +0.0,    0, +0.0 },
	{ +0.0,    1, +0.0 },
	{ +0.0,    2, +0.0 },
	{ +0.0,   10, +0.0 },
	{ +0.0,  100, +0.0 },
	{ +0.0, 1000, +0.0 },
	{ +0.0, 1E50, +0.0 },

	{ -0.0,    0, -0.0 },
	{ -0.0,    1, -0.0 },
	{ -0.0,    2, -0.0 },
	{ -0.0,   10, -0.0 },
	{ -0.0,  100, -0.0 },
	{ -0.0, 1000, -0.0 },
	{ -0.0, 1E50, -0.0 },

#ifdef	INFINITY
	{ +INFINITY,    0, INFINITY },
	{ +INFINITY,    1, INFINITY },
	{ +INFINITY,    2, INFINITY },
	{ +INFINITY,   10, INFINITY },
	{ +INFINITY,  100, INFINITY },
	{ +INFINITY, 1000, INFINITY },
	{ +INFINITY, 1E50, INFINITY },

	{ -INFINITY,    0, -INFINITY },
	{ -INFINITY,    1, -INFINITY },
	{ -INFINITY,    2, -INFINITY },
	{ -INFINITY,   10, -INFINITY },
	{ -INFINITY,  100, -INFINITY },
	{ -INFINITY, 1000, -INFINITY },
	{ -INFINITY, 1E50, -INFINITY },
#endif	/* INFINITY */

	/* If exp is 0, x shall be returned */
	{   0.0, 0,    0.0 },
	{   0.1, 0,    0.1 },
	{   0.2, 0,    0.2 },
	{   0.4, 0,    0.4 },
	{   0.8, 0,    0.8 },
	{  1E-5, 0,   1E-5 },
	{ 1E-10, 0,  1E-10 },
	{ 1E-50, 0,  1E-50 },

	{    0, 0,    0 },
	{    1, 0,    1 },
	{    2, 0,    2 },
	{    4, 0,    4 },
	{    8, 0,    8 },
	{  100, 0,  100 },
	{ 1000, 0, 1000 },
	{ 1E10, 0, 1E10 },
	{ 1E50, 0, 1E50 }
};

ATF_TC(test_ldexp2);
ATF_TC_HEAD(test_ldexp2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_ldexp2, tc)
{
	double oval;	/* output value */
	size_t i, N;

	N = sizeof(t2table) / sizeof(t2table[0]);

	for (i = 0; i < N; i++) {
		/* Ready to go */
		oval = ldexp(t2table[i].x, t2table[i].exp);
		ATF_CHECK(fpcmp_equal(oval, t2table[i].z));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_ldexp1);
	ATF_TP_ADD_TC(tp, test_ldexp2);

	return atf_no_error();
}
