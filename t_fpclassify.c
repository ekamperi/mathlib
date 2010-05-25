#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <atf-c.h>

/*
 * Test case 1 -- float version
 */
struct t1entry {
	float x;	/* Input */
	int class;	/* fpclassify() output */
} t1table[] = {
	/* Infinity */
#ifdef  HUGE_VALF
        {  HUGE_VALF, FP_INFINITE },
        { -HUGE_VALF, FP_INFINITE },
#endif

	/* NAN */
#ifdef	NAN
	{ NAN, FP_NAN },
#endif

	/* Normal numbers */
	{    1, FP_NORMAL },
	{   10, FP_NORMAL },
	{  1E5, FP_NORMAL },

	/* Subnormal numbers */
	/* Zero */
	{  0.0, FP_ZERO },
	{ -0.0, FP_ZERO }
};

ATF_TC(test_fpclassify_float);
ATF_TC_HEAD(test_fpclassify_float, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check fpclassify with float input");
}
ATF_TC_BODY(test_fpclassify_float, tc)
{
	size_t i, N;
	int oval;	/* output value */

	N = sizeof(t1table) / sizeof(t1table[0]);
	for (i = 0; i < N; i++) {
	  printf("%f\n", t1table[i].x);
		oval = fpclassify(t1table[i].x);
		ATF_CHECK(oval == t1table[i].class);
	}
}

/*
 * Test case 2 -- double version
 */
struct t2entry {
	double x;	/* Input */
	int class;	/* fpclassify() output */
} t2table[] = {
	/* Infinity */
#ifdef  HUGE_VALF
        {  HUGE_VALF, FP_INFINITE },
        { -HUGE_VALF, FP_INFINITE },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL, FP_INFINITE },
	{ -HUGE_VAL, FP_INFINITE },
#endif 

	/* NAN */
#ifdef	NAN
	{ NAN, FP_NAN },
#endif

	/* Normal numbers */
	/* Subnormal numbers */
	{ 1.00000e-308, FP_SUBNORMAL },
	{ 2.00000e-308, FP_SUBNORMAL },
	{ 2.22392e-308, FP_SUBNORMAL },
	{ 2.22446e-308, FP_SUBNORMAL },
	{ 2.22506e-308, FP_SUBNORMAL },

	/* Zero */
	{  0.0, FP_ZERO },
	{ -0.0, FP_ZERO }
};

ATF_TC(test_fpclassify_double);
ATF_TC_HEAD(test_fpclassify_double, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check fpclassify with double input");
}
ATF_TC_BODY(test_fpclassify_double, tc)
{
	size_t i, N;
	int oval;	/* output value */

	N = sizeof(t2table) / sizeof(t2table[0]);
	for (i = 0; i < N; i++) {
		oval = fpclassify(t2table[i].x);
		ATF_CHECK(oval == t2table[i].class);
	}
}

/*
 * Test case 3 -- long double version
 */
struct t3entry {
	long double x;	/* Input */
	int class;	/* fpclassify() output */
} t3table[] = {
	/* Infinity */
#ifdef  HUGE_VALF
        {  HUGE_VALF, FP_INFINITE },
        { -HUGE_VALF, FP_INFINITE },
#endif
#ifdef	HUGE_VAL
	{  HUGE_VAL, FP_INFINITE },
	{ -HUGE_VAL, FP_INFINITE },
#endif
#ifdef	HUGE_VALL
	{  HUGE_VALL, FP_INFINITE },
	{ -HUGE_VALL, FP_INFINITE },
#endif

	/* NAN */
#ifdef	NAN
	{ NAN, FP_NAN },
#endif

	/* Normal numbers */
	/* Subnormal numbers */
	/* Zero */
	{  0.0, FP_ZERO },
	{ -0.0, FP_ZERO }
};

ATF_TC(test_fpclassify_long_double);
ATF_TC_HEAD(test_fpclassify_long_double, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check fpclassify with long double input");
}
ATF_TC_BODY(test_fpclassify_long_double, tc)
{
	size_t i, N;
	int oval;	/* output value */

	N = sizeof(t3table) / sizeof(t3table[0]);
	for (i = 0; i < N; i++) {
		oval = fpclassify(t3table[i].x);
		ATF_CHECK(oval == t3table[i].class);
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fpclassify_float);
	ATF_TP_ADD_TC(tp, test_fpclassify_double);
	ATF_TP_ADD_TC(tp, test_fpclassify_long_double);

	return atf_no_error();
}
