dnl Checks for libcdatetime required headers and functions
dnl
dnl Version: 20190308

dnl Function to detect if libcdatetime is available
dnl ac_libcdatetime_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCDATETIME_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libcdatetime" = xno],
    [ac_cv_libcdatetime=no],
    [ac_cv_libcdatetime=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libcdatetime" != x && test "x$ac_cv_with_libcdatetime" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libcdatetime"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libcdatetime}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libcdatetime}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libcdatetime],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libcdatetime],
          [libcdatetime >= 20141018],
          [ac_cv_libcdatetime=yes],
          [ac_cv_libcdatetime=check])
        ])
      AS_IF(
        [test "x$ac_cv_libcdatetime" = xyes],
        [ac_cv_libcdatetime_CPPFLAGS="$pkg_cv_libcdatetime_CFLAGS"
        ac_cv_libcdatetime_LIBADD="$pkg_cv_libcdatetime_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libcdatetime" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libcdatetime.h])

      AS_IF(
        [test "x$ac_cv_header_libcdatetime_h" = xno],
        [ac_cv_libcdatetime=no],
        [dnl Check for the individual functions
        ac_cv_libcdatetime=yes

        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_get_version,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])

        dnl Time elements functions
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_initialize,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_free,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_copy,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])

        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_year,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_day_of_year,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_month,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_day_of_month,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_date_values,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])

        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_hours,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_minutes,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_seconds,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_milli_seconds,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_micro_seconds,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_nano_seconds,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_time_values,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])

        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_set_current_time_utc,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_set_current_time_localtime,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])

        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_delta_in_seconds,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_set_from_delta_in_seconds,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])

        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_get_string_size,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_copy_to_string,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_elements_copy_to_string_with_index,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])

        dnl Timestamp functions
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_timestamp_initialize,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_timestamp_free,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_timestamp_copy,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])

        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_timestamp_set_current_time,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])

        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_timestamp_get_delta_in_seconds,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])

        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_timestamp_get_string_size,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_timestamp_copy_to_string,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])
        AC_CHECK_LIB(
          cdatetime,
          libcdatetime_timestamp_copy_to_string_with_index,
          [ac_cv_libcdatetime_dummy=yes],
          [ac_cv_libcdatetime=no])

        ac_cv_libcdatetime_LIBADD="-lcdatetime"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libcdatetime" != x && test "x$ac_cv_with_libcdatetime" != xauto-detect && test "x$ac_cv_libcdatetime" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libcdatetime in directory: $ac_cv_with_libcdatetime],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libcdatetime" = xyes],
    [AC_DEFINE(
      [HAVE_LIBCDATETIME],
      [1],
      [Define to 1 if you have the `cdatetime' library (-lcdatetime).])
    ])

  AS_IF(
    [test "x$ac_cv_libcdatetime" = xyes],
    [AC_SUBST(
      [HAVE_LIBCDATETIME],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBCDATETIME],
      [0])
    ])
  ])

dnl Function to detect if ctime_r or ctime is available
dnl Also checks how to use ctime_r
AC_DEFUN([AX_LIBCDATETIME_CHECK_FUNC_CTIME],
  [AC_CHECK_FUNCS([ctime_r])

  AS_IF(
    [test "x$ac_cv_func_ctime_r" = xyes],
    [AC_MSG_CHECKING(
      [how to use ctime_r])

    AC_LANG_PUSH(C)

    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <time.h>]],
        [[ctime_r( NULL, NULL, 0 )]] )],
        [AC_MSG_RESULT(
          [with additional size argument])
        ac_cv_cv_ctime_r_size=yes],
        [ac_cv_cv_ctime_r_size=no])

    AS_IF(
      [test "x$ac_cv_cv_ctime_r_size" = xno],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [[#include <time.h>]],
          [[ctime_r( NULL, NULL )]] )],
        [AC_MSG_RESULT(
          [with two arguments])
        ac_cv_cv_ctime_r_posix=yes],
        [ac_cv_cv_ctime_r_posix=no])
      ])

    AS_IF(
      [test "x$ac_cv_cv_ctime_r_posix" = xno],
      [CPPFLAGS="$CPPFLAGS -D_POSIX_PTHREAD_SEMANTICS"
      AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [[#include <time.h>]],
          [[ctime_r( NULL, NULL )]] )],
        [AC_MSG_RESULT(
          [with two arguments and definition _POSIX_PTHREAD_SEMANTICS])
          ac_cv_cv_ctime_r_posix=yes],
        [ac_cv_cv_ctime_r_posix=no])
      ])

    AC_LANG_POP(C)

    AS_IF(
      [test "x$ac_cv_cv_ctime_r_size" = xno && test "x$ac_cv_cv_ctime_r_posix" = xno],
      [AC_MSG_WARN(
        [unknown])
      ac_cv_func_ctime_r=no])

    AS_IF(
      [test "x$ac_cv_func_ctime_r" = xyes],
      [AC_DEFINE(
        [HAVE_CTIME_R],
        [1],
        [Define to 1 if you have the ctime_r function.])
      ])

    AS_IF(
      [test "x$ac_cv_cv_ctime_r_size" = xyes],
      [AC_DEFINE(
        [HAVE_CTIME_R_SIZE],
        [1],
        [Define to 1 if you have the ctime_r function with a third size argument.])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_func_ctime_r" = xno],
    [AC_CHECK_FUNCS([ctime])

    AS_IF(
      [test "x$ac_cv_func_ctime" = xno],
      [AC_MSG_FAILURE(
        [Missing function: ctime_r and ctime],
        [1])
      ])
    ])
  ])

dnl Function to detect if libcdatetime dependencies are available
AC_DEFUN([AX_LIBCDATETIME_CHECK_LOCAL],
  [dnl Headers included in libcdatetime/libcdatetime_elements.c
  dnl and libcdatetime/libcdatetime_timestamp.c
  AC_CHECK_HEADERS([errno.h])

  AC_HEADER_TIME

  dnl Types used in libcdatetime/libcdatetime_elements.c
  AC_STRUCT_TM

  dnl Date and time functions used in libcdatetime/libcdatetime_elements.c
  AC_CHECK_FUNCS([mktime])

  AS_IF(
    [test "x$ac_cv_func_mktime" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: mktime],
      [1])
    ])

  dnl Date and time functions used in libcdatetime/libcdatetime_elements.c
  AX_LIBCDATETIME_CHECK_FUNC_CTIME

  AC_CHECK_FUNCS([gmtime gmtime_r localtime localtime_r mktime time])

  AS_IF(
    [test "x$ac_cv_func_gmtime" != xyes && test "x$ac_cv_func_gmtime_r" != xyes],
    [AC_MSG_FAILURE(
      [Missing functions: gmtime_r and gmtime],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_localtime" != xyes && test "x$ac_cv_func_localtime_r" != xyes],
    [AC_MSG_FAILURE(
      [Missing functions: localtime_r and localtime],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_mktime" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: mktime],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_time" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: time],
      [1])
    ])

  ac_cv_libcdatetime_CPPFLAGS="-I../libcdatetime";
  ac_cv_libcdatetime_LIBADD="../libcdatetime/libcdatetime.la";

  ac_cv_libcdatetime=local
  ])

dnl Function to detect how to enable libcdatetime
AC_DEFUN([AX_LIBCDATETIME_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libcdatetime],
    [libcdatetime],
    [search for libcdatetime in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBCDATETIME_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libcdatetime" != xyes],
    [AX_LIBCDATETIME_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBCDATETIME],
      [1],
      [Define to 1 if the local version of libcdatetime is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBCDATETIME],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBCDATETIME],
    [test "x$ac_cv_libcdatetime" = xlocal])
  AS_IF(
    [test "x$ac_cv_libcdatetime_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBCDATETIME_CPPFLAGS],
      [$ac_cv_libcdatetime_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libcdatetime_LIBADD" != "x"],
    [AC_SUBST(
      [LIBCDATETIME_LIBADD],
      [$ac_cv_libcdatetime_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libcdatetime" = xyes],
    [AC_SUBST(
      [ax_libcdatetime_pc_libs_private],
      [-lcdatetime])
    ])

  AS_IF(
    [test "x$ac_cv_libcdatetime" = xyes],
    [AC_SUBST(
      [ax_libcdatetime_spec_requires],
      [libcdatetime])
    AC_SUBST(
      [ax_libcdatetime_spec_build_requires],
      [libcdatetime-devel])
    ])
  ])

