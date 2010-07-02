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
AC_CHECK_FUNCS([acos acosh asin asinh atan atanh cbrt ceil cosh erf erfc floor hypot])
AC_CHECK_FUNCS([sin cos exp expm1 exp2 fabs log log1p log10 log2 rint sinh])
AC_CHECK_FUNCS([sqrt pow tan tanh tgamma y0 y1 yn])

AC_OUTPUT
