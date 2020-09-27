dnl Checks for required headers and functions
dnl
dnl Version: 20200713

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

dnl Function to detect if ewftools dependencies are available
AC_DEFUN([AX_EWFTOOLS_CHECK_LOCAL],
  [dnl Headers used in ewftools
  AC_CHECK_HEADERS([signal.h sys/signal.h unistd.h])

  dnl Functions used in ewftools
  AC_CHECK_FUNCS([close getopt setvbuf])

  AS_IF(
   [test "x$ac_cv_func_close" != xyes],
   [AC_MSG_FAILURE(
     [Missing function: close],
     [1])
  ])

  dnl Check for the host operation system will be used as a fall back in the ewftools
  AX_LIBEWF_CHECK_HOST_OPERATING_SYSTEM

  dnl Headers included in ewftools/ewftools_glob.h
  AC_CHECK_HEADERS([errno.h glob.h])

  AS_IF(
    [test "x$ac_cv_header_glob_h" = xno],
    [AC_CHECK_HEADERS([io.h])
  ])

  dnl Headers included in ewftools/log_handle.c
  AC_CHECK_HEADERS([stdarg.h varargs.h])

  AS_IF(
    [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
    [AC_MSG_FAILURE(
      [Missing headers: stdarg.h and varargs.h],
      [1])
  ])

  dnl Headers included in ewftools/ewfmount.c
  AC_CHECK_HEADERS([errno.h])

  AC_HEADER_TIME

  dnl Functions included in ewftools/mount_file_system.c and ewftools/mount_file_entry.c
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_FUNCS([clock_gettime getegid geteuid time])
  ])
])

dnl Function to check if DLL support is needed
AC_DEFUN([AX_LIBEWF_CHECK_DLL_SUPPORT],
  [AS_IF(
    [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw* | *msys*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])

      AC_SUBST(
        [LIBEWF_DLL_EXPORT],
        ["-DLIBEWF_DLL_EXPORT"])

      AC_SUBST(
        [LIBEWF_DLL_IMPORT],
        ["-DLIBEWF_DLL_IMPORT"])
      ])
    ])
  ])

