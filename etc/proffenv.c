#include <assert.h>
#include <fenv.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

/* Function prototypes */
static int prof_clearexcept_all(size_t iterations);
static int prof_clearexcept_random(size_t iterations);

static int prof_getenv(size_t iterations);

static int prof_raiseexcept(size_t iterations);

static int prof_setenv_default(size_t iterations);
static int prof_setenv_getenv(size_t iterations);

static int prof_updateenv_default(size_t iterations);
static int prof_updateenv_getenv(size_t iterations);


/* Constants and macros */
#define ITERATIONS (1 * 1000 * 1000)

/*
 * They are esentially the same, but we still define them as if
 * they were different, for conceptual consistency and clarity.
 */
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

struct pentry {
	const char *p_desc;
	int (*p_func)();
	size_t p_iter;
} ptable[] = {
	{ "feclearexcept() FE_ALL_EXCEPT",
	  prof_clearexcept_all,
	  ITERATIONS },

	{ "feclearexcept()        random",
	  prof_clearexcept_random,
	  ITERATIONS },

	{ "     fegetenv()              ",
	  prof_getenv,
	  ITERATIONS },

        { "feraiseexcept()              ",
          prof_raiseexcept,
          ITERATIONS },

	{ "     fesetenv()    FE_DFL_ENV",
	  prof_setenv_default,
	  ITERATIONS },

	{ "     fesetenv()        random",
	  prof_setenv_getenv,
	  ITERATIONS },

	{ "  feupdateenv()    FE_DFL_ENV",
	  prof_updateenv_default,
	  ITERATIONS },

	{ "  feupdateenv()        random",
	  prof_updateenv_getenv,
	  ITERATIONS },

	{ NULL, NULL, 0 }
};

int
main(void)
{
	const struct pentry *p;
	int msecs;

	for (p = ptable; p; p++) {
		if (p->p_desc == NULL && p->p_func == NULL && p->p_iter == 0)
			break;

		msecs = p->p_func(p->p_iter);
		printf("%s: %5d msecs for %d iterations\n",
		       p->p_desc, msecs, p->p_iter);
	}

	return 0;
}

static int
prof_clearexcept_all(size_t iterations)
{
	struct timeval tv1, tv2;
	size_t i;

	MARK_START(&tv1);

	for (i = 0; i < iterations; i++)
		assert(feclearexcept(FE_ALL_EXCEPT) == 0);

	MARK_END(&tv2);

	return MSECS(tv1, tv2);
}

static int
prof_clearexcept_random(size_t iterations)
{
	struct timeval tv1, tv2;
	size_t i, N;

	N = sizeof(extable) / sizeof(extable[0]);
	assert(N != 0);

	MARK_START(&tv1);

	for (i = 0; i < iterations; i++)
		assert(feclearexcept(extable[i%N]) == 0);

	MARK_END(&tv2);

	return MSECS(tv1, tv2);
}

static int
prof_getenv(size_t iterations)
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
prof_setenv_default(size_t iterations)
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
prof_updateenv_default(size_t iterations)
{
	struct timeval tv1, tv2;
	size_t i;

	MARK_START(&tv1);

	for (i = 0; i < iterations; i++)
		assert(feupdateenv(FE_DFL_ENV) == 0);

	MARK_END(&tv2);

	return MSECS(tv1, tv2);
}

static int
prof_setenv_getenv(size_t iterations)
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
prof_updateenv_getenv(size_t iterations)
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

static int
prof_raiseexcept(size_t iterations)
{
	struct timeval tv1, tv2;
	size_t i, N;

	N = sizeof(extable) / sizeof(extable[0]);
	assert(N > 0);

	MARK_START(&tv1);

	for (i = 0; i < iterations; i++) {
		assert(feraiseexcept(extable[i%N]) == 0);
	}

	MARK_END(&tv2);

	return MSECS(tv1, tv2);
}
