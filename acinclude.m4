dnl Function to detect if libewf dependencies are available
AC_DEFUN([AX_LIBEWF_CHECK_LOCAL],
 [dnl Check for type definitions
 dnl Type used in libewf/libewf_date_time_values.h, libewf/libewf_date_time.h
 dnl and libewf/libewf_header_values.c
 AC_STRUCT_TM

 dnl Check for headers
 dnl Headers included in libewf/libewf_date_time.h
 AC_HEADER_TIME

 dnl Check for functions
 dnl Date and time functions used in libewf/libewf_date_time.h
 AC_CHECK_FUNCS([localtime localtime_r mktime])

 AS_IF(
  [test "x$ac_cv_func_localtime" != xyes && test "x$ac_cv_func_localtime_r" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: localtime and localtime_r],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_mktime" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: mktime],
   [1])
  ])

 dnl Check for internationalization functions in libewf/libewf_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

dnl Function to determine the host operating system
AC_DEFUN([AX_LIBEWF_CHECK_HOST_OPERATING_SYSTEM],
 [ac_libewf_determine_operating_system_target_string="$target";

 AS_IF(
  [test "x$ac_libewf_determine_operating_system_target_string" = x],
  [ac_libewf_determine_operating_system_target_string="$host"])

 AS_IF(
  [test "x$ac_libewf_determine_operating_system_target_string" = x],
  [ac_libewf_determine_operating_system_target_string="$build"])

 AS_CASE(
  [$ac_libewf_determine_operating_system_target_string],
  [*cygwin*], [ac_libewf_operating_system="Cygwin";],
  [*darwin*], [ac_libewf_operating_system="Darwin";],
  [*freebsd*], [ac_libewf_operating_system="FreeBSD";],
  [*netbsd*], [ac_libewf_operating_system="NetBSD";],
  [*openbsd*], [ac_libewf_operating_system="OpenBSD";],
  [*linux*], [ac_libewf_operating_system="Linux";],
  [*mingw*], [ac_libewf_operating_system="MingW";],
  [*solaris*], [ac_libewf_operating_system="SunOS";],
  [*], [ac_libewf_operating_system="Unknown";])

 AC_DEFINE_UNQUOTED(
  LIBEWF_OPERATING_SYSTEM,
  "$ac_libewf_operating_system",
  [Defines the fallback operating system string.])
 ])

