#include <fenv.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>

#include <atf-c.h>

#pragma STDC FENV_ACCESS ON

/*
 * Test case 1
 */
ATF_TC(test_feholdexcept1);
ATF_TC_HEAD(test_feholdexcept1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "feholdexcept() shall clear the floating-point status flags");
}
ATF_TC_BODY(test_feholdexcept1, tc)
{
	/* Raise all the exceptions */
	ATF_REQUIRE(feraiseexcept(FE_ALL_EXCEPT) == 0);

	/*
	 * We don't put a _REQUIRE() here, as the return value refers to the
	 * installation of a non-stop mode, which is optional.
	 */
	fenv_t env;
	ATF_CHECK(feholdexcept(&env) == 0);

	/* Make sure all exceptions were indeed cleared */
	ATF_CHECK(fetestexcept(FE_ALL_EXCEPT) == 0);
}

/*
 * Test case 2
 */
volatile sig_atomic_t flag = 0;

static void
myhandler(int sig)
{
	flag = 1;
}

ATF_TC(test_feholdexcept2);
ATF_TC_HEAD(test_feholdexcept2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "feholdexcept() shall install a non-stop mode (optional)");
}
ATF_TC_BODY(test_feholdexcept2, tc)
{
	/* Install a custom SIGFPE handler */
	ATF_REQUIRE(signal(SIGFPE, myhandler) != SIG_ERR);

	/* Install a non-stop mode */
        fenv_t env;
        ATF_REQUIRE(feholdexcept(&env) == 0);

        /* Make sure all exceptions were indeed cleared */
        ATF_CHECK(fetestexcept(FE_ALL_EXCEPT) == 0);

        float x = 1.0;
        float y = 0.0;
	volatile float z;

	/* Beam us up Scotty */
        z = x / y;

	/*
	 * There shouldn't be any SIGFPE signal delivered to us, since we are
	 * in non-stop, continue on floating-point exceptions mode.
	 */
	ATF_CHECK(flag == 0);

	/* The rest of the behaviours shouldn't be affected */
	ATF_CHECK(fpclassify(z) == FP_INFINITE);
        ATF_CHECK(fetestexcept(FE_ALL_EXCEPT) == FE_DIVBYZERO);

	/* XXX: Should we restore the fp environment ? */
	ATF_CHECK(feupdateenv(&env) == 0);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_feholdexcept1);
	ATF_TP_ADD_TC(tp, test_feholdexcept2);

	return atf_no_error();
}
