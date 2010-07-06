#include <atf-c++.hpp>
#include <fenv.h>

/* Test case 1 */
ATF_TEST_CASE(test_extern);
ATF_TEST_CASE_HEAD(test_extern)
{
	set_md_var("descr", "Check whether C++ programs can use fenv.h");
}
ATF_TEST_CASE_BODY(test_extern)
{
	fexcept_t flag;
	fenv_t env;

	/* Make some fe*() calls */
	ATF_CHECK(feclearexcept(FE_ALL_EXCEPT) == 0);
	ATF_CHECK(fegetexceptflag(&flag, FE_ALL_EXCEPT) == 0);
	ATF_CHECK(fesetexceptflag(&flag, FE_ALL_EXCEPT) == 0);
	ATF_CHECK(feraiseexcept(FE_ALL_EXCEPT) == 0);
	(void) fegetround();
	ATF_CHECK(fesetround(fegetround()) == 0);
	ATF_CHECK(fegetenv(&env) == 0);
	ATF_CHECK(feholdexcept(&env) == 0);
	ATF_CHECK(fesetenv(&env) == 0);
	ATF_CHECK(feupdateenv(&env) == 0);
}

/* Add test case to test program */
ATF_INIT_TEST_CASES(tcs)
{
	ATF_ADD_TEST_CASE(tcs, test_extern);
}
