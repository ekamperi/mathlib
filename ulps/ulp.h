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

struct ulp_complex {
	struct ulp ulp_real;
	struct ulp ulp_imag;
	struct ulp ulp_total;
};

/*******************************************************************************
 *				Real arithmetic
 ******************************************************************************/
#define	ULP_INIT(u)				\
	assert(u);				\
	(u)->ulp_max	  = -DBL_MAX;		\
	(u)->ulp_min	  =  DBL_MAX;		\
	(u)->ulp_avg	  =  0.0;		\
	(u)->ulp_skipped  =  0;			\
	(u)->ulp_maxl	  = -LDBL_MAX;		\
	(u)->ulp_minl	  =  LDBL_MAX;		\
	(u)->ulp_avgl	  =  0.0;		\
	(u)->ulp_skippedl =  0;

#define	ULP_UPDATE(u, n)			\
assert(u);					\
do {						\
	if ((n) > (u)->ulp_max)			\
		  (u)->ulp_max = n;		\
	if ((n) < (u)->ulp_min)			\
		  (u)->ulp_min = n;		\
	(u)->ulp_avg += n;			\
} while(0)

#define ULP_UPDATEL(u, nl)			\
assert(u);					\
do {						\
	if ((nl) > (u)->ulp_maxl)		\
		   (u)->ulp_maxl = nl;		\
	if ((nl) < (u)->ulp_minl)		\
		   (u)->ulp_minl = nl;		\
	(u)->ulp_avgl += nl;			\
} while(0)


/*******************************************************************************
 *				Complex arithmetic
 ******************************************************************************/
#define ULP_COMPLEX_INIT(uc)						\
	ULP_INIT(&uc->ulp_real);					\
	ULP_INIT(&uc->ulp_imag);					\
	ULP_INIT(&uc->ulp_total);

#define ULP_COMPLEX_UPDATE(uc, n)					\
assert(uc);								\
ULP_UPDATE(&uc->ulp_real, creal(n));					\
ULP_UPDATE(&uc->ulp_imag, cimag(n));					\
ULP_UPDATE(&uc->ulp_total, fabs(creal(n)) + fabs(cimag(n)));

#define ULP_COMPLEX_UPDATEL(uc, n)					\
assert(uc);								\
ULP_UPDATEL(&uc->ulp_real, creall(n));					\
ULP_UPDATEL(&uc->ulp_imag, cimagl(n));					\
ULP_UPDATEL(&uc->ulp_total, fabsl(creall(n)) + fabsl(cimagl(n)));

double calculp_double(double computed, double exact);
long double calculp_long_double(long double computedl, long double exactl);

void getfunctionulp_real(const struct fentry *f, struct ulp *u);
void getfunctionulp_complex(const struct fentry *f, struct ulp_complex *u);

void printulps_double(struct ulp u);
void printulps_long_double(struct ulp u);
void printulps_double_complex(struct ulp_complex u);
void printulps_long_double_complex(struct ulp_complex u);

#define	NITERATIONS (100 * 1000)

#endif	/* ! __ULP_H__ */
