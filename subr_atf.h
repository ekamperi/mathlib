#ifndef	__SUBR_ATF_H__
#define	__SUBR_ATF_H__

bool get_config_var_as_bool(const atf_tc_t *tc, const char *name);
long get_config_var_as_long(const atf_tc_t *tc, const char *name);

/* Ugly, I know */
#define ATF_MYCHECK(x, cond)			\
do {						\
	*(cond) = (x);				\
	ATF_CHECK(x);				\
} while(0)

#endif	/* ! __SUBR_ATF_H__ */
