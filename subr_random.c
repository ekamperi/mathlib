#define	_XOPEN_SOURCE	600

#include <assert.h>
#include <complex.h>
#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "subr_random.h"

#define	MY_RANDOM(x)	mrand48()

#if	defined(__vax__)
#define	ISVALIDFP(x)	isvalidfp_vax(x)
#elif	defined(__i386__) || defined(__x86_64__)
#define	ISVALIDFP(x)	isvalidfp_intel80bit(x)
#else
#define	ISVALIDFP(x)	1	/* Always valid, until proven otherwise */
#endif

#undef	DEBUG
#ifdef  DEBUG
#define	DPRINTF(x)      do { printf x fflush(NULL); } while(0);
#else
#define	DPRINTF(x)
#endif

int
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

/*
 * Intel Architecture, Software Developer’s Manual, Volume 1:
 * Basic Architecture,
 *
 * 7.2.2. Floating-Point Format,
 * 7.4.1 Real Numbers, Table 7-9. Real Number and NaN encodings
 */
int
isvalidfp_intel80bit(const uint32_t *y)
{
	int intbit;
	int e;

	intbit = y[1] & 0x80000000;
	e      = y[2] & 0x7FFF;

	/* Infinity */
	if (e == 0x7FFF) {
		if ((y[0] == 0) && (y[1] == 0x80000000)) {
			DPRINTF(("-> infinity\n"));
			return 1;
		}
	}

	/* Normal */
	if ((e > 0) && (e < 0xFFFE)) {
		if (intbit) {
			DPRINTF(("-> normal\n"));
			return 1;
		}
	}

	/* Denormal */
	if (e == 0) {
		if (intbit == 0) {
			if (y[0] || y[1]) {
				DPRINTF(("-> denormal\n"));
				return 1;
			}
		}
	}

	/* Zero */
	if (e == 0) {
		if ((y[0] == 0) && (y[1] == 0)) {
			DPRINTF(("-> zero\n"));
			return 1;
		}
	}

	/* NaNs */
	if (e == 0x7FFF) {
		if (intbit) {
			if ((y[1] & 0x40000000) == 0) {
				if (y[0] || (y[1] & 0x7FFFFFFF)) {
					DPRINTF(("-> SNaN\n"));
					return 1;
				}
			} else {
				if ((y[0] == 0) && (y[1] == 0xC0000000)) {
					DPRINTF(("-> Real indefinite QNaN\n"));
					return 1;
				} else {
					DPRINTF(("-> QNaN\n"));
					return 1;
				}
			}
		}
	}

	DPRINTF(("-> INVALID\n"));
	return 0;
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
 *
 * The formats we are studying (96bits, 128bits) are 4-byte aligned,
 * so uin32_t is fine for the moment.
 */
long double
random_long_double(int fpclass)
{
#define	NBYTES	(sizeof(long double) / sizeof(uint32_t))
	size_t i;

	union {
		long double x;
		uint32_t y[NBYTES];
	} u;

	do {
		for (i = 0; i < NBYTES; i++)
			u.y[i] = MY_RANDOM();
	} while (!ISVALIDFP(u.y) || fpclassify(u.x) != fpclass);

	return (u.x);
}

/*
 * Really cheap hack to avoid deploying validation functions for floats
 * and doubles. The price is that it slows down things A LOT, due to normal
 * long double numbers being converted to NaNs, Inf and subnormals at the
 * time of the cast. XXX: Could we take advantage of that ?
 */
double
random_double(int fpclass)
{
	double x;

	do {
		x = (double)random_long_double(fpclass);
	} while (fpclassify(x) != fpclass);

	return (x);
}

float
random_float(int fpclass)
{
	float x;

	do {
		x = (float)random_long_double(fpclass);
	} while (fpclassify(x) != fpclass);

	return (x);
}

/*******************************************************************************
 *				Complex Numbers				       *
 ******************************************************************************/

/*
 * Solaris doesn't provide the `_Imaginary' datatype,
 * so we need to workaround it.
 *
 * ISO/IEC 9899:TC2 - 6.2.5.13 Types
 * Each complex type has the same representation and alignment requirements as
 * an array type containing exactly two elements of the corresponding real type;
 * the first element is equal to the real part, and the second element to the
 * imaginary part, of the complex number.
 */

long double complex
random_long_double_complex(int fpclass)
{
	long double x, y;

	x = random_long_double(fpclass);
	y = random_long_double(fpclass);

#if defined(__sun__)
	union {
		long double complex z;
		long double a[2];
	} u = { .a = {x, y}};
	return (u.z);
#else
	return (x + y*I);
#endif
}

double complex
random_double_complex(int fpclass)
{
	double x, y;

	x = random_double(fpclass);
	y = random_double(fpclass);

#if defined(__sun__)
	union {
		double complex z;
		double a[2];
	} u = { .a = {x, y}};
	return (u.z);
#else
	return (x + y*I);
#endif
}

float complex
random_float_complex(int fpclass)
{
	float x, y;

	x = random_float(fpclass);
	y = random_float(fpclass);

#if defined(__sun__)
	union {
		long double complex z;
		long double a[2];
	} u = { .a = {x, y}};
	return (u.z);
#else
	return (x + y*I);
#endif
}

void
init_randgen(void)
{
	srand48(time(NULL));
}
