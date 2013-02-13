dnl Functions for Python bindings
dnl
dnl Version: 20130212

dnl Function to check if pyhton-config binary is available
dnl "python$PYTHON_VERSION-config"
AC_DEFUN([AX_PROG_PYTHON_CONFIG],
 [AC_REQUIRE([AM_PATH_PYTHON])
 AS_IF(
  [test x"$PYTHON" != x],
  [AC_CHECK_PROGS(
   [PYTHON_CONFIG],
   [python$PYTHON_VERSION-config])
  ])
 AS_IF(
  [test x"$PYTHON_CONFIG" = x],
  [AC_MSG_ERROR(
   [Cannot find python-config]) ])
 AC_SUBST(
  [PYTHON_CONFIG],
  [$PYTHON_CONFIG])
 ])

dnl Function to detect if Python build environment is available
AC_DEFUN([AX_PYTHON_CHECK],
 [AX_PROG_PYTHON_CONFIG

 AS_IF(
  [test x"$PYTHON_CONFIG" != x],
  [dnl Check for Python includes
  PYTHON_INCLUDES=`$PYTHON_CONFIG --includes`

  AC_MSG_CHECKING(
   [for Python includes])
  AC_MSG_RESULT(
   [$PYTHON_INCLUDES])

  dnl Check for Python libraries
  PYTHON_LIBS=`$PYTHON_CONFIG --libs`

  AC_MSG_CHECKING(
   [for Python libraries])
  AC_MSG_RESULT(
   [$PYTHON_LIBS])

  dnl Check for the existence of Python.h
  BACKUP_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES"

  AC_CHECK_HEADERS(
   [Python.h],
   [ac_cv_header_python_h=yes],
   [ac_cv_header_python_h=no])

  CPPFLAGS="$BACKUP_CPPFLAGS"
 ])

 AS_IF(
  [test x"$ac_cv_header_python_h" != xyes],
  [ac_cv_enable_python=no],
  [ac_cv_enable_python=$PYTHON_VERSION
  AC_SUBST(
   [PYTHON_CPPFLAGS],
   [$PYTHON_INCLUDES])

  AC_SUBST(
   [PYTHON_LDFLAGS],
   [$PYTHON_LIBS])

  AC_SUBST(
   [PYTHON_SITE_PACKAGES_DIR],
   [$am_cv_python_pythondir])
  ])
 ])

dnl Function to detect if to enable Python
AC_DEFUN([AX_PYTHON_CHECK_ENABLE],
 [dnl Deprecated way of enabling Python bindings
 AX_COMMON_ARG_ENABLE(
  [python],
  [python],
  [build Python bindings],
  [no])

 AS_IF(
  [test "x$ac_cv_enable_python" != xno],
  [AM_PATH_PYTHON([2.5])
  AX_PYTHON_CHECK])

 AS_IF(
  [test "x$ac_cv_enable_python" != xno],
  [AC_DEFINE(
   [HAVE_PYTHON],
   [1],
   [Define to 1 if you have Python])
  ])

 AM_CONDITIONAL(
  HAVE_PYTHON,
  [test "x$ac_cv_enable_python" != xno])
])

