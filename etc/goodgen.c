/*
 * The purpose of this utility is to generate C arrays of exact (x, $func(x))
 * pairs for consumption by the rest of the test cases.
 *
 * The code is a bit ugly. Sorry, tried to do it as less ugly as possible.
 */
#define _XOPEN_SOURCE 600

#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <gmp.h>
#include <mpfr.h>

#include "config.h"
#include "gen.h"
#include "subr_random.h"

/* Function prototypes */
static void gen_float(const char *fname, size_t total,
    float lower,
    float upper);
static void gen_double(const char *fname, size_t total,
    double lower,
    double upper);
static void gen_ldouble(const char *fname, size_t total,
    long double lower,
    long double upper);
static void usage(const char *progname);
static char *strtoupper(const char *str);

int
main(int argc, char *argv[])
{
	const char *progname;
	const char *fname;
	const char *type;
	int  min, max, total;
	int opt;
	double dlower, dupper;
	long double ldlower, ldupper;

	/*
	 * Save program name, function name and floating
	 * point type for later use.
	 */
	progname = argv[0];
	fname = argv[1];
	type = argv[2];

	optind += 2;

	/* Parse arguments. */
	min = max = total = -1;
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
	    && (strcmp(type, "ldouble") && strcmp(type, "all"))) {
		usage(progname);
		/* NEVER REACHED */
	}

	/* Initialize random number generator */
	init_randgen();

	/* Print if guards */
	char *FNAME = strtoupper(fname);
	printf("#ifndef __T_%s_H__\n", FNAME);
	printf("#define __T_%s_H__\n", FNAME);
	printf("\n");

	/* Ready to go */
	if (!strcmp(type, "double") || !strcmp(type, "all")) {
		if (min == 0 && max == 0) {
			dlower = -DBL_MAX;
			dupper =  DBL_MAX;
		} else {
			dlower = min;
			dupper = max;
		}
		printf("const struct\nt1dentry {\n"
			    "\tdouble x;	/* Input */\n"
			    "\tdouble y;	/* Output */\n"
		    "} t1dtable[] = {\n");
		gen_double(fname, total, dlower, dupper);
		printf("};\n");
	}

	if (!strcmp(type, "ldouble") || !strcmp(type, "all")) {
		if (min == 0 && max == 0) {
			ldlower = -LDBL_MAX;
			ldupper =  LDBL_MAX;
		} else {
			ldlower = min;
			ldupper = max;
		}
                printf("const struct\nt1ldentry {\n"
		    "\tlong double x;   /* Input */\n"
		    "\tlong double y;   /* Output */\n"
                    "} t1ldtable[] = {\n");
		gen_ldouble(fname, total, ldlower, ldupper);
		printf("};\n");
	}

	/* Close ifdef guard */
	printf("#endif	/* __T_%s_H__ */\n", FNAME);
	free(FNAME);

	return (EXIT_SUCCESS);
}

static void
gen_double(const char *fname, size_t total, double lower, double upper)
{
	const struct fentry *f;
	mpfr_t mp_x, mp_y, mp_exact;
	double x, y, exact;
	size_t i;
	const mpfr_rnd_t tonearest = GMP_RNDN;

	assert(fname);
	f = getfunctionbyname(fname);
	assert(f);
	assert(f->f_narg == 1 || f->f_narg == 2);

	/* Initialize high precision variables */
	mpfr_init2(mp_x,     500);
	mpfr_init2(mp_y,     500);
	mpfr_init2(mp_exact, 500);

	for (i = 0; i < total; i++) {
		/*
		 * Generate random input
		 *
		 * A little bit suboptimal for the f_narg = 2 case, as we may be
		 * discarding -say- valid x values, because y was out of bounds.
		 */
		if (f->f_narg == 1) {
			do {
				x = random_double(FP_NORMAL);
			} while (x < lower || x > upper || !f->f_u.fp1(x));
		}
		if (f->f_narg == 2) {
			do {
				x = random_double(FP_NORMAL);
				y = random_double(FP_NORMAL);
			} while (x < lower || x > upper ||
				 y < lower || y > upper || !f->f_u.fp2(x, y));
		}

		/* Set the mpfr variables */
		if (f->f_narg >= 1)
			mpfr_set_d(mp_x, x, tonearest);
		if (f->f_narg == 2)
			mpfr_set_d(mp_y, y, tonearest);
		mpfr_set_d(mp_exact, 0.0, tonearest);

		/* Compute exact value */
		if (f->f_narg == 1)
			f->f_mpfr(mp_exact, mp_x, tonearest);
		else
			f->f_mpfr(mp_exact, mp_x, mp_y, tonearest);

		/* Extract exact value */
		exact = mpfr_get_d(mp_exact, tonearest);

		if (f->f_narg == 1)
			printf("\t{ % .16e, % .16e }", x, exact);
		else
			printf("\t{ % .16e,\n\t  % .16e,\n\t  % .16e }", x, y, exact);

		if (i < total - 1)
			printf(",\n");
		else
			printf("\n");
	}

	/* Free resources */
	mpfr_clear(mp_x);
	mpfr_clear(mp_y);
	mpfr_clear(mp_exact);
}

static void
gen_ldouble(const char *fname, size_t total,
    long double lower, long double upper)
{
	const struct fentry *f;
	mpfr_t mp_x, mp_y, mp_exact;
	long double x, y, exact;
	size_t i;
	const mpfr_rnd_t tonearest = GMP_RNDN;

	assert(fname);
	f = getfunctionbyname(fname);
	assert(f);
	assert(f->f_narg == 1 || f->f_narg == 2);

	/* Initialize high precision variables */
	mpfr_init2(mp_x,     500);
	mpfr_init2(mp_y,     500);
	mpfr_init2(mp_exact, 500);

	for (i = 0; i < total; i++) {
		/*
		 * Generate random input
		 *
		 * A little bit suboptimal for the f_narg = 2 case, as we may be
		 * discarding -say- valid x values, because y was out of bounds.
		 */
		if (f->f_narg == 1) {
			do {
				x = random_double(FP_NORMAL);
			} while (x < lower || x > upper || !f->f_u.fp1(x));
		}
		if (f->f_narg == 2) {
			do {
				x = random_double(FP_NORMAL);
				y = random_double(FP_NORMAL);
			} while (x < lower || x > upper ||
				 y < lower || y > upper || !f->f_u.fp2(x, y));
		}

		/* Set the mpfr variables */
		if (f->f_narg >= 1)
			mpfr_set_d(mp_x, x, tonearest);
		if (f->f_narg == 2)
			mpfr_set_d(mp_y, y, tonearest);
		mpfr_set_d(mp_exact, 0.0, tonearest);

		/* Compute exact value */
		if (f->f_narg == 1)
			f->f_mpfr(mp_exact, mp_x, tonearest);
		else
			f->f_mpfr(mp_exact, mp_x, mp_y, tonearest);

		/* Extract exact value */
		exact = mpfr_get_d(mp_exact, tonearest);

		if (f->f_narg == 1)
			printf("\t{ % .35Le, % .35Le }", x, exact);
		else
			printf("\t{ % .35Le, % .35Le,\n\t% .35Le }", x, y, exact);

		if (i < total - 1)
			printf(",\n");
		else
			printf("\n");
	}

	/* Free resources */
	mpfr_clear(mp_x);
	mpfr_clear(mp_y);
	mpfr_clear(mp_exact);
}

static void
usage(const char *progname)
{
	fprintf(stderr,
	    "usage: %s fname fptype -t total -m min -M max\n",
	    progname);
	fprintf(stderr,
		"\t`fname' is the function name, as declared in math.h\n");
	fprintf(stderr,
		"\t`fptype' is one of `float', `double' or `ldouble'\n");
	fprintf(stderr,
	    "\tIf `min'equals `max', then all possible range is assumed,\n"
	    "\te.g. [-DBL_MAX, DBL_MAX]\n");

	exit(EXIT_FAILURE);
}

/*
 * The caller must free up the memory
 */
static char *
strtoupper(const char *str)
{
	const char *oldp;
	char *newp, *newstr;

	assert(str);

	newstr = malloc(sizeof(str));
	assert(newstr);

	oldp = str;
	newp = newstr;
	do {
		*newp = toupper(*oldp);
		newp++;
	} while(*oldp++);

	return (newstr);
}
