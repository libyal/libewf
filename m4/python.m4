dnl Functions for Python bindings
dnl
dnl Version: 20121203

dnl Function to detect if Python build environment is available
AC_DEFUN([AX_PYTHON_CHECK],
 [AC_REQUIRE([AM_PATH_PYTHON])

 dnl Check for Python include path
 AC_MSG_CHECKING(
  [for Python include path])

 PYTHON_INCLUDE_DIR=`$PYTHON -c "import distutils.sysconfig;print distutils.sysconfig.get_python_inc() "`;

 AC_MSG_RESULT(
  [$PYTHON_INCLUDE_DIR])

 AC_SUBST(
  [PYTHON_CPPFLAGS],
  [-I$PYTHON_INCLUDE_DIR])

 AS_IF(
  [test ! -r $PYTHON_INCLUDE_DIR/Python.h],
  [AC_MSG_ERROR(
  [Missing Python include file]) ])

 dnl Check for Python library path
 AC_MSG_CHECKING(
  [for Python library path])

 PYTHON_LIBRARY_DIR=`$PYTHON -c "import distutils.sysconfig;print distutils.sysconfig.get_python_lib() "`;

 AC_MSG_RESULT(
  [$PYTHON_LIBRARY_DIR])

 AC_SUBST(
  [PYTHON_LDFLAGS],
  ["-L$PYTHON_LIBRARY_DIR -lpython$PYTHON_VERSION"])

 PYTHON_SITE_PACKAGES_DIR=`echo $PYTHON_LIBRARY_DIR | sed "s/config/site-packages/"`;

 AC_SUBST(
  [PYTHON_SITE_PACKAGES_DIR],
  [$PYTHON_SITE_PACKAGES_DIR])

 dnl Determine the Python libraries which must be linked in when embedding
 AC_MSG_CHECKING(
  [for Python extra libraries])

 PYTHON_EXTRA_LIBS=`$PYTHON -c "import distutils.sysconfig;conf = distutils.sysconfig.get_config_var;print conf('LOCALMODLIBS')+' '+conf('LIBS')"`;

 AC_MSG_RESULT(
  [$PYTHON_EXTRA_LIBS])

 AC_SUBST(
  [PYTHON_EXTRA_LIBS])
 ])

dnl Function to detect if to enable Python
AC_DEFUN([AX_PYTHON_CHECK_ENABLE],
 [AX_COMMON_ARG_ENABLE(
  [python],
  [python],
  [build Python bindings],
  [no])

 AS_IF(
  [test "x$ac_cv_enable_python" != xno],
  [AM_PATH_PYTHON([2.5])
  AX_PYTHON_CHECK
  ac_cv_enable_python=yes])

 AS_IF(
  [test "x$ac_cv_enable_python" = xyes],
  [AC_DEFINE(
   [HAVE_PYTHON],
   [1],
   [Define to 1 if you have Python])
  ])

 AM_CONDITIONAL(
  HAVE_PYTHON,
  [test "x$ac_cv_enable_python" = xyes])
])

