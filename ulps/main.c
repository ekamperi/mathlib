#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen.h"
#include "subr_random.h"
#include "ulp.h"

static int isbanned(const char *fname, int argc, char *argv[]); 
static void usage(void);

int
main(int argc, char *argv[])
{
	struct ulp u;
	struct ulp_complex uc;
	int i, total, all, realonly, complexonly, ban, list;
	const char *target;
	const struct fentry *f;

	/* Skip program name */
	argv++;
	argc--;

	/*
	 * Parse command line arguments
	 * all  = probe all supported functions
	 * list = list  all supported functions
	 */
	all = 0;
	realonly = 0;
	complexonly = 0;
	list = 0;
	if (argc == 0) {
		usage();
		/* never reached */
	} else if (argc == 1 && !strcmp(argv[0], "all")) {
		all = 1;
		argv++; argc--;
	} else if (argc == 1 && !strcmp(argv[0], "real")) {
		all = realonly = 1;
		argv++; argc--;
	} else if (argc == 1 && !strcmp(argv[0], "complex")) {
		all = complexonly = 1;
		argv++; argc--;
	} else if (argc > 1 && !strcmp(argv[0], "ban")) {
		all = ban = 1;
		argv++; argc--;
	} else if (argc == 1 && !strcmp(argv[0], "list")) {
		list = 1;
		argv++; argc--;
	}

	if (list)
		printfunctions();

	/* Initialize random number generator */
	init_randgen();

	/* Print header */
	if (!list)
		printf("\tFUNCTION     Max ULP    Min ULP    Avg ULP    "
		    "skipped\n");

	total = all ? fsize : argc;

	for (i = 0; i < total; i++) {
		if (all) {
			f = getfunctionbyidx(i);
			if ((realonly    && !f->f_mpfr) ||
			    (complexonly &&  f->f_mpfr))
				continue;
			
			if (ban) {
				if (isbanned(f->f_name, argc, argv)) {
				  printf("[%2u/%2u] %-12s %s\n", i+1, total, f->f_name, "\t\t--- banned ---");
					continue;
				}
			}

			target = f->f_name;
		} else {
			target = argv[i];
		}

		/* Is it real or complex value function ? */
		f = getfunctionbyname(target);
		if (f == NULL) {
			fprintf(stderr, "function: %s not found\n",
			    argv[i]);
			continue;
		}

		/* Calculate ulp */
		if (f->f_mpfr)
			getfunctionulp_real(f, &u);
		else
			getfunctionulp_complex(f, &uc);

		if (f->f_mpfr) {
			printf("[%2u/%2u] %-12s ", i+1, total, target);
			printulps_double(u);
			if (all && f->f_libml_real) {
				printf("        %-12s ", f->f_namel);
				printulps_long_double(u);
			}
		} else {
			printf("[%2u/%2u] %-12s ", i+1, total, target);
			printulps_double_complex(uc);
		}
	}

	return EXIT_SUCCESS;
}

static int
isbanned(const char *fname, int argc, char *argv[])
{
	int i;

	for (i = 0; i < argc; i++) {
		if (!strcmp(fname, argv[i])) {
			return 1;
		}
	}

	return 0;
}

static void
usage(void)
{
	fprintf(stderr,
	    "ulps: [list | all | real | complex | "
	    "ban <function1> <function2> ...] | "
	    "<function1> <function2> ... \n");
	exit(EXIT_FAILURE);
}
