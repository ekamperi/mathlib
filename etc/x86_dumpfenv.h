#ifndef	__DUMPFENV_H__
#define	__DUMPFENV_H__

typedef struct {
	struct {
		uint16_t control;
		uint16_t status;
		uint32_t tag;
		uint32_t unused[4];
	} x87;

	uint32_t mxcsr;
} myfenv_t;

void fenv_dump(myfenv_t *envp);
void fenv_print(const myfenv_t *envp);

#endif	/* ! __DUMP_FENV_H__ */
