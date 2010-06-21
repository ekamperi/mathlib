#ifndef __ULPH_H__
#define __ULPH_H__

struct ulp {
	double ulp_max;
	double ulp_min;
	double ulp_avg;
	size_t ulp_skipped;
};

int getfunctionulp(const char *fname, struct ulp *u);

#endif	/* ! __ULP_H__ */
