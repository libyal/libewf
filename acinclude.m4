dnl Function to detect whether version 1 API support should be enabled
AC_DEFUN([AX_LIBEWF_CHECK_ENABLE_V1_API],
 [AX_COMMON_ARG_ENABLE(
  [v1-api],
  [v1_api],
  [enable version 1 API],
  [yes])

 AS_IF(
  [test "x$ac_cv_enable_v1_api" != xno],
  [AC_DEFINE(
   [HAVE_V1_API],
   [1],
   [Define to 1 if the version 1 API should be available.])
  AC_SUBST(
   [HAVE_V1_API],
   [1])
  ac_cv_enable_v1_api=yes],
  [AC_SUBST(
   [HAVE_V1_API],
   [0]) ])

 AM_CONDITIONAL(
  HAVE_V1_API,
  [test "x$ac_cv_enable_v1_api" != xno])
 ])

dnl Function to detect whether low level function support should be enabled
AC_DEFUN([AX_LIBEWF_CHECK_ENABLE_LOW_LEVEL_FUNCTION],
 [AX_COMMON_ARG_ENABLE(
  [low-level-functions],
  [low_level_functions],
  [use libewf's low level read and write functions in the ewftools],
  [no])

 AS_IF(
  [test "x$ac_cv_enable_low_level_functions" != xno],
  [AC_DEFINE(
   [HAVE_LOW_LEVEL_FUNCTIONS],
   [1],
   [Define to 1 if libewfs low level read and write functions should be used in the ewf tools.])

  ac_cv_enable_low_level_functions=yes])
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

