#include <mpc.h>
#include "mytypes.h"

double complex
mpc_get_dc(mpc_t op, mpc_rnd_t rnd)
{
	mpfr_t mp_real;
	mpfr_t mp_imag;
	double_complex z;

	mpfr_init2(mp_real, 200);
	mpfr_init2(mp_imag, 200);

	mpc_real(mp_real, op, rnd);
	mpc_imag(mp_imag, op, rnd);

	z.parts[0] = mpfr_get_d(mp_real, rnd);
	z.parts[1] = mpfr_get_d(mp_imag, rnd);

	mpfr_clear(mp_real);
	mpfr_clear(mp_imag);

	return (z.z);
}

long double complex
mpc_get_ldc(mpc_t op, mpc_rnd_t rnd)
{
	long_double_complex z;

	return (z.z);
}
