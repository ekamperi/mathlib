#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen.h"
#include "subr_random.h"
#include "ulp.h"

int
main(int argc, char *argv[])
{
	struct ulp u;
	struct ulp_complex uc;
	int i, total, all, list;
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
	list = 0;
	if (argc == 1) {
		if (!strcmp(argv[0], "list")) {
			list = 1;
			argv++; argc--;
		} else if (!strcmp(argv[0], "all")) {
			all = 1;
			argv++; argc--;
		}
	}

	if (list)
		printfunctions();
	else {
		/* Initialize random number generator */
		init_randgen();

		/* Print header */
		printf("\tFUNCTION     Max ULP    Min ULP    Avg ULP    "
		       "skipped\n");

		total = all ? fsize : argc;

		for (i = 0; i < total; i++) {
			if (all) {
				f = getfunctionbyidx(i);
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
				getfunctionulp(f, &u);
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
	}

	return EXIT_SUCCESS;
}
