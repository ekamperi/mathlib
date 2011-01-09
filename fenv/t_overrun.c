#include <atf-c.h>
#include <fenv.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "subr_atf.h"

#pragma STDC FENV_ACCESS ON

/*
 * Test case 1
 */
ATF_TC(test_overrun1);
ATF_TC_HEAD(test_overrun1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check for overrun");
}
ATF_TC_BODY(test_overrun1, tc)
{
	fenv_t env[2];
	unsigned char bytes[sizeof(fenv_t)];
	long N;
	size_t i, j;

	N = atf_tc_get_config_var_as_long(tc, "iterations");
	ATF_REQUIRE(N > 0);

	ATF_FOR_LOOP(i, N, i++) {
		for (j = 0; j < sizeof(fenv_t); j++)
			bytes[j] = rand() % 255;
		memcpy(&env[1], bytes, sizeof(fenv_t));

		ATF_REQUIRE(fegetenv(env) == 0);

		/* One failure is enough to prove corruption */
		ATF_PASS_OR_BREAK(
			memcmp(&env[1], bytes, sizeof(fenv_t)) == 0);
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_overrun1);

	return atf_no_error();
}
