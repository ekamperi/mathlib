#define	_XOPEN_SOURCE	600

#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "subr_random.h"

/* Function prototypes */
static int isvalidfp_vax(const uint32_t *y);
static int isvalidfp_ldouble(const uint32_t *y);

#define	MY_RANDOM(x)	mrand48()

#ifdef	__vax__
#define	ISVALIDFP(x)	isvalidfp_vax(x)
#else
#define	ISVALIDFP(x)	isvalidfp_ldouble(x)	/* Always valid */
#endif

static int
isvalidfp_vax(const uint32_t *y)
{
	/*
	 * A reserved operand exception occurs if a source operand of a
	 * floating instruction has a sign of one and a biased exponent of
	 * zero.
	 *
	 * Every operation that involves a reserved operand, let it be as
	 * simple as a MOV operation, will result in SIGILL delivered to us.
	 */
#define SIGNMASK_BIT 0x8000     /* 1000000000000000 (15   bit) */
#define BEXPMASK_BIT 0x7F80     /* 0111111110000000 (14-7 bits) */

	if (y[0] & SIGNMASK_BIT) {
		if ((y[0] & BEXPMASK_BIT) == 0) {
			return 0;
		}
	}

	return 1;
}

static int
isvalidfp_ldouble(const uint32_t *y)
{
	/* XXX: Allow subnormals */
	/* It's subnormal */
	if ((y[1] & 0x80000000) == 0)
		return 0;

	return 1;
}

/*
 * Floating-point numbers are more dense the closer we get to zero, and
 * more sparse as we head towards infinities. This has to do with how they
 * are represented in machine format.
 *
 * That said, the following functions do NOT constitute a uniform distribution
 * of floating-point numbers. They are biased towards zero. If you want
 * equally distributed numbers, you may want drand48() or you should roll
 * out your own  with rand().
 */
#define DECL(TYPE)							\
TYPE									\
random_##TYPE(int fpclass)						\
{									\
	size_t i, nbytes;						\
									\
	nbytes = sizeof(TYPE) / sizeof(uint32_t);			\
	union {								\
		TYPE x;							\
		uint32_t y[nbytes];					\
	} u;								\
									\
	do {								\
		for (i = 0; i < nbytes; i++)				\
			u.y[i] = MY_RANDOM();				\
	} while (!ISVALIDFP(u.y) || fpclassify(u.x) != fpclass);	\
									\
	return (u.x);							\
}

DECL(float)
DECL(double)
typedef long double long_double;
DECL(long_double)

void
init_randgen(void)
{
	srand48(time(NULL));
}
