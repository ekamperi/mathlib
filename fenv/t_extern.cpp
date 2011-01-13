#include <atf-c++.hpp>
#include <fenv.h>

/* Test case 1 */
ATF_TEST_CASE(test_extern);
ATF_TEST_CASE_HEAD(test_extern)
{
	set_md_var("descr",
	    "Check whether C++ programs can use fenv.h");
}
ATF_TEST_CASE_BODY(test_extern)
{
	fexcept_t flag;
	fenv_t env;

	/* Make some fe*() calls */
	ATF_REQUIRE(feclearexcept(FE_ALL_EXCEPT) == 0);
	ATF_REQUIRE(fegetexceptflag(&flag, FE_ALL_EXCEPT) == 0);
	ATF_REQUIRE(fesetexceptflag(&flag, FE_ALL_EXCEPT) == 0);
	ATF_REQUIRE(feraiseexcept(FE_ALL_EXCEPT) == 0);
	(void) fegetround();
	ATF_REQUIRE(fesetround(fegetround()) == 0);
	ATF_REQUIRE(fegetenv(&env) == 0);
	ATF_REQUIRE(feholdexcept(&env) == 0);
	ATF_REQUIRE(fesetenv(&env) == 0);
	ATF_REQUIRE(feupdateenv(&env) == 0);
}

/* Add test case to test program */
ATF_INIT_TEST_CASES(tcs)
{
	ATF_ADD_TEST_CASE(tcs, test_extern);
}
