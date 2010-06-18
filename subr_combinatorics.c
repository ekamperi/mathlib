#include <stdio.h>
#include <stdlib.h>

#include "include/subr_combinatorics.h"

/*
 * Returns the m-th subset of a set of size `nelms'.
 *
 * The subset is returned as a sequence of 0, 1.
 * 0 means that the respective element is omitted and 1 that is included.
 *
 * Example:
 * set		  = { A, B, C, D }
 * subset	  = { 0, 0, 1, 1 }
 * corresponds to = {       C, D }
 */
static int *
subset_get(int nelms, int mth)
{
	int *ret;
	int i;

	ret = malloc(nelms * sizeof(int));
	if (ret == NULL)
		return (NULL);

	for (i = 0; i < nelms; i++)
		ret[i] = (mth & (1 << i)) ? 1 : 0;

	return (ret);
}

static void
subset_rel(int *s)
{
	free(s);
}

static void
subset_print(const int *s, int nelms)
{
	int i;

	printf("{");
	for (i = 0; i < nelms; i++) {
		printf("%d", s[i]);
		if (i != (nelms - 1))
		    printf(", ");
	}

	printf("}");
}

/*
 * From a set `set' with `nelms' elements, return the m-th (out of 2^nelms - 1)
 * bitwise combination.
 */
int
subset_combine(const int *set, int nelms, int mth)
{
	int i, ret;
	int *sub;

	sub = subset_get(nelms, mth);
	if (sub == NULL)
		return (-1);

	ret = 0;
	for (i = 0; i < nelms; i++) {
		ret |= sub[i] ? set[i] : ret;
	}

	subset_rel(sub);

	return (ret);
}

/*
 * Return the number of non-empty subsets of a set of size `nelms'
 *              C(n, 1) + C(n, 2) + ... + C(n, n)
 * which equals
 *                      2^n - 1
 */
int
num_of_combos(int nelms)
{
	return ((1 << nelms) - 1);
}
