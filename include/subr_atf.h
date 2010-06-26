#ifndef	__SUBR_ATF_H__
#define	__SUBR_ATF_H__

#include <atf-c.h>
#include <math.h>

bool get_config_var_as_bool(const atf_tc_t *tc, const char *name);
long get_config_var_as_long(const atf_tc_t *tc, const char *name);

#define ATF_PRINT_ERRMSG(fmt, ...)					\
do {									\
	atf_tc_fail_check(__FILE__, __LINE__, fmt, ##__VA_ARGS__);	\
} while(0)

/*
 * We always check the value of `__cond' before updating it, because
 * one good deed cannot make a bad act even. E.g.,
 *
 * ATF_FOR_LOOP(i, N, i++) {
 *	ATF_PASS_OR_BREAK(... false ...);
 *	ATF_PASS_OR_BREAK(... true  ...);
 * }
 *
 * Also, we only print an error message if the current expression (x)
 * is false, not if some past one (__cond) were. Otherwise, the latter
 * would print twice the same error.
 */
#define ATF_PASS_OR_BREAK(x)			\
do {						\
	__cond = __cond ? (x) : 0;		\
	if (!(x))				\
		ATF_PRINT_ERRMSG(#x);		\
} while(0)

/* We can have only one such loop within a function */
#define ATF_FOR_LOOP(i, end, incr)		\
int __cond = 1;					\
for (i = 0; i < (end) && __cond; incr)

/*
 * If host doesn't support NaNs, quietly return 1 as if NaNs were supported
 * and the argument was indeed NaN. This effectively disables the check.
 */
#ifdef	NAN
#define ATF_CHECK_IFNAN(x)	ATF_CHECK(isnan(x))
#else
#define ATF_CHECK_IFNAN(x)	1
#endif

#endif	/* ! __SUBR_ATF_H__ */
