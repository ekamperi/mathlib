#ifndef	__SUBR_RANDOM_H__
#define	__SUBR_RANDOM_H__

void init_randgen(void);

float random_float(int class);
double random_double(int class);
long double random_long_double(int class);

/* We export these functions, only to be used by self tests */
int isvalidfp_vax(const uint32_t *y);
int isvalidfp_ldouble(const uint32_t *y);

#endif	/* ! __SUBR_RANDOM_H__ */
