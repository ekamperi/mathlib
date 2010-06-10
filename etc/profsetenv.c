#include <assert.h>
#include <fenv.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

/* Function prototypes */
static int prof_feclearexcept(size_t iterations);
static int prof_fegetenv(size_t iterations);
static int prof_fesetenv(size_t iterations);
static int prof_feupdateenv(size_t iterations);
static int prof_fesetenv_fegetenv(size_t iterations);
static int prof_feupdateenv_fegetenv(size_t iterations);

/* Constants and macros */
#define ITERATIONS (10 * 1000 * 1000)

#define	MARK_START(tv1)					\
	assert(gettimeofday(tv1, NULL) == 0);

#define MARK_END(tv2)					\
	assert(gettimeofday(tv2, NULL) == 0);

#define	MSECS(tv1, tv2)					\
	(((tv2).tv_sec  - (tv1).tv_sec ) * 1000 +	\
	 ((tv2).tv_usec - (tv1).tv_usec) / 1000)

static int extable[] = {
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
	FE_UNDERFLOW,
#endif
#ifdef	FE_ALL_EXCEPT
	FE_ALL_EXCEPT
#endif
};

int
main(void)
{
	int msecs;

	/* Profile feclearexcept() */
	msecs = prof_feclearexcept(ITERATIONS);
        printf("feclearexcept()           : %5d msecs for %d iterations\n",
            msecs, ITERATIONS);

        /* Profile fegetenv() */
        msecs = prof_fegetenv(ITERATIONS);
        printf("     fegetenv()           : %5d msecs for %d iterations\n",
            msecs, ITERATIONS);

	/* Profile fesetenv() with FE_DFL_ENV */
	msecs = prof_fesetenv(ITERATIONS);
	printf("     fesetenv() FE_DFL_ENV: %5d msecs for %d iterations\n",
	    msecs, ITERATIONS);

	/* Profile feupdateenvv() with FE_DFL_ENV */
	msecs = prof_feupdateenv(ITERATIONS);
	printf("  feupdateenv() FE_DFL_ENV: %5d msecs for %d iterations\n",
	    msecs, ITERATIONS);

	/* Profile fesetenv() without FE_DFL_ENV */
	msecs = prof_fesetenv_fegetenv(ITERATIONS);
	printf("     fesetenv()     random: %5d msecs for %d iterations\n",
	    msecs, ITERATIONS);

	/* Profile feupdateenvv() without FE_DFL_ENV */
	msecs = prof_feupdateenv_fegetenv(ITERATIONS);
	printf("  feupdateenv()     random: %5d msecs for %d iterations\n",
	    msecs, ITERATIONS);

	return 0;
}

static int
prof_feclearexcept(size_t iterations)
{
        struct timeval tv1, tv2;
        fenv_t env;
        size_t i, N;

        MARK_START(&tv1);

	N = sizeof(extable) / sizeof(extable[0]);
	assert(N != 0);

        for (i = 0; i < iterations; i++)
                assert(feclearexcept(extable[i%N]) == 0);

        MARK_END(&tv2);

        return MSECS(tv1, tv2);
}

static int
prof_fegetenv(size_t iterations)
{
	struct timeval tv1, tv2;
	fenv_t env;
	size_t i;

	MARK_START(&tv1);

	for (i = 0; i < iterations; i++)
		assert(fegetenv(&env) == 0);

	MARK_END(&tv2);

	return MSECS(tv1, tv2);
}

static int
prof_fesetenv(size_t iterations)
{
	struct timeval tv1, tv2;
	size_t i;

	MARK_START(&tv1);

	for (i = 0; i < iterations; i++)
		assert(fesetenv(FE_DFL_ENV) == 0);

	MARK_END(&tv2);

	return MSECS(tv1, tv2);
}

static int
prof_feupdateenv(size_t iterations)
{
	struct timeval tv1, tv2;
	size_t i;
	int msecs;

	MARK_START(&tv1);

	for (i = 0; i < iterations; i++)
		assert(feupdateenv(FE_DFL_ENV) == 0);

	MARK_END(&tv2);

	return MSECS(tv1, tv2);
}

static int
prof_fesetenv_fegetenv(size_t iterations)
{
	struct timeval tv1, tv2;
	fenv_t env;
	size_t i;

	MARK_START(&tv1);

	for (i = 0; i < iterations; i++) {
		assert(fegetenv(&env) == 0);
		assert(fesetenv(&env) == 0);
	}

	MARK_END(&tv2);

	return MSECS(tv1, tv2);
}

static int
prof_feupdateenv_fegetenv(size_t iterations)
{
	struct timeval tv1, tv2;
	fenv_t env;
	size_t i;

	MARK_START(&tv1);

	for (i = 0; i < iterations; i++) {
		assert(fegetenv(&env) == 0);
		assert(feupdateenv(&env) == 0);
	}

	MARK_END(&tv2);

	return MSECS(tv1, tv2);
}
