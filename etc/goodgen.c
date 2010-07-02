/*
 * The purpose of this utility is to generate valid C arrays of exact
 * (x, ..., func(x, ...)) pairs for consumption by the rest of the test cases.
 *
 * The code is a bit ugly. Sorry, tried to do it as less ugly as possible.
 */
#define _XOPEN_SOURCE 600

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <gmp.h>
#include <mpfr.h>

#include "gen.h"
#include "subr_random.h"

/* Function prototypes */
#define DECL_GEN(type)					\
static void gen_##type(const char *fname, size_t total, \
		       type lower, type upper)
typedef long double long_double;
DECL_GEN(double);
DECL_GEN(long_double);
static void usage(const char *progname);
static char *strtoupper(const char *str);
static long double mystrtold(const char *str, const char *what);

/*
 * Bite the bullet and don't protect macro arguments with surrounding
 * parentheses nor use intermediate variables. Trade safety, for pleasure.
 */
#define	COMPUTE_EXACT_VAL(f, mp_exact, mp_x, mp_y, rndmode)		\
do {									\
	if (f->f_narg == 1)						\
		f->f_mpfr(mp_exact, mp_x, rndmode);			\
	else								\
		f->f_mpfr(mp_exact, mp_x, mp_y, rndmode);		\
} while(0)

/*
 * A little bit suboptimal for the f_narg = 2 case, as we may be
 * discarding -say- valid x values, because y was out of bounds.
 */
#define GET_RANDOM_VAL(type, f, x, y)					\
do {									\
	if (f->f_narg == 1) {						\
		do {							\
			x = random_##type(FP_NORMAL);			\
		} while (x < lower || x > upper || !f->f_u.fp1(x));	\
	}								\
									\
	if (f->f_narg == 2) {						\
		do {							\
			x = random_##type(FP_NORMAL);			\
			y = random_##type(FP_NORMAL);			\
		} while (x < lower || x > upper ||			\
			 y < lower || y > upper || !f->f_u.fp2(x, y));	\
	}								\
} while(0)

int
main(int argc, char *argv[])
{
	const char *progname, *fname, *type;
	long double min, max;
        long double ldlower, ldupper;
	double dlower, dupper;
        int total, opt;

	/*
	 * Save program name, function name and floating-point type.
	 * We will be using them later.
	 */
	progname = argv[0];
	fname = argv[1];
	type = argv[2];
	optind += 2;

	/* Parse arguments */
	total = -1;
	while ((opt = getopt(argc, argv, "m:M:t:")) != -1) {
		switch (opt) {
		case 'm':
			min = mystrtold(optarg, "min");
			break;
		case 'M':
			max = mystrtold(optarg, "Max");
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
		usage(progname);
		/* NEVER REACHED */
	}

	/* Validate input -- make sure all arguments were given */
	if (total < 0 || min > max || argc != 6) {
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

	/* Open #ifdef guard */
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
		gen_long_double(fname, total, ldlower, ldupper);
		printf("};\n");
	}

	/* Close #ifdef guard */
	printf("#endif	/* ! __T_%s_H__ */\n", FNAME);
	free(FNAME);

	return (EXIT_SUCCESS);
}

static void
gen_double(const char *fname, size_t total, double lower, double upper)
{
	const struct fentry *f;
	const mpfr_rnd_t tonearest = GMP_RNDN;
	mpfr_t mp_x, mp_y, mp_exact;
	double x, y, exact;
	size_t i;

	/* Look up the function */
	f = getfunctionbyname(fname);
	assert(f);

	/* Initialize high precision variables */
	mpfr_inits2(500, mp_x, mp_y, mp_exact, NULL);

	for (i = 0; i < total; i++) {
		/* Generate a random input for function f */
		GET_RANDOM_VAL(double, f, x, y);

		/* Set the mpfr variables */
		if (f->f_narg >= 1)
			mpfr_set_d(mp_x, x, tonearest);
		if (f->f_narg == 2)
			mpfr_set_d(mp_y, y, tonearest);
		mpfr_set_d(mp_exact, 0.0, tonearest);

		/* Compute exact value, mp_exact = f(mp_x, ...)  */
		COMPUTE_EXACT_VAL(f, mp_exact, mp_x, mp_y, tonearest);

		/* Extract exact value */
		exact = mpfr_get_d(mp_exact, tonearest);

		/* Skip infinities, NAN, etc */
		if (isinf(exact) || isnan(exact)) {
			i--;
			continue;
		}

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
	mpfr_clears(mp_x, mp_y, mp_exact, NULL);
}

static void
gen_long_double(const char *fname, size_t total,
    long double lower, long double upper)
{
	const struct fentry *f;
	const mpfr_rnd_t tonearest = GMP_RNDN;
	mpfr_t mp_x, mp_y, mp_exact;
	long double x, y, exact;
	size_t i;

	/* Lookup the function */
	f = getfunctionbyname(fname);
	assert(f);

	/* Initialize high precision variables */
	mpfr_inits2(500, mp_x, mp_y, mp_exact, NULL);

	for (i = 0; i < total; i++) {
		/* Generate a random input for function f */
		GET_RANDOM_VAL(long_double, f, x, y);

		/* Set the mpfr variables */
		if (f->f_narg >= 1)
			mpfr_set_ld(mp_x, x, tonearest);
		if (f->f_narg == 2)
			mpfr_set_ld(mp_y, y, tonearest);
		mpfr_set_ld(mp_exact, 0.0, tonearest);

		/* Compute exact value, mp_exact = f(mp_x, ...) */
		COMPUTE_EXACT_VAL(f, mp_exact, mp_x, mp_y, tonearest);

		/* Extract exact value */
		exact = mpfr_get_ld(mp_exact, tonearest);

		/* Skip infinities, NAN, etc */
		if (isinf(exact) || isnan(exact)) {
			i--;
			continue;
		}

		/*
		 * Don't forget the L suffix in long double floating-point
		 * constants, as by default they are treated as double.
		 */
		if (f->f_narg == 1)
			printf("\t{ % .35LeL, % .35LeL }", x, exact);
		else
			printf("\t{ % .35LeL, % .35LeL,\n\t% .35LeL }", x, y, exact);

		if (i < total - 1)
			printf(",\n");
		else
			printf("\n");
	}

	/* Free resources */
	mpfr_clears(mp_x, mp_y, mp_exact, NULL);
}

static void
usage(const char *progname)
{
	fprintf(stderr,
	    "usage: %s fname fptype -t total -m min -M Max\n"
	    "\t`fname' is the function name, as declared in math.h\n"
	    "\t`fptype' is one of `double', `ldouble' or `all'\n"
	    "If `min' equals `Max', then all possible range is assumed,\n"
	    "e.g. [-DBL_MAX, DBL_MAX]\n",
	    progname);

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

static long double
mystrtold(const char *str, const char *what)
{
	long double rv;
	char *endp;

	errno = 0;

	rv = strtold(str, &endp);

	if (*endp)
		fprintf(stderr,
		    "WARNING: Conversion stopped at %s (was given = %s)\n"
		    "WARNING: Continuing with %s = %.35Le\n",
		    endp, str, what, rv);

	/* We only allow over/under-flows to happen (ERANGE) */
	if (errno) {
		perror("strtold");
		if (errno != ERANGE)
			exit(EXIT_FAILURE);
	}

	return (rv);
}
