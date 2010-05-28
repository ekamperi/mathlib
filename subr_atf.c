#include <atf-c.h>
#include <atf-c/tc.h>
#include <atf-c/text.h>

#include "subr_atf.h"

bool
get_config_var_as_bool(const atf_tc_t *tc, const char *name)
{
	const char *valstr;
	bool rv;
	atf_error_t err;

	ATF_REQUIRE(tc);
	ATF_REQUIRE(name);
	ATF_REQUIRE(atf_tc_has_config_var(tc, name));

	valstr = atf_tc_get_config_var(tc, name);

	err = atf_text_to_bool(valstr, &rv);
	ATF_REQUIRE(!atf_is_error(err));

	return (rv);
}

long
get_config_var_as_long(const atf_tc_t *tc, const char *name)
{
	const char *valstr;
	long rv;
	atf_error_t err;

	ATF_REQUIRE(tc);
	ATF_REQUIRE(name);
	ATF_REQUIRE(atf_tc_has_config_var(tc, name));

	valstr = atf_tc_get_config_var(tc, name);

	err = atf_text_to_long(valstr, &rv);
	ATF_REQUIRE(!atf_is_error(err));

	return (rv);
}
