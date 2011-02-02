#include <assert.h>
#include <math.h>
#include <mytypes.h>
#include <stdio.h>
#include <string.h>

#include "subr_random.h"

struct
stats {
	const char *st_type;
	size_t st_normals;
	size_t st_subnormals;
	size_t st_nans;
	size_t st_inf;
	size_t st_zero;
	size_t st_invalid;
};

#define DECL(type)						\
static void							\
probe_##type(struct stats *st, size_t N)			\
{								\
	type x;							\
	size_t i;						\
								\
	assert(st);						\
	memset(st, 0, sizeof(*st));				\
								\
	st->st_type = #type;					\
								\
	for (i = 0; i < N; i++) {				\
		x = random_##type(1);				\
		switch (fpclassify(x)) {			\
		case FP_NORMAL:					\
			++st->st_normals;			\
			break;					\
		case FP_SUBNORMAL:				\
			++st->st_subnormals;			\
			break;					\
		case FP_NAN:					\
			++st->st_nans;				\
			break;					\
		case FP_INFINITE:				\
			++st->st_inf;				\
			break;					\
		case FP_ZERO:					\
			++st->st_zero;				\
			break;					\
		default:					\
			/* invalid floating-point number */	\
			++st->st_invalid;			\
			break;					\
		}						\
	}							\
}

typedef long double long_double;
DECL(float);
DECL(double);
DECL(long_double);

static void
print_stats(const struct stats *st)
{
	assert(st);

	printf("---------- %s ----------\n", st->st_type);
	printf("  Normals: %u\n", st->st_normals);
        printf("Subnormals: %u\n", st->st_subnormals);
        printf("     NaNs: %u\n", st->st_nans);
        printf("      Inf: %u\n", st->st_inf);
        printf("     Zero: %u\n", st->st_zero);
	printf("  Invalid: %u\n", st->st_invalid);
	fflush(stdout);
}

int
main(void)
{
	struct stats s;
	const size_t N = 100*1000*1000;

	init_randgen();
#if 0
	/* floats */
	probe_float(&s, N);
	print_stats(&s);

	/* double */
        probe_double(&s, N);
        print_stats(&s);
#endif

	/* long double */
        probe_long_double(&s, N);
        print_stats(&s);

	return 0;
}
