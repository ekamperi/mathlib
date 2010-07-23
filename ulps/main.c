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
	int i, rv, total, all;
	const char *target;

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

	total = all ? fsize : argc;

	for (i = 0; i < total; i++) {
		target = all ? ftable[i].f_name : argv[i];
		rv = getfunctionulp(target, &u);
		if (rv != -1) {
			printf("[%2u/%2u] function: %-8s ", i+1, total, target);
			printulps(u);
			if (ftable[i].f_libm) {
				printf("[%2u/%2u] function: %s%-7s ",
				    i+1, total, "l", target);
				printulps_long_double(u);
			}
		} else {
			fprintf(stderr, "function: %s not found\n", argv[i]);
			continue;
		}
	}

	return EXIT_SUCCESS;
}
