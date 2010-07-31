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

	/* Print header */
	printf("\tFUNCTION     Max ULP    Min ULP    Avg ULP    skipped\n");

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
                        fprintf(stderr, "function: %s not found\n", argv[i]);
                        continue;
		}

		/* Calculate ulp */
		if (f->f_mpfr)
			rv = getfunctionulp(f, &u);
		else
			rv = getfunctionulp_complex(f, &uc);

		if (f->f_mpfr) {
			printf("[%2u/%2u] %-12s ", i+1, total, target);
			printulps(u);
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
