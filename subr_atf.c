#include <atf-c.h>
#include <atf-c/tc.h>
#include <atf-c/text.h>
#include <stdio.h>
#include <string.h>

#include "subr_atf.h"

/* Default values for variables */
static const struct
ventry {
	const char *ve_name;
	const char *ve_defval;
} vtable[] = {
	{ "iterations", "1000" },
	{ NULL, NULL }
};

static void
print_undefined_var_msg(const char *name, const char *defval)
{
	ATF_REQUIRE(name);
	ATF_REQUIRE(defval);

	fprintf(stderr,
	    "Variable '%s' appears to be undefined\n"
	    "Please provide a value for it, either by running the test\n"
	    "case via atf-run or by invoking it as <test> -v %s=<val>\n"
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
	const struct ventry *ve;
	const char *valstr, *defval;
	long rv;
	atf_error_t err;

	ATF_REQUIRE(tc);
	ATF_REQUIRE(name);

	/* Fallback to default value, if possible */
	defval = NULL;
	for (ve = vtable; ve != NULL; ve++) {
		if (ve->ve_name == NULL || ve->ve_defval == NULL)
			break;

		if (!strcmp(name, ve->ve_name)) {
			if (!atf_tc_has_config_var(tc, name)) {
				defval = ve->ve_defval;
				print_undefined_var_msg(name, defval);
			}
			break;
		}
	}

	/* If no default fallback value exists, strictly require it */
	if (!defval)
		ATF_REQUIRE(atf_tc_has_config_var(tc, name));

	valstr = atf_tc_get_config_var_wd(tc, name, defval);

	err = atf_text_to_long(valstr, &rv);
	ATF_REQUIRE(!atf_is_error(err));

	return (rv);
}
