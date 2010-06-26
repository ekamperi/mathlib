#include <atf-c.h>
#include <math.h>

/* Test case 1 -- various macros availibility */
ATF_TC(test_macros_avail);
ATF_TC_HEAD(test_macros_avail, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Test availibility of fp macros in math.h");
}
ATF_TC_BODY(test_macros_avail, tc)
{
	int isdefined;

	/* fpclassify() */
	isdefined = 0;
#ifdef fpclassify
	isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

        /* isfinite() */
        isdefined = 0;
#ifdef isfinite
        isdefined = 1;
#endif
        ATF_CHECK(isdefined == 1);

	/* isinf() */
        isdefined = 0;
#ifdef isinf
        isdefined = 1;
#endif
        ATF_CHECK(isdefined == 1);

        /* isnan() */
        isdefined = 0;
#ifdef isnan
        isdefined = 1;
#endif
        ATF_CHECK(isdefined == 1);

        /* isnormal() */
	isdefined = 0;
#ifdef isnormal
        isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* signbit() */
        isdefined = 0;
#ifdef signbit
        isdefined = 1;
#endif
        ATF_CHECK(isdefined == 1);

        /* isgreater() */
        isdefined = 0;
#ifdef isgreater
        isdefined = 1;
#endif
	ATF_CHECK(isdefined == 1);

	/* isgreaterequal() */
        isdefined = 0;
#ifdef isgreaterequal
        isdefined = 1;
#endif
        ATF_CHECK(isdefined == 1);

        /* isless() */
        isdefined = 0;
#ifdef isless
        isdefined = 1;
#endif
        ATF_CHECK(isdefined == 1);

        /* islessequal() */
	isdefined = 0;
#ifdef islessequal
        isdefined = 1;
#endif
        ATF_CHECK(isdefined == 1);

        /* islessgreater() */
	isdefined = 0;
#ifdef islessgreater
        isdefined = 1;
#endif
        ATF_CHECK(isdefined == 1);

        /* isunordered() */
	isdefined = 0;
#ifdef isunordered
        isdefined = 1;
#endif
        ATF_CHECK(isdefined == 1);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_macros_avail);

	return atf_no_error();
}
