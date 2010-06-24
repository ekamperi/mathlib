# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
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
AC_CHECK_FUNCS([acos acosh asin asinh atan atanh cbrt ceil cosh erf floor hypot])
AC_CHECK_FUNCS([sin cos exp expm1 exp2 fabs log log1p log10 log2 rint sinh])
AC_CHECK_FUNCS([sqrt pow tgamma y0 y1 yn])

# Get GMP include path
AC_ARG_WITH([gmp-include-path],
  [AS_HELP_STRING([--with-gmp-include-path],
    [location of the GMP headers, defaults to /usr/include/gmp])],
  [GMP_CFLAGS="-I$withval"],
  [GMP_CFLAGS='-I/usr/include/gmp'])
AC_SUBST([GMP_CFLAGS])

# Get GMP library path
AC_ARG_WITH([gmp-lib-path],
  [AS_HELP_STRING([--with-gmp-lib-path],
    [location of the GMP libraries, defaults to -lmpfr])],
  [MPFR_LIBS="-L$withval -lmpfr"],
  [MPFR_LIBS='-lgmp'])
AC_SUBST([GMP_LIBS])

# Get MPFR include path
AC_ARG_WITH([mpfr-include-path],
  [AS_HELP_STRING([--with-mpfr-include-path],
    [location of the MPFR headers, defaults to /usr/include/mpfr])],
  [MPFR_CFLAGS="-I$withval"],
  [MPFR_CFLAGS='-I/usr/include/mpfr'])
AC_SUBST([MPFR_CFLAGS])

# Get MPFR library path
AC_ARG_WITH([mpfr-lib-path],
  [AS_HELP_STRING([--with-mpfr-lib-path],
    [location of the MPFR libraries, defaults to -lmpfr])],
  [MPFR_LIBS="-L$withval -lmpfr"],
  [MPFR_LIBS='-lmpfr'])
AC_SUBST([MPFR_LIBS])

#AC_CONFIG_FILES([Makefile])
AC_OUTPUT
