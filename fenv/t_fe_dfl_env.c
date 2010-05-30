#include <atf-c.h>
#include <fenv.h>
#include <stdio.h>
#include <string.h>

#pragma STDC FENV_ACCESS ON

/*
 * Test case 1 -- fesetenv()
 */
ATF_TC(test_fesetenv);
ATF_TC_HEAD(test_fesetenv, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "fesetenv()");
}
ATF_TC_BODY(test_fesetenv, tc)
{
	fenv_t env, env2;

	/*
	 * Since we are going to compare the two environments,
	 * be paranoid and assign to them different contents.
	 */
	memset(&env, 0x00, sizeof(env));
	memset(&env2, 0xFF, sizeof(env2));

	/* Get the floating point environment installed at program startup */
	ATF_REQUIRE(fegetenv(&env) == 0);

	/* Set the floating point environment to the default one */
	ATF_REQUIRE(fesetenv(FE_DFL_ENV) == 0);

	/* And get it again */
	ATF_REQUIRE(fegetenv(&env2) == 0);

	/* These two should be equal */
	ATF_CHECK(memcmp(&env, &env2, sizeof(env)) == 0);
}

/*
 * Test case 2 -- feupdateenv()
 */
ATF_TC(test_feupdateenv);
ATF_TC_HEAD(test_feupdateenv, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "feupdateenv()");
}
ATF_TC_BODY(test_feupdateenv, tc)
{
	fenv_t env, env2;

	/*
	 * Since we are going to compare the two environments,
	 * be paranoid and assign to them different contents.
	 */
	memset(&env, 0x00, sizeof(env));
	memset(&env2, 0xFF, sizeof(env2));

	/* Get the floating point environment installed at program startup */
	ATF_REQUIRE(fegetenv(&env) == 0);

	/* Update the floating point environment to the default one */
	ATF_REQUIRE(feupdateenv(FE_DFL_ENV) == 0);

	/* And get it again */
	ATF_REQUIRE(fegetenv(&env2) == 0);

	/* These two should be equal */
	ATF_CHECK(memcmp(&env, &env2, sizeof(env)) == 0);
}

/*
 * Test case 3 -- memcpy()
 */
ATF_TC(test_memcpy);
ATF_TC_HEAD(test_memcpy, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "memcmpy()");
}
ATF_TC_BODY(test_memcpy, tc)
{
	fenv_t env1, env2;
	fenv_t tmp;

	/* Get the floating point environment installed at program startup */
        ATF_REQUIRE(fegetenv(&env1) == 0);

	memcpy(&tmp, FE_DFL_ENV, sizeof tmp);

	/*
	 * Set the floating point environment to the default one,
	 * but do so via an intermediate variable.
	 */
        ATF_REQUIRE(fesetenv(&tmp) == 0);

	/* And get it again */
        ATF_REQUIRE(fegetenv(&env2) == 0);

	ATF_CHECK(memcmp(&env1, &env2, sizeof env1) == 0);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_fesetenv);
	ATF_TP_ADD_TC(tp, test_feupdateenv);
	ATF_TP_ADD_TC(tp, test_memcpy);

	return atf_no_error();
}
