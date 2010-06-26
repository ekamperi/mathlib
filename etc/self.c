#include <atf-c.h>
#include <stdio.h>

#include "../include/subr_atf.h"

ATF_TC(test_self);
ATF_TC_HEAD(test_self, tc)
{
}
ATF_TC_BODY(test_self, tc)
{
	int i, N = 100;
	
	/*
	 * Make sure that:
	 * 1. failures are sticky 
	 * 2. for each failure, only one error message is printed
	 * (the latter requires visual inspection)
	 */
	ATF_FOR_LOOP(i, N, i++) {
		ATF_PASS_OR_BREAK(0);
		ATF_PASS_OR_BREAK(1);
	}

	printf("%d = i\n", i);
	ATF_CHECK(i == 1);
}

ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_self);

	return atf_no_error();
}
