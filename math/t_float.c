#include <atf-c.h>
#include <math.h>

/* Test case 1 -- float.h constants */
ATF_TC(test_float);
ATF_TC_HEAD(test_float, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Test availability of <float.h> constants");
}
ATF_TC_BODY(test_float, tc)
{
	/* Radix of exponent representation */
#ifdef	FLT_RADIX
	ATF_CHECK(FLT_RADIX >= 2);
#endif

	/*
	 * Number of decimal digits, n, such that any floating-point number in
	 * the widest supported floating type with pmax radix b digits can be
	 * rounded to a floating-point number with n decimal digits and back
	 * again without change to the value.
	 */
#ifdef	DECIMAL_DIG
	ATF_CHECK(DECIMAL_DIG >= 10);
#endif

	/*
	 * Number of decimal digits, q, such that any floating-point number with
	 * q decimal digits can be rounded into a floating-point number with p
	 * radix b digits and back again without change to the q decimal digits.
	 */
#ifdef	FLT_DIG
	ATF_CHECK(FLT_DIG >= 6);
#endif
#ifdef	DBL_DIG
	ATF_CHECK(DBL_DIG >= 10);
#endif
#ifdef	LDBL_DIG
	ATF_CHECK(LDBL_DIG >= 10);
#endif

	/*
	 * Minimum negative integer such that 10 raised to that power is in the
	 * range of normalized floating-point numbers.
	 */
#ifdef	FLT_MIN_10_EXP
	ATF_CHECK(FLT_MIN_10_EXP >= -37);
	ATF_CHECK(fpclassify(powf(10.0, FLT_MIN_10_EXP)) == FP_NORMAL);
#endif
#ifdef	DBL_MIN_10_EXP
	ATF_CHECK(DBL_MIN_10_EXP >= -37);
	ATF_CHECK(fpclassify(pow(10.0, DBL_MIN_10_EXP)) == FP_NORMAL);
#endif
#ifdef	LDBL_MIN_10_EXP
	ATF_CHECK(LDBL_MIN_10_EXP >= -37);
	ATF_CHECK(fpclassify(powl(10.0, LDBL_MIN_10_EXP)) == FP_NORMAL);
#endif

	/*
	 * Maximum integer such that 10 raised to that power is in the range of
	 * representable finite floating-point numbers.
	 */
#ifdef	FLT_MAX_10_EXP
	ATF_CHECK(FLT_MAX_10_EXP >= +37);
#endif
#ifdef	DBL_MAX_10_EXP
	ATF_CHECK(DBL_MAX_10_EXP >= +37);
#endif
#ifdef	LDBL_MAX_10_EXP
	ATF_CHECK(LDBL_MAX_10_EXP >= +37);
#endif

	/* Maximum representable finite floating-point number */
#ifdef	FLT_MAX
	ATF_CHECK(FLT_MAX >= 1E+37);
#endif
#ifdef	DBL_MAX
	ATF_CHECK(DBL_MAX >= 1E+37);
#endif
#ifdef	LDBL_MAX
	ATF_CHECK(LDBL_MAX >= 1E+37);
#endif

	/*
	 * The difference between 1 and the least value greater than 1 that is
	 * representable in the given floating-point type.
	 */
#ifdef	FLT_EPSILON
	ATF_CHECK(FLT_EPSILON >= 1E-5);
#endif
#ifdef	DBL_EPSILON
	ATF_CHECK(DBL_EPSILON >= 1E-5);
#endif
#ifdef	LDBL_EPSILON
	ATF_CHECK(LDBL_EPSILON >= 1E-9);
#endif

	/* Minimum normalized positive floating-point number */
#ifdef	FLT_MIN
	ATF_CHECK(FLT_MIN >= 1E-37);
	ATF_CHECK(fpclassify(FLT_MIN) == FP_NORMAL);
#endif
#ifdef	DBL_MIN
	ATF_CHECK(DBL_MIN >= 1E-37);
	ATF_CHECK(fpclassify(DBL_MIN) == FP_NORMAL);
#endif
#ifdef	LDBL_MIN
	ATF_CHECK(LDBL_MIN >= 1E-37);
	ATF_CHECK(fpclassify(LDBL_MIN) == FP_NORMAL);
#endif
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_float);

	return atf_no_error();
}
