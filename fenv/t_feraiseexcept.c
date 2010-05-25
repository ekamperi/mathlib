#include <fenv.h>
#include <stdio.h>
#include <string.h>

#include <atf-c.h>

/*
 * Each FE_* symbol shall expand to an _integer constant_ expression,
 * so we can safely use them as initializers to an array holding integers.
 */
int extable[] = {
#ifdef	FE_DIVBYZERO
	FE_DIVBYZERO,
#endif
#ifdef	FE_INEXACT
	FE_INEXACT,
#endif
#ifdef	FE_INVALID
	FE_INVALID,
#endif
#ifdef	FE_OVERFLOW
	FE_OVERFLOW,
#endif
#ifdef	FE_UNDERFLOW
	FE_UNDERFLOW
#endif
};

/*
 * Test case 1 -- feraiseexcept()
 */
ATF_TC(test_feraiseexcept);
ATF_TC_HEAD(test_feraiseexcept, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Floating-point exception raising");
}
ATF_TC_BODY(test_feraiseexcept, tc)
{
	size_t i, N;
	int ex;

	N = sizeof(extable) / sizeof(extable[0]);
	for (i = 0; i < N; i++) {
		/*
		 * Clear all the floating-point exceptions
		 * If this is not met, further execution results cannot be trusted
		 */
		ATF_REQUIRE(feclearexcept(FE_ALL_EXCEPT) == 0);

		/* A little bit paranoid */
                ex = fetestexcept(FE_ALL_EXCEPT);
                ATF_REQUIRE((ex & FE_ALL_EXCEPT) == 0);

		/* Raise an exception */
		ATF_CHECK(feraiseexcept(extable[i]) == 0);

		/* Trust noone -- check for yourself */
		ex = fetestexcept(extable[i]);
		ATF_CHECK((ex & extable[i]) == extable[i]);
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_feraiseexcept);

	return atf_no_error();
}
