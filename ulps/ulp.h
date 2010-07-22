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

#define	ULP_INIT(u)					\
	assert(u);					\
	(u)->ulp_max = -DBL_MAX;			\
	(u)->ulp_min = DBL_MAX;				\
	(u)->ulp_avg = 0.0;				\
	(u)->ulp_skipped = 0;				\
	(u)->ulp_maxl = -LDBL_MAX;			\
	(u)->ulp_minl = LDBL_MAX;			\
	(u)->ulp_avgl = 0.0;				\
	(u)->ulp_skippedl = 0;				\

#define	ULP_UPDATE(u, myulp)			\
assert(u);					\
do {						\
	if (myulp > u->ulp_max)			\
		u->ulp_max = myulp;		\
	if (myulp < u->ulp_min)			\
		u->ulp_min = myulp;		\
	u->ulp_avg += myulp;			\
} while(0)

#define ULP_UPDATEL(u, myulpl)			\
assert(u);					\
do {						\
	if (myulpl > u->ulp_maxl)		\
		u->ulp_maxl = myulpl;		\
	if (myulpl < u->ulp_minl)		\
		u->ulp_minl = myulpl;		\
	u->ulp_avgl += myulpl;			\
} while(0)


int getfunctionulp(const char *fname, struct ulp *u);

#endif	/* ! __ULP_H__ */
