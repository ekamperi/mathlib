#include "config.h"
#include "subr_errhandling.h"

#include <assert.h>
#include <atf-c.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>

#ifdef  HAVE_FENV_H
#include <fenv.h>
#endif

void
clear_exceptions(void)
{
#if defined(math_errhandling) && defined(MATH_ERREXCEPT)        \
	&& (math_errhandling & MATH_ERREXCEPT)
	ATF_REQUIRE(feclearexcept(FE_ALL_EXCEPT) == 0);
#endif
}

/*
 * Checks if `excepts' were raised.
 *
 * If the implementation doesn't support floating-point exceptions, we silently
 * skip this check. The consumer must first have checked that either errno or
 * floating-point exceptions is supported in the host environment.
 *
 * This allows us to write from the test case, for example:
 * ATF_CHECK(raised_exceptions(FE_INVALID));
 *
 * instead of:
 * if (haserrexcept)
 *	ATF_CHECK(raised_exceptions(FE_INVALID));
 *
 * but we CAN'T write:
 * ATF_CHECK(!raised_exceptions(FE_INVALID)), as this will fail if host doesn't
 * support floating-point exceptions. Use not_raised_exceptions() instead.
 */
int
raised_exceptions(int excepts)
{
	int raised;

#if defined(math_errhandling) && defined(MATH_ERREXCEPT)	\
	&& (math_errhandling & MATH_ERREXCEPT)

	raised = fetestexcept(excepts);
	return (raised & excepts);
#endif

	return (1);
}

int
not_raised_exceptions(int excepts)
{
	int raised;

#if defined(math_errhandling) && defined(MATH_ERREXCEPT)        \
	&& (math_errhandling & MATH_ERREXCEPT)

	raised = fetestexcept(excepts);
	return ((raised & excepts) == 0);
#endif

	return (1);
}

/*
 * Checks if `errno' is equal to `error'.
 *
 * If the implementation doesn't support errno, we silently skip this check.
 * The consumer must first have checked that either errno or floating-point
 * exceptions is supported in the host environment.
 */
int
iserrno_equalto(int error)
{
#if defined(math_errhandling) && defined(MATH_ERRNO)		\
	&& (math_errhandling & MATH_ERRNO)
	return (error == errno);
#endif

	return (1);
}

void
query_errhandling(int *hasfp, int *haserrno)
{
	assert(hasfp);
	assert(haserrno);

	/* Initialize values */
	*hasfp = 0;
	*haserrno = 0;

#if defined(math_errhandling) && defined(MATH_ERREXCEPT)	\
	&& (math_errhandling & MATH_ERREXCEPT)
	*hasfp = 1;
#endif
#if defined(math_errhandling) && defined(MATH_ERRNO)		\
	&& (math_errhandling & MATH_ERRNO)
	*haserrno = 1;
#endif
}

/*******************************************************************************
 * This is used for debugging purposes
 ******************************************************************************/
static const
struct fpeentry {
	const char *fpe_name;
	const int fpe_val;
} fpetable[] = {
#ifdef	FE_DIVBYZERO
	{ "FE_DIVBYZERO", FE_DIVBYZERO	},
#endif
#ifdef	FE_DENORMAL
	{ "FE_DENORMAL ", FE_DENORMAL	},
#endif
#ifdef	FE_INEXACT
	{ "FE_INEXACT  ", FE_INEXACT	},
#endif
#ifdef	FE_INVALID
	{ "FE_INVALID  ", FE_INVALID	},
#endif
#ifdef	FE_OVERFLOW
	{ "FE_OVERFLOW ", FE_OVERFLOW	},
#endif
#ifdef	FE_UNDERFLOW
	{ "FE_UNDERFLOW", FE_UNDERFLOW	},
#endif
	{ NULL, -1 }
};

void
print_exceptions(void)
{
#if defined(HAVE_FENV_H)					\
	&& defined(math_errhandling)				\
	&& defined(MATH_ERREXCEPT)				\
	&& (math_errhandling & MATH_ERREXCEPT)

	const struct fpeentry *f;
	fenv_t env;
	int ex;

	/*
	 * Store current floating-point environment, since printf() tends
	 * to taint it in some platforms (like NetBSD), as opposed to
	 * others that try to preserve it where possible (e.g., FreeBSD).
	 */
	assert(fegetenv(&env) == 0);

	ex = fetestexcept(FE_ALL_EXCEPT);
	for (f = fpetable; f; f++) {
		if (f->fpe_name == NULL && f->fpe_val == -1)
			break;

		printf("%-12s: %s\n",
		    f->fpe_name,
		    (ex & f->fpe_val) ? "+SET+" : "UNSET");
	}

	/* Restore floating-point environment */
	assert(fesetenv(&env) == 0);
#else
	printf("Host OS does not support floating-point exceptions\n");
#endif
}
