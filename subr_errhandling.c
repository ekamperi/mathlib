#include "config.h"
#include "subr_errhandling.h"

#include <assert.h>
#include <atf-c.h>
#include <errno.h>
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
 * support floating-point exceptions.
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
