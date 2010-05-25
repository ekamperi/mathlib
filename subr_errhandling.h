#ifndef	__SUBR_ERRHANDLING_H__
#define	__SUBR_ERRHANDLING_H__

void clear_exceptions(void);
void clear_errno(void);

int raised_exceptions(void);
int set_errno(void);

void query_errhandling(int *hasfp, int *haserrno);

#endif	/* ! __SUBR_ERRHANDLING_H__ */
