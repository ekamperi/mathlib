#include "config.h"
#include "subr_errhandling.h"

#include <assert.h>
#include <atf-c.h>
#include <errno.h>
#include <math.h>

#ifdef  HAS_MATH_ERREXCEPT
#include <fenv.h>
#endif  /* HAS_MATH_ERREXCEPT */

void
clear_exceptions(void)
{
#if defined(math_errhandling) && defined(MATH_ERREXCEPT)        \
        && (math_errhandling & MATH_ERREXCEPT)
	ATF_REQUIRE(feclearexcept(FE_ALL_EXCEPT) == 0);
#endif
}

void
clear_errno(void)
{
#if defined(math_errhandling) && defined(MATH_ERRNO)            \
        && (math_errhandling & MATH_ERRNO)
	errno = 0;
#endif
}

int
raised_exceptions(void)
{
	int raised = 0;

#if defined(math_errhandling) && defined(MATH_ERREXCEPT)        \
        && (math_errhandling & MATH_ERREXCEPT)

	raised = fetestexcept(FE_INVALID | FE_DIVBYZERO		\
	    | FE_OVERFLOW | FE_UNDERFLOW);
#endif

	/*
	 * XXX: Is it possible that some FE_* constant is
	 * defined as zero ?
	 */
	return (raised);
}

int
set_errno(void)
{
	int error = 0;
#if defined(math_errhandling) && defined(MATH_ERRNO)            \
        && (math_errhandling & MATH_ERRNO)
	error = errno;
#endif
	return (error);
}

void
query_errhandling(int *hasfp, int *haserrno)
{
	assert(hasfp);
	assert(haserrno);

#if defined(math_errhandling) && defined(MATH_ERREXCEPT)	\
	&& (math_errhandling & MATH_ERREXCEPT)
	*hasfp = 1;
#endif
#if defined(math_errhandling) && defined(MATH_ERRNO)		\
	&& (math_errhandling & MATH_ERRNO)
	*haserrno = 1;
#endif
}
