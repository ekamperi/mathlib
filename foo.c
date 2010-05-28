#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <stdio.h>
#include <stdlib.h>

#include "subr_atf.h"

ATF_TC(test_foo);
ATF_TC_HEAD(test_foo, tc)
{
}
ATF_TC_BODY(test_foo, tc)
{
	long val;
	val = get_config_var_as_long(tc, "lala");
	printf(">>>> %ld\n", val);
}

ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_foo);

	return atf_no_error();
}
