#ifndef	__SUBR_ATF_H__
#define	__SUBR_ATF_H__

#include <atf-c.h>

bool get_config_var_as_bool(const atf_tc_t *tc, const char *name);
long get_config_var_as_long(const atf_tc_t *tc, const char *name);

#define ATF_PRINT_ERRMSG(fmt, ...)					\
do {									\
	atf_tc_fail_check(__FILE__, __LINE__, fmt, ##__VA_ARGS__);	\
} while(0)

/* Hacky, ugly, fragile. But it gets the job done */
#define ATF_PASS_OR_BREAK(x)			\
do {						\
	__cond = __cond ? (x) : 0;		\
	if (!__cond)				\
		ATF_PRINT_ERRMSG(#x);		\
} while(0)

#define ATF_FOR_LOOP(i, end, incr)		\
int __cond = 1;					\
for (i = 0; i < (end) && __cond; incr)

#endif	/* ! __SUBR_ATF_H__ */
