# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([ulp], [0.1], [ekamperi@gmail.com])
AC_CONFIG_SRCDIR([gen.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# This is for NetBSD (pkgsrc in general), otherwise AC_CHECK_LIB()
# will fail to report an existing library as such, if it is located
# in some non-standard path.
LDFLAGS="$LDFLAGS -L/usr/pkg/lib"

# And this is for Linux, FreeBSD and so on.
LDFLAGS="$LDFLAGS -L/usr/local/lib"

# Checks for libraries.
AC_CHECK_LIB([m],[cos],,AC_MSG_ERROR(requires lm))
AC_CHECK_LIB([mpfr],[mpfr_init],,AC_MSG_ERROR(requires mpfr))

# Checks for header files.
AC_CHECK_HEADERS([float.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([acos acosh asin asinh atan atanh atan2 cbrt ceil cos cosh erf])
AC_CHECK_FUNCS([erfc exp expm1 exp2 fabs floor fmod hypot j0 j1 lgamma log log1p])
AC_CHECK_FUNCS([log10 log2 pow remainder rint round sin sinh sqrt tan tanh tgamma])
AC_CHECK_FUNCS([trunc y0 y1 yn])

# j0l, j1l are extentions to the standard.
# XXX: roundl
AC_CHECK_FUNCS([acosl acoshl asinl asinhl atanl atanhl atan2l cbrtl ceill cosl])
AC_CHECK_FUNCS([cosl coshl erfl erfcl expl exp2l expm1l fabsl floorl fmodl])
AC_CHECK_FUNCS([hypotl j0l j1l lgammal log10l log1pl log2l logbl logl])
AC_CHECK_FUNCS([powl remainderl rintl roundl sinhl sinl sqrtl tanhl tanl tgammal truncl])

# Long double versions of complex arithmetic functions.
AC_CHECK_FUNCS([cabs cacos cacosh carg casin casinh catan catanh ccos cexp clog])
AC_CHECK_FUNCS([cpow csin csinh csqrt ctan ccos conj])

AC_CHECK_FUNCS([cabsl cacoshl cacosl cargl casinhl casinl catanhl catanl])
AC_CHECK_FUNCS([ccoshl ccosl cexpl cimagl clogl conjl cpowl cprojl creall])
AC_CHECK_FUNCS([csinhl csinl csqrtl ctanhl ctanl])

AC_OUTPUT
