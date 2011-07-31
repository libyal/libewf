dnl Functions to for Python bindings

dnl Function to detect if Python build environment is available
AC_DEFUN([AC_CHECK_PYTHON_DEVEL],
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

 python_path=`$PYTHON -c "import distutils.sysconfig;print distutils.sysconfig.get_python_lib() "`;
 AC_MSG_RESULT(
  [$python_path])

 AC_SUBST(
  [PYTHON_LDFLAGS],
  ["-L$python_path -lpython$PYTHON_VERSION"])

 python_site=`echo $python_path | sed "s/config/site-packages/"`;

 AC_SUBST(
  [PYTHON_SITE_PKG],
  [$python_site])

 dnl Determine the Python libraries which must be linked in when embedding
 AC_MSG_CHECKING(
  [for Python extra libraries])

 PYTHON_EXTRA_LIBS=`$PYTHON -c "import distutils.sysconfig;conf = distutils.sysconfig.get_config_var;print conf('LOCALMODLIBS')+' '+conf('LIBS')"`;
 AC_MSG_RESULT(
  [$PYTHON_EXTRA_LIBS])

 AC_SUBST(
 [PYTHON_EXTRA_LIBS])
 ])

