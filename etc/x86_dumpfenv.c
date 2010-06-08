#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "x86_dumpfenv.h"

/* No-Wait Store x87 environment */
#define __fnstenv(__env)        __asm__ __volatile__    \
        ("fnstenv %0" : "=m" (*(__env)))

/* Store MXCSR register */
#define	__stmxcsr(__csr)	__asm__ __volatile__	\
	("stmxcsr %0" : "=m" (*(__csr)))

void
fenv_dump(myfenv_t *envp)
{
	memset(envp, 0, sizeof(myfenv_t));

	__fnstenv(&envp->x87);
	__stmxcsr(&envp->mxcsr);
}

void
fenv_print(const myfenv_t *envp)
{
	size_t i;

	printf("  word: 0x%x\n", envp->x87.control);
        printf("status: 0x%x\n", envp->x87.status);
        printf("   tag: 0x%x\n", envp->x87.tag);

        for (i = 0; i < 4; i++)
                printf("others[%d] = 0x%x\n", i, envp->x87.others[i]);

	printf("mxcsr: 0x%x\n", envp->mxcsr);
}

#if 0
int
main(void)
{
	myfenv_t env1, env2;

	/*
	 * printf() taints fp environment; therefore first populate the
	 * structures and and then print their contents.
	 *
	 * Many thanks to sjamman@ who saved the day!
	 */
	fenv_dump(&env1);
	fenv_dump(&env2);

	fenv_print(&env1);
	fenv_print(&env2);

	return 0;
}
#endif
