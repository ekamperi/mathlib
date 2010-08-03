#include <atf-c.h>
#include <atf-c/tc.h>
#include <atf-c/text.h>
#include <stdio.h>
#include <string.h>

#include "subr_atf.h"

/* Default value for 'iterations' variable, if not provided */
static const char *defiterations = "1000";

static void
print_undefined_var_msg(const char *name, const char *defval)
{
	ATF_REQUIRE(name);
	fprintf(stderr,
	    "Variable '%s' appears to be undefined\n"
	    "Please provide a value for it, by either running the test case\n"
	    "via atf-run or by invoking it as <test> -v %s=<val>\n"
	    "Using default value '%s'\n",
	    name, name, defval);
}

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
	const char *valstr, *defval;
	long rv;
	atf_error_t err;

	ATF_REQUIRE(tc);
	ATF_REQUIRE(name);

	if (!strcmp(name, "iterations") &&
	    !atf_tc_has_config_var(tc, name)) {
		print_undefined_var_msg(name, defiterations);
		defval = defiterations;
	} else {
		ATF_REQUIRE(atf_tc_has_config_var(tc, name));
		/* NEVER RETURNS */
	}

	valstr = atf_tc_get_config_var_wd(tc, name, defval);

	err = atf_text_to_long(valstr, &rv);
	ATF_REQUIRE(!atf_is_error(err));

	return (rv);
}
