# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([mathlib], [0.1], [ekamperi@gmail.com])
#AC_CONFIG_SRCDIR([t_atan.c])
AC_CONFIG_HEADERS([include/config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([m], [cos])

# Checks for header files.
AC_CHECK_HEADERS([fenv.h float.h inttypes.h limits.h math.h stdlib.h string.h])
AC_CHECK_HEADERS([tgmath.h])

# This is a non-standard interface. It is only consumed by fenv/t_ieeefp.c,
# to check whether the concurrent inclusion of fenv.h and ieeefp.h causes
# any symbols' collisions.
AC_CHECK_HEADERS([ieeefp.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC

# Needed by t_tgmath only.
AC_CHECK_FUNCS([acos asin atan acosh asinh atanh cos sin tan cosh sinh tanh])
AC_CHECK_FUNCS([exp log pow sqrt fabs])

# Long double versions of real-value math functions.
# Note: `fmal' is not a typo, it refers to fused-multiply-and-add function.
AC_CHECK_FUNCS([acoshl acosl asinhl asinl atan2l atanhl atanl cbrtl ceill])
AC_CHECK_FUNCS([copysignl coshl cosl erfcl erfl exp2l expl expm1l fabsl fdiml])
AC_CHECK_FUNCS([floorl fmal fmaxl fminl fmodl frexpl hypotl ilogbl ldexpl])
AC_CHECK_FUNCS([lgammal llrintl llroundl log10l log1pl log2l logbl logl lrintl])
AC_CHECK_FUNCS([lroundl modfl nanl nearbyintl nextafterl nexttowardl powl])
AC_CHECK_FUNCS([remainderl remquol rintl roundl scalblnl scalbnl sinhl sinl])
AC_CHECK_FUNCS([sqrtl tanhl tanl tgammal truncl])

# Long double versions of complex arithmetic functions.
AC_CHECK_FUNCS([cabsl cacoshl cacosl cargl casinhl casinl catanhl catanl])
AC_CHECK_FUNCS([ccoshl ccosl cexpl cimagl clogl conjl cpowl cprojl creall])
AC_CHECK_FUNCS([csinhl csinl csqrtl ctanhl ctanl])

AC_CHECK_FUNCS([strtold])

AC_OUTPUT
