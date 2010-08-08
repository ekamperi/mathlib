#include "basic-complex.h"

/*******************************************************************************
 *	Basic arithmetic of complex numbers (double precision)
 ******************************************************************************/
double complex
cadd(double complex z, double complex w)
{
	return (z+w);
}

double complex
csub(double complex z, double complex w)
{
	return (z-w);
}

double complex
cmul(double complex z, double complex w)
{
	return (z*w);
}

double complex
cdiv(double complex z, double complex w)
{
	return (z/w);
}

double complex
cneg(double complex z)
{
	return (-z);
}

/*******************************************************************************
 *	Basic arithmetic of complex numbers (long double precision)
 ******************************************************************************/
long double complex
caddl(long double complex z, long double complex w)
{
	return (z+w);
}

long double complex
csubl(long double complex z, long double complex w)
{
	return (z-w);
}

long double complex
cmull(long double complex z, long double complex w)
{
	return (z*w);
}

long double complex
cdivl(long double complex z, long double complex w)
{
	return (z/w);
}

long double complex
cnegl(long double complex z)
{
	return (-z);
}
