#define _XOPEN_SOURCE 600

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "subr_random.h"

static void usage(const char *progname);

int
main(int argc, char *argv[])
{
	const char *progname, *type;
	int i, min = -1, max = -1, total = -1;
	int opt;
	float fx, flower, fupper;
	double dx, dlower, dupper;
	long double ldx, ldlower, ldupper;

	/* Save program name for later use */
	progname = argv[0];
	type = argv[1];

	optind++;

	/* Parse arguments. */
	while ((opt = getopt(argc, argv, "m:M:t:")) != -1) {
		switch (opt) {
		case 'm':
			min = atoi(optarg);
			break;
		case 'M':
			max = atoi(optarg);
			break;
		case 't':
			total = atoi(optarg);
			break;
		default:
			usage(progname);
			/* NEVER REACHED */
		}
	}

	/* optind is the argv[] index of the first non-option element */
	if (optind < argc) {
		fprintf(stderr, "non-option argv[]-elements: ");
		while (optind < argc)
			fprintf(stderr, "%s ", argv[optind++]);
		fprintf(stderr, "\n");
		exit(EXIT_FAILURE);
	}

	/* Make sure all arguments were supplied, plus validate input */
	if (total == -1 || min == -1 || max == -1 || min > max) {
		usage(progname);
		/* NEVER REACHED */
	}

	if (strcmp(type, "float") && strcmp(type, "double")
	    && (strcmp(type, "ldouble"))) {
		usage(progname);
		/* NEVER REACHED */
	}

	/* Initialize random number generator */
	init_randgen();

	/* Ready to go */
	if (!strcmp(type, "float")) {
		if (min == 0 && max == 0) {
			flower = -FLT_MAX;
			fupper =  FLT_MAX;
		}

		for (i = 0; i < total; i++) {
			do {
				fx = random_float(FP_NORMAL);
			} while (fx < flower || fx > fupper);
			printf("% .8e\n", fx);
		}
	}

	if (!strcmp(type, "double")) {
		if (min == 0 && max == 0) {
			dlower = -DBL_MAX;
			dupper =  DBL_MAX;
		}

		for (i = 0; i < total; i++) {
			do {
				dx = random_double(FP_NORMAL);
			} while(dx < dlower || dx > dupper);
			printf("% .16e\n", dx);
		}
	}

	/* XXX: How many significand bits to write ? */
	if (!strcmp(type, "ldouble")) {
		if (min == 0 && max == 0) {
                        ldlower = -LDBL_MAX;
                        ldupper =  LDBL_MAX;
                }
		for (i = 0; i < total; i++) {
			do {
				ldx = random_long_double(FP_NORMAL);
			} while(ldx < ldlower || ldx > ldupper);
			printf("% .32Le\n", ldx);
		}
	}

	return (EXIT_SUCCESS);
}

static void
usage(const char *progname)
{
	fprintf(stderr,
	    "usage: %s {float, double, ldouble} -t total -m min -M max\n",
	    progname);
	fprintf(stderr,
	    "\tIf min == max, then all possible range is assumed,\n"
	    "\te.g. [-DBL_MAX, DBL_MAX]\n");
	exit(EXIT_FAILURE);
}
