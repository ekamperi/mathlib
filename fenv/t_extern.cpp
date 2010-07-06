#include <atf-c++.hpp>
#include <fenv.h>

/* Test case 1 */
ATF_TEST_CASE(my_test_case);
ATF_TEST_CASE_HEAD(my_test_case)
{
	set_md_var("descr", "Check whether C++ programs can use fenv.h");
}
ATF_TEST_CASE_BODY(my_test_case)
{
}

/* Add test case to test program */
ATF_INIT_TEST_CASES(tcs)
{
	ATF_ADD_TEST_CASE(tcs, my_test_case);
}
