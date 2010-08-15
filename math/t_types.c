#include <math.h>

#include <atf-c.h>

/* Test case 1 -- float_t */
ATF_TC(test_float_t);
ATF_TC_HEAD(test_float_t, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "float_t is at least as wide as float");
}
ATF_TC_BODY(test_float_t, tc)
{
	ATF_CHECK(sizeof(float_t) >= sizeof(float));
}

/* Test case 2 -- double_t */
ATF_TC(test_double_t);
ATF_TC_HEAD(test_double_t, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "double_t is at least as wide as double"
	    "and at least as wide as float_t");
}
ATF_TC_BODY(test_double_t, tc)
{
	ATF_CHECK(sizeof(double_t) >= sizeof(double));
	ATF_CHECK(sizeof(double_t) >= sizeof(float_t));
}

/* Test case 3 -- FLT_EVAL_METHOD */
ATF_TC(test_flt_eval_method);
ATF_TC_HEAD(test_flt_eval_method, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "If FLT_EVAL_METHOD equals 0, float_t and double_t shall be float"
	    "and double, respectively; if FLT_EVAL_METHOD equals 1, they shall"
	    "both be double; if FLT_EVAL_METHOD equals 2, they shall both be"
	    "long double");
}
ATF_TC_BODY(test_flt_eval_method, tc)
{
#ifdef FLT_EVAL_METHOD
	if (FLT_EVAL_METHOD == 0) {
		ATF_CHECK(sizeof(float_t)  == sizeof(float));
		ATF_CHECK(sizeof(double_t) == sizeof(double));
	} else if (FLT_EVAL_METHOD == 1) {
		ATF_CHECK(sizeof(float_t)  == sizeof(double));
		ATF_CHECK(sizeof(double_t) == sizeof(double));
	} else if (FLT_EVAL_METHOD == 2) {
		ATF_CHECK(sizeof(float_t)  == sizeof(long double));
		ATF_CHECK(sizeof(double_t) == sizeof(long double));
	}
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_float_t);
	ATF_TP_ADD_TC(tp, test_double_t);
	ATF_TP_ADD_TC(tp, test_flt_eval_method);

	return atf_no_error();
}
