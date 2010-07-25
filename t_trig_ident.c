#include <atf-c.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "subr_atf.h"
#include "subr_fpcmp.h"
#include "subr_random.h"

/* Don't be too harsh with symbols */
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

/*
 * Test case 1 -- symmetry
 */
ATF_TC(test_symmetry);
ATF_TC_HEAD(test_symmetry, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Symmetry");
}
ATF_TC_BODY(test_symmetry, tc)
{
	double x;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		x = random_double(FP_NORMAL);

		ATF_PASS_OR_BREAK(fpreal_equal(sin(-x), -sin(x)));
		ATF_PASS_OR_BREAK(fpreal_equal(cos(-x),  cos(x)));
	}
}

/*
 * Test case 2 -- shifts and periodicity
 */
ATF_TC(test_shifts);
ATF_TC_HEAD(test_shifts, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Shifts and periodicity");
}
ATF_TC_BODY(test_shifts, tc)
{
	double x;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		x = random_double(FP_NORMAL);

		ATF_PASS_OR_BREAK(fpreal_equal(sin(x + M_PI_2), cos(x)));
		ATF_PASS_OR_BREAK(fpreal_equal(cos(x + M_PI_2), sin(x)));
	}
}

/*
 * Test case 3 -- Angle sum and difference
 */
ATF_TC(test_angle_sum);
ATF_TC_HEAD(test_angle_sum, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Angle sum and difference");
}
ATF_TC_BODY(test_angle_sum, tc)
{
	double x, y, s;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		x = random_double(FP_NORMAL);
		y = random_double(FP_NORMAL);

		s = sin(x) * cos(y) + cos(x) * sin(y);
		ATF_PASS_OR_BREAK(fpreal_equal(sin(x+y), s));

		s = sin(x) * cos(y) - cos(x) * sin(y);
		ATF_PASS_OR_BREAK(fpreal_equal(sin(x-y), s));

		s = cos(x) * cos(y) + sin(x) * sin(y);
		ATF_PASS_OR_BREAK(fpreal_equal(cos(x-y), s));

		s = cos(x) * cos(y) + sin(x) * sin(y);
		ATF_PASS_OR_BREAK(fpreal_equal(cos(x-y), s));
	}
}

/*
 * Test case 4 -- Half, double, triple angle
 */
ATF_TC(test_half_angle);
ATF_TC_HEAD(test_half_angle, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Half, double, triple angle");
}
ATF_TC_BODY(test_half_angle, tc)
{
	double x;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		x = random_double(FP_NORMAL);

		ATF_PASS_OR_BREAK(
			fpreal_equal(
				sin(x/2)*sin(x/2),
				0.5*(1-cos(x))));
		ATF_PASS_OR_BREAK(
			fpreal_equal(
				cos(x/2)*cos(x/2),
				0.5*(1+cos(x))));

		ATF_PASS_OR_BREAK(
			fpreal_equal(
				sin(2*x),
				    2*sin(x)*cos(x)));
		ATF_PASS_OR_BREAK(
			fpreal_equal(cos(2*x),
			    cos(x)*cos(x) - sin(x)*sin(x)));

		ATF_PASS_OR_BREAK(
			fpreal_equal(sin(3*x),
			    (3*sin(x) - 4*sin(x)*sin(x)*sin(x))));
		ATF_PASS_OR_BREAK(
			fpreal_equal(cos(3*x),
			    4*cos(x)*cos(x)*cos(x) - 3*cos(x)));
	}
}

/*
 * Test case 5 -- Product to sum
 */
ATF_TC(test_product_to_sum);
ATF_TC_HEAD(test_product_to_sum, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Product to sum");
}
ATF_TC_BODY(test_product_to_sum, tc)
{
	double x, y, s;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		x = random_double(FP_NORMAL);
		y = random_double(FP_NORMAL);

		s = 0.5*(cos(x-y) + cos(x + y));
		ATF_PASS_OR_BREAK(fpreal_equal(cos(x)*cos(y), s));

		s = 0.5*(cos(x-y) - cos(x+y));
		ATF_PASS_OR_BREAK(fpreal_equal(sin(x)*sin(y), s));

		s = 0.5*(sin(x+y) + sin(x-y));
		ATF_PASS_OR_BREAK(fpreal_equal(sin(x)*cos(y), s));

		s = 0.5*(sin(x+y) - sin(x-y));
		ATF_PASS_OR_BREAK(fpreal_equal(cos(x)*sin(y), s));
	}
}

/*
 * Test case 6 -- Fundamental trigonometric identity
 */
ATF_TC(test_fundamental);
ATF_TC_HEAD(test_fundamental, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Fundamental trigonometric identity, sin^ + cos^ = 1");
}
ATF_TC_BODY(test_fundamental, tc)
{
	double x;
	long i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		x = random_double(FP_NORMAL);

		ATF_PASS_OR_BREAK(
			fpreal_equal(
				sin(x)*sin(x) + cos(x)*cos(x),
				1.0));
	}
}

/*
 * Test case 7 -- Power reduction
 */
ATF_TC(test_power_reduction);
ATF_TC_HEAD(test_power_reduction, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Power reduction");
}
ATF_TC_BODY(test_power_reduction, tc)
{
	double x, e1, e2;
	size_t i, N;

	N = get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		x = random_double(FP_NORMAL);

		/* sin^2(x) = 0.5(1 - cos(2x)) */
		e1 = sin(x) * sin(x);
		e2 = 0.5 * (1 - cos(2*x));
		ATF_PASS_OR_BREAK(fpreal_equal(e1, e2));

		/* cos^2(x) = 0.5(1 + cos(2x)) */
		e1 = cos(x) * cos(x);
		e2 = 0.5 * (1 + cos(2*x));
		ATF_PASS_OR_BREAK(fpreal_equal(e1, e2));

		/* sin^2(x)*cos^2(x) = 0.125(1-cos(4x)) */
		e1 = sin(x)*sin(x) * cos(x)*cos(x);
		e2 = 0.125*(1-cos(4*x));
		ATF_PASS_OR_BREAK(fpreal_equal(e1, e2));

		/* sin^3(x) = 0.25(3sin(x) - sin(3x)) */
		e1 = sin(x) * sin(x) * sin(x);
		e2 = 0.25 * (3*sin(x) - sin(3*x));
		ATF_PASS_OR_BREAK(fpreal_equal(e1, e2));

		/* cos^3(x) = 0.25(3cos(x) + cos(3x)) */
		e1 = cos(x) * cos(x) * cos(x);
		e2 = 0.25 * (3*cos(x) + cos(3*x));
		ATF_PASS_OR_BREAK(fpreal_equal(e1, e2));

		/* sin^3(x)*cos^3(x) = 0.03125(3sin(2x) - sin(6x)) */
		e1 = sin(x)*sin(x)*sin(x) * cos(x)*cos(x)*cos(x);
		e2 = 0.03125 * (3*sin(2*x) - sin(6*x));
		ATF_PASS_OR_BREAK(fpreal_equal(e1, e2));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_symmetry);
	ATF_TP_ADD_TC(tp, test_shifts);
	ATF_TP_ADD_TC(tp, test_angle_sum);
	ATF_TP_ADD_TC(tp, test_half_angle);
	ATF_TP_ADD_TC(tp, test_product_to_sum);
	ATF_TP_ADD_TC(tp, test_fundamental);
	ATF_TP_ADD_TC(tp, test_power_reduction);

	return atf_no_error();
}
