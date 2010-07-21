#ifndef __ULPH_H__
#define __ULPH_H__

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
	(u)->ulp_max = -DBL_MAX;			\
	(u)->ulp_min = DBL_MAX;				\
	(u)->ulp_avg = 0.0;				\
	(u)->ulp_skipped = 0;				\
	(u)->ulp_maxl = -LDBL_MAX;			\
	(u)->ulp_minl = LDBL_MAX;			\
	(u)->ulp_avgl = 0.0;				\
	(u)->ulp_skippedl = 0;				\


int getfunctionulp(const char *fname, struct ulp *u);

#endif	/* ! __ULP_H__ */
