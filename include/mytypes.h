#ifndef	__MY_TYPES_H__
#define	__MY_TYPES_H__

#include <complex.h>

/*
 * Quoting from ISO/IEC 9899:TC2:
 *
 * 6.2.5.13 Types
 * Each complex type has the same representation and alignment requirements as
 * an array type containing exactly two elements of the corresponding real type;
 * the first element is equal to the real part, and the second element to the
 * imaginary part, of the complex number.
 */
typedef union {
	float complex z;
	float parts[2];
} float_complex;

typedef union {
	double complex z;
	double parts[2];
} double_complex;

typedef union {
	long double complex z;
	long double parts[2];
} long_double_complex;

/*
 * The FP_* macros shall be defined for number classification. They represent
 * the mutually-exclusive kinds of floating-point values. They expand to integer
 * constant expressions with distinct values.
 *
 * Unfortunately, the standard doesn't have a requirement for the values to be
 * _bitwise_ exclusive. So we can't safely assume that e.g.
 * FP_NORMAL | FP_SUBNORMAL will be distinct from FP_ZERO. For this reason we
 * provide wrappers that are guaranteed to be bitwise distinct.
 */
#define	MY_FP_INFINITE	0x01
#define	MY_FP_NAN	0x02
#define	MY_FP_NORMAL	0x04
#define	MY_FP_SUBNORMAL	0x08
#define	MY_FP_ZERO	0x10

#define	MY_FP_REAL (MY_FP_NORMAL | MY_FP_SUBNORMAL | MY_FP_ZERO)

#endif	/* ! __MY_TYPES__ */
