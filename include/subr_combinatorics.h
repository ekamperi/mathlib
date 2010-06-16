#ifndef	__SUBR_COMBINATORICS_H__
#define	__SUBR_COMBINATORICS_H__

int subset_combine(const int *set, int nelms, int mth);
int num_of_combos(int nelms);

#define FOREACH_BITWISE_COMBO(table, nelms, mth, comb)			 \
	for (*(mth) = 1, *(comb) = subset_combine(table, nelms, *(mth)); \
	     *(mth) < num_of_combos(nelms) + 1;				 \
	     *(comb) = subset_combine(table, nelms, *(mth)), (*(mth))++)

#endif	/* ! __SUBR_COMBINATORICS_H__ */
