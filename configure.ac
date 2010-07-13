# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([mathlib], [0.1], [ekamperi@gmail.com])
AC_CONFIG_SRCDIR([t_atan.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([m], [cos])

# Checks for header files.
AC_CHECK_HEADERS([fenv.h float.h inttypes.h limits.h math.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC

AC_CHECK_FUNCS([acos asin])

AC_CHECK_FUNCS([acoshl acosl asinhl asinl atan2l atanhl atanl cbrtl ceill])
AC_CHECK_FUNCS([copysignl coshl cosl erfcl erfl exp2l expl expm1l fabsl fdiml])
AC_CHECK_FUNCS([floorl fmal fmaxl fminl fmodl frexpl hypotl ilogbl ldexpl])
AC_CHECK_FUNCS([lgammal llrintl llroundl log10l log1pl log2l logbl logl lrintl])
AC_CHECK_FUNCS([lroundl modfl nanl nearbyintl nextafterl nexttowardl powl])
AC_CHECK_FUNCS([remainderl remquol rintl roundl scalblnl scalbnl sinhl sinl])
AC_CHECK_FUNCS([sqrtl tanhl tanl tgammal truncl])

AC_CHECK_FUNCS([strtold])

AC_OUTPUT
