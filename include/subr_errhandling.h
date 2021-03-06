#ifndef	__SUBR_ERRHANDLING_H__
#define	__SUBR_ERRHANDLING_H__

#include "config.h"

#ifdef  HAVE_FENV_H
#include <fenv.h>
#endif

/* Floating point exception constant is missing. */
#define FE_IS_MISSING	(-1)

#ifdef	FE_INVALID
#define	MY_FE_INVALID	FE_INVALID
#else
#define MY_FE_INVALID	FE_IS_MISSING
#endif

#ifdef	FE_DIVBYZERO
#define	MY_FE_DIVBYZERO	FE_DIVBYZERO
#else
#define MY_FE_DIVBYZERO	FE_IS_MISSING
#endif

#ifdef	FE_OVERFLOW
#define MY_FE_OVERFLOW	FE_OVERFLOW
#else
#define MY_FE_OVERFLOW	FE_IS_MISSING
#endif

#ifdef	FE_UNDERFLOW
#define MY_FE_UNDERFLOW	FE_UNDERFLOW
#else
#define MY_FE_UNDERFLOW	FE_IS_MISSING
#endif

#ifdef	FE_ALL_EXCEPT
#define	MY_FE_ALL_EXCEPT	FE_ALL_EXCEPT
#else
#define	MY_FE_ALL_EXCEPT	FE_IS_MISSING
#endif

/* Function prototypes */
void clear_exceptions(void);
void clear_errno(void);

int raised_exceptions(int excepts);
int not_raised_exceptions(int excepts);

int errno_equalto(int error);
int not_errno_equalto(int error);

void query_errhandling(int *hasfp, int *haserrno);

void print_exceptions(void);

#endif	/* ! __SUBR_ERRHANDLING_H__ */
