#include <inttypes.h>
#include <stdio.h>
#include <string.h>

/* Store x87 FPU state */
#define __fnstenv(__env)        __asm__ __volatile__	\
	("fnstenv %0" : "=m" (*(__env)))

struct fenv {
	uint32_t fe_cw;         /* Control word register */
	uint32_t fe_sw;         /* Status word register */
	uint32_t fe_tw;         /* Tag word register */

	uint32_t fe_unused[5];
};

int
main(void)
{
	struct fenv f;

	memset(&f, 0, sizeof f);
	__fnstenv(&f);

	printf("Control word: 0x%x\n", f.fe_cw);
	printf(" Status word: 0x%x\n", f.fe_sw);
	printf("    Tag word: 0x%x\n", f.fe_tw);

	printf("Instruct Ptr: 0x%x 0x%x\n",
	    f.fe_unused[0],
	    f.fe_unused[1]);
	printf(" Operand Ptr: 0x%x 0x%x\n",
	    f.fe_unused[2],
	    f.fe_unused[3]);

	return 0;
}
