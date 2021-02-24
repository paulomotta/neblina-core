#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.68])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
PACKAGE=neblina
VERSION=1.1.15

AM_INIT_AUTOMAKE($PACKAGE,$VERSION)

AC_CONFIG_SRCDIR([neblina.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_AWK
#AC_PROG_YACC
AC_PROG_CC
#AC_PROG_LEX

# Checks for libraries.
AC_CHECK_LIB([OpenCL], [main], [vis=yes; break;])

AS_IF([test "x$vis" != "xyes"],
	[AC_MSG_ERROR([Unable to find the OpenCL library])])
	
AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_FUNC_ALLOCA
#AC_CHECK_HEADER([CL/cl.h],
#                     [AC_DEFINE([HAVE_CL_H], [1],
#                        [])],
#                     [AC_MSG_ERROR([header CL/cl.h not found])])
                     
AC_CHECK_HEADERS([inttypes.h libintl.h malloc.h math.h stdio.h stddef.h stdlib.h string.h sys/time.h unistd.h])
# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday printf fprintf memset sqrt cos sin tan acos asin atan sinh cosh sinh exp floor ceil round pow log abs fabs time gettimeofday])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
