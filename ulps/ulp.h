#ifndef __ULPH_H__
#define __ULPH_H__

#include <assert.h>

struct ulp {
	double ulp_max;
	double ulp_min;
	double ulp_avg;
	size_t ulp_skipped;

	long double ulp_maxl;
	long double ulp_minl;
	long double ulp_avgl;
	size_t ulp_skippedl;
};

/*
 * As usual, bite the bullet and don't surround macro arguments with
 * parentheses. Nor put any intermediate variables. Trade security,
 * for beauty. Until it bites as back ;)
 */
#define	ULP_INIT(u)				\
	assert(u);				\
	u->ulp_max = -DBL_MAX;			\
	u->ulp_min = DBL_MAX;			\
	u->ulp_avg = 0.0;			\
	u->ulp_skipped = 0;			\
	u->ulp_maxl = -LDBL_MAX;		\
	u->ulp_minl = LDBL_MAX;			\
	u->ulp_avgl = 0.0;			\
	u->ulp_skippedl = 0;			\

#define	ULP_UPDATE(u, n)			\
assert(u);					\
do {						\
	if (n > u->ulp_max)			\
		u->ulp_max = n;			\
	if (n < u->ulp_min)			\
		u->ulp_min = n;			\
	u->ulp_avg += n;			\
} while(0)

#define ULP_UPDATEL(u, nl)			\
assert(u);					\
do {						\
	if (nl > u->ulp_maxl)			\
		u->ulp_maxl = nl;		\
	if (nl < u->ulp_minl)			\
		u->ulp_minl = nl;		\
	u->ulp_avgl += nl;			\
} while(0)

int getfunctionulp(const char *fname, struct ulp *u);

#endif	/* ! __ULP_H__ */
