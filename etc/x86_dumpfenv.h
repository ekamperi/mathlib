#ifndef	__DUMPFENV_H__
#define	__DUMPFENV_H__

#include <stdint.h>

typedef struct {
	struct {
		uint16_t control;
		uint16_t unused1;

		uint16_t status;
		uint16_t unused2;

		uint16_t tag;
		uint16_t unused3;

		uint32_t others[4];
	} x87;

	uint32_t mxcsr;
} myfenv_t;

void fenv_dump(myfenv_t *envp);
void fenv_print(const myfenv_t *envp);

#endif	/* ! __DUMP_FENV_H__ */
