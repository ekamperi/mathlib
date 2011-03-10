#ifndef	__SUBR_MPC_H__
#define	__SUBR_MPC_H__

#if MPC_VERSION_MINOR == 0 && MPC_VERSION_MAJOR < 9
double complex mpc_get_dc(mpc_t op, mpfr_rnd_t rnd);
long double complex mpc_get_ldc(mpc_t op, mpfr_rnd_t rnd);
#endif

#endif	/* ! __SUBR_MPC_H__ */
