#include <atf-c.h>
#include <fenv.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#pragma STDC FENV_ACCESS ON

fenv_t env1;

static void *
mythread(void *arg)
{
	fenv_t env2;
	size_t i;

	for (i = 0; i < 100; i++) {
		ATF_REQUIRE(fegetenv(&env2) == 0);
		ATF_REQUIRE(memcmp(&env1, &env2, sizeof(fenv_t)) == 0);
	}

	pthread_exit(NULL);
}

ATF_TC(test_thread);
ATF_TC_HEAD(test_thread, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Make sure fp env is saved/restored across mcontext switces");
}
ATF_TC_BODY(test_thread, tc)
{
#define	NTHREADS 10
	pthread_t tid[NTHREADS];
	size_t i;

	/*
	 * Save the default (start-up) floating environemnt, whatever
	 * that may be.
	 */
	ATF_REQUIRE(fegetenv(&env1) == 0);

	/* Create the threads */
	for (i = 0; i < NTHREADS; i++)
		ATF_REQUIRE(pthread_create(&tid[i], NULL, mythread, NULL) == 0);

	/* Make sure all threads are done */
	for (i = 0; i < NTHREADS; i++)
		ATF_REQUIRE(pthread_join(tid[i], NULL) == 0);
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_thread);

	return atf_no_error();
}
