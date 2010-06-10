#include <assert.h>
#include <fenv.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

/* Function prototypes */
static int prof_fesetenv(size_t iterations);
static int prof_feupdateenv(size_t iterations);

#define ITERATIONS (10 * 1000 * 1000)

int
main(void)
{
	int msecs;

	/* Profile fesetenv() with FE_DFL_ENV */
	msecs = prof_fesetenv(ITERATIONS);
	printf("   fesetenv() with FE_DFL_ENV: %d msecs for %d iterations\n",
	    msecs, ITERATIONS);

	/* Profile feupdateenvv() with FE_DFL_ENV */
        msecs = prof_feupdateenv(ITERATIONS);
        printf("feupdateenv() with FE_DFL_ENV: %d msecs for %d iterations\n",
            msecs, ITERATIONS);

	return 0;
}

static int
prof_fesetenv(size_t iterations)
{
	struct timeval tv1, tv2;
        size_t i;
        int msecs;

	/* Mark the start */
        assert(gettimeofday(&tv1, NULL) == 0);

	/* Fire off */
        for (i = 0; i < iterations; i++)
                assert(fesetenv(FE_DFL_ENV) == 0);

        /* Mark the end */
        assert(gettimeofday(&tv2, NULL) == 0);

	/* Calculate time elapsed */
        msecs = (tv2.tv_sec  - tv1.tv_sec ) * 1000 +
	        (tv2.tv_usec - tv1.tv_usec) / 1000;

	return (msecs);
}

static int
prof_feupdateenv(size_t iterations)
{
	struct timeval tv1, tv2;
        size_t i;
        int msecs;

        /* Mark the start */
        assert(gettimeofday(&tv1, NULL) == 0);

        /* Fire off */
        for (i = 0; i < iterations; i++)
                assert(feupdateenv(FE_DFL_ENV) == 0);

        /* Mark the end */
        assert(gettimeofday(&tv2, NULL) == 0);

        /* Calculate time elapsed */
        msecs = (tv2.tv_sec  - tv1.tv_sec ) * 1000 +
	    (tv2.tv_usec - tv1.tv_usec) / 1000;

        return (msecs);
}
