#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "gen.h"
#include "subr_random.h"

#define NSAMPLE (10*1000)

/*
 * They are esentially the same, but we still define them as if
 * they were different, for conceptual consistency and clarity.
 */
#define MARK_START(tv1)						\
	assert(gettimeofday(tv1, NULL) == 0);

#define MARK_END(tv2)						\
	assert(gettimeofday(tv2, NULL) == 0);

#define USECS(tv1, tv2)						\
	(((tv2).tv_sec  - (tv1).tv_sec ) * 1000000 +		\
	 ((tv2).tv_usec - (tv1).tv_usec))

/*
 * Make these globals, so that we allocate memory only once
 * during the program's lifetime.
 */
double *dx = NULL;
double *dy = NULL;
double *dz = NULL;
struct timeval *start = NULL;
struct timeval *end   = NULL;

static void
init_arrays(void)
{
	dx    = malloc(NSAMPLE * sizeof(double));
	dy    = malloc(NSAMPLE * sizeof(double));
	dz    = malloc(NSAMPLE * sizeof(double));
	start = malloc(NSAMPLE * sizeof(struct timeval));
	end   = malloc(NSAMPLE * sizeof(struct timeval));

	assert(dx && dy && dz && start && end);
}

static void
reset(void)
{
	assert(dx && dy && dz && start && end);

	memset(dx,    0, NSAMPLE * sizeof(double));
	memset(dy,    0, NSAMPLE * sizeof(double));
	memset(dz,    0, NSAMPLE * sizeof(double));
	memset(start, 0, NSAMPLE * sizeof(struct timeval));
	memset(end,   0, NSAMPLE * sizeof(struct timeval));
}

static void
cleanup(void)
{
	assert(dx && dy && dz && start && end);

	free(dx);
	free(dy);
	free(dz);
	free(start);
	free(end);

	dx = dy = dz = NULL;
	start = end = NULL;
}

static int
proffunc(const char *fname)
{
	const struct fentry *f;
	FILE *fp;
	size_t i, j;

	/* Lookup the function */
	f = getfunctionbyname(fname);
	if (f == NULL)
		return (-1);

	/* Zero out the arrays we will be writing at */
	reset();

	/* Generate random input -- do it before hand */
	for (i = 0; i < NSAMPLE; i++) {
		if (f->f_narg >= 1) {
			do {
				dx[i] = random_double(FP_NORMAL);
			} while (!f->f_u.fp1(dx[i]));
		}
		if (f->f_narg >= 2) {
			do {
				dx[i] = random_double(FP_NORMAL);
				dy[i] = random_double(FP_NORMAL);
			} while (!f->f_u.fp2(dx[i], dy[i]));
		}
	}

	/* Ready to go */
	for (i = 0; i < NSAMPLE; i++) {
		MARK_START(&start[i]);

		for (j = 0; j < 1000; j++) {
			if (f->f_narg == 1) {
				dz[i] = f->f_libm_real(dx[i]);
			} else {
				dz[i] = f->f_libm_real(dx[i], dy[i]);
			}
		}

		MARK_END(&end[i]);
	}

	/* Calculate diffs and dump them to the file */
	char buf[256];
	snprintf(buf, sizeof(buf), "%s.csv", fname);
	fp = fopen(buf, "w");
	assert(fp);

	fprintf(fp, "#%d\n", f->f_narg);
	for (i = 0; i < NSAMPLE; i++) {
		if (f->f_narg == 1) {
			if (fpclassify(dx[i]) != FP_NORMAL ||
			    fpclassify(dz[i]) != FP_NORMAL)
				continue;
			fprintf(fp, "% .16e   % ld\n",
			    dx[i], USECS(start[i], end[i]));
		}
		else {
                        if (fpclassify(dx[i]) != FP_NORMAL ||
			    fpclassify(dy[i]) != FP_NORMAL ||
                            fpclassify(dz[i]) != FP_NORMAL)
				continue;
			fprintf(fp, "%.16e   %.16e   %ld\n",
			    dx[i], dy[i], USECS(start[i], end[i]));
		}
	}

	/* Success */
	return (0);
}

int
main(int argc, char *argv[])
{
	int i, rv, total, all;
	const char *target;
	const struct fentry *f;

	/* Skip program name */
	argv++;
	argc--;

	/*
	 * If none argument or "all" is given, then we assume
	 * that all functions should be probed.
	 */
	all = 0;
	if (argc == 0 || (argc == 1 && !strcmp(argv[0], "all"))) {
		argv++;
		argc--;

		all = 1;
	}

	/* Initialize random number generator */
	init_randgen();

	/* Allocate memory */
	init_arrays();

	total = all ? fsize : argc;

	for (i = 0; i < total; i++) {
		f = getfunctionbyidx(i);
		target = all ? f->f_name : argv[i];
		printf("[%2d/%2d] %-10s : ", i+1, total, target);
		fflush(stdout);
		rv = proffunc(target);
		if (rv != -1) {
			printf("OK\n");
		} else {
			fprintf(stderr, "function: %s not probed -- skipping\n",
			    argv[i]);
			continue;
		}
	}

	/* Free up resources */
	cleanup();

	return (EXIT_SUCCESS);
}
