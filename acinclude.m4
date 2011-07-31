dnl Function to test if a certain feature was enabled
AC_DEFUN([LIBEWF_ARG_ENABLE],
 [AC_ARG_ENABLE(
  [$1],
  [AS_HELP_STRING(
   [--enable-$1],
   [$3 [default is $4]])],
  [ac_cv_enable_$2=$enableval],
  [ac_cv_enable_$2=$4])dnl
  AC_CACHE_CHECK(
   [whether to enable $3],
   [ac_cv_enable_$2],
   [ac_cv_enable_$2=$4])dnl
 ])

dnl Function to test if the location of a certain feature was provided
AC_DEFUN([LIBEWF_ARG_WITH],
 [AC_ARG_WITH(
  [$1],
  [AS_HELP_STRING(
   [--with-$1=[$5]],
   [$3 [default is $4]])],
  [ac_cv_with_$2=$withval],
  [ac_cv_with_$2=$4])dnl
  AC_CACHE_CHECK(
   [whether to use $3],
   [ac_cv_with_$2],
   [ac_cv_with_$2=$4])dnl
 ])

dnl Function to detect whether WINAPI support should be enabled
AC_DEFUN([LIBEWF_CHECK_WINAPI],
 [AS_IF(
  [test "x$ac_cv_enable_winapi" = xauto-detect],
  [ac_libewf_target_string="$target";

  AS_IF(
   [test "x$ac_libewf_target_string" = x],
   [ac_libewf_target_string="$build"])

  AS_CASE(
   [$ac_libewf_target_string],
   [*mingw*],[AC_MSG_NOTICE(
              [Detected MinGW enabling WINAPI support for cross-compilation])
             ac_cv_enable_winapi=yes],
   [*],[ac_cv_enable_winapi=no])
  ])
 ])

dnl Function to detect whether a certain #define is present in a certain WINAPI header
AC_DEFUN([LIBEWF_CHECK_WINAPI_DEFINE],
 [AC_CACHE_CHECK(
  [whether $1 defines $2],
  [ac_cv_libewf_winapi_define_$2],
  [AC_LANG_PUSH(C)
  AC_COMPILE_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <windows.h>
#include <$1>

#if !defined( $2 )
#error $2 is not defined
#endif]]
    [[]] )],
   [ac_cv_libewf_winapi_define_$2=yes],
   [ac_cv_libewf_winapi_define_$2=no])
   AC_LANG_POP(C) ])
 ])

dnl Function to detect whether nl_langinfo supports CODESET
AC_DEFUN([LIBEWF_CHECK_FUNC_LANGINFO_CODESET],
 [AC_CHECK_FUNCS([nl_langinfo])

 AS_IF(
  [test "x$ac_cv_func_nl_langinfo" = xyes],
  [AC_CACHE_CHECK(
   [for nl_langinfo CODESET support],
   [ac_cv_libewf_langinfo_codeset],
   [AC_LANG_PUSH(C)
   AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
     [[#include <langinfo.h>]],
     [[char* charset = nl_langinfo( CODESET );]] )],
    [ac_cv_libewf_langinfo_codeset=yes],
    [ac_cv_libewf_langinfo_codeset=no])
   AC_LANG_POP(C) ]) ],
  [ac_cv_libewf_langinfo_codeset=no])

 AS_IF(
  [test "x$ac_cv_libewf_langinfo_codeset" = xyes],
  [AC_DEFINE(
   [HAVE_LANGINFO_CODESET],
   [1],
   [Define if nl_langinfo has CODESET support.])
  ])
 ])

dnl Function to detect whether printf conversion specifier "%jd" is available
AC_DEFUN([LIBEWF_CHECK_FUNC_PRINTF_JD],
 [AC_MSG_CHECKING(
  [whether printf supports the conversion specifier "%jd"])

 SAVE_CFLAGS="$CFLAGS"
 CFLAGS="$CFLAGS -Wall -Werror"
 AC_LANG_PUSH(C)

 dnl First try to see if compilation and linkage without a parameter succeeds
 AC_LINK_IFELSE(
  [AC_LANG_PROGRAM(
   [[#include <stdio.h>]],
   [[printf( "%jd" ); ]] )],
  [ac_cv_libewf_have_printf_jd=no],
  [ac_cv_libewf_have_printf_jd=yes])

 dnl Second try to see if compilation and linkage with a parameter succeeds
 AS_IF(
  [test "x$ac_cv_libewf_have_printf_jd" = xyes],
  [AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[printf( "%jd", (off_t) 10 ); ]] )],
    [ac_cv_libewf_have_printf_jd=yes],
    [ac_cv_libewf_have_printf_jd=no])
  ])

 dnl Third try to see if the program runs correctly
 AS_IF(
  [test "x$ac_cv_libewf_have_printf_jd" = xyes],
  [AC_RUN_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[char string[ 3 ];
if( snprintf( string, 3, "%jd", (off_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
    [ac_cv_libewf_have_printf_jd=yes],
    [ac_cv_libewf_have_printf_jd=no],
    [ac_cv_libewf_have_printf_jd=undetermined])
   ])

 AC_LANG_POP(C)
 CFLAGS="$SAVE_CFLAGS"

 AS_IF(
  [test "x$ac_cv_libewf_have_printf_jd" = xyes],
  [AC_MSG_RESULT(
   [yes])
  AC_DEFINE(
   [HAVE_PRINTF_JD],
   [1],
   [Define to 1 whether printf supports the conversion specifier "%jd".]) ],
  [AC_MSG_RESULT(
   [$ac_cv_libewf_have_printf_jd]) ])
 ])

dnl Function to detect whether printf conversion specifier "%zd" is available
AC_DEFUN([LIBEWF_CHECK_FUNC_PRINTF_ZD],
 [AC_MSG_CHECKING(
  [whether printf supports the conversion specifier "%zd"])

 SAVE_CFLAGS="$CFLAGS"
 CFLAGS="$CFLAGS -Wall -Werror"
 AC_LANG_PUSH(C)

 dnl First try to see if compilation and linkage without a parameter succeeds
 AC_LINK_IFELSE(
  [AC_LANG_PROGRAM(
   [[#include <stdio.h>]],
   [[printf( "%zd" ); ]] )],
  [ac_cv_libewf_have_printf_zd=no],
  [ac_cv_libewf_have_printf_zd=yes])

 dnl Second try to see if compilation and linkage with a parameter succeeds
 AS_IF(
  [test "x$ac_cv_libewf_have_printf_zd" = xyes],
  [AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[printf( "%zd", (size_t) 10 ); ]] )],
    [ac_cv_libewf_have_printf_zd=yes],
    [ac_cv_libewf_have_printf_zd=no])
  ])

 dnl Third try to see if the program runs correctly
 AS_IF(
  [test "x$ac_cv_libewf_have_printf_zd" = xyes],
  [AC_RUN_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[char string[ 3 ];
if( snprintf( string, 3, "%zd", (size_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
    [ac_cv_libewf_have_printf_zd=yes],
    [ac_cv_libewf_have_printf_zd=no],
    [ac_cv_libewf_have_printf_zd=undetermined])
   ])

 AC_LANG_POP(C)
 CFLAGS="$SAVE_CFLAGS"

 AS_IF(
  [test "x$ac_cv_libewf_have_printf_zd" = xyes],
  [AC_MSG_RESULT(
   [yes])
  AC_DEFINE(
   [HAVE_PRINTF_ZD],
   [1],
   [Define to 1 whether printf supports the conversion specifier "%zd".]) ],
  [AC_MSG_RESULT(
   [$ac_cv_libewf_have_printf_zd]) ])
 ])

dnl Function to detect if ctime_r or ctime is available
dnl Also checks how to use ctime_r
AC_DEFUN([LIBEWF_CHECK_FUNC_CTIME],
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
    ac_cv_libewf_ctime_r_size=yes],
    [ac_cv_libewf_ctime_r_size=no])

  AS_IF(
   [test "x$ac_cv_libewf_ctime_r_size" = xno],
   [AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
     [[#include <time.h>]],
     [[ctime_r( NULL, NULL )]] )],
    [AC_MSG_RESULT(
     [with two arguments])
    ac_cv_libewf_ctime_r_posix=yes],
    [ac_cv_libewf_ctime_r_posix=no])
   ])

  AS_IF(
   [test "x$ac_cv_libewf_ctime_r_posix" = xno],
   [CPPFLAGS="$CPPFLAGS -D_POSIX_PTHREAD_SEMANTICS"
   AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
     [[#include <time.h>]],
     [[ctime_r( NULL, NULL )]] )],
    [AC_MSG_RESULT(
     [with two arguments and definition _POSIX_PTHREAD_SEMANTICS])
     ac_cv_libewf_ctime_r_posix=yes],
    [ac_cv_libewf_ctime_r_posix=no])
   ])

  AC_LANG_POP(C)

  AS_IF(
   [test "x$ac_cv_libewf_ctime_r_size" = xno && test "x$ac_cv_libewf_ctime_r_posix" = xno],
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
   [test "x$ac_cv_libewf_ctime_r_size" = xyes],
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

dnl Function to detect if mkdir is available
dnl Also checks how to use mkdir
AC_DEFUN([LIBEWF_CHECK_FUNC_MKDIR],
 [AC_CHECK_FUNCS([mkdir])

 AS_IF(
  [test "x$ac_cv_func_mkdir" = xyes],
  [AC_MSG_CHECKING(
   [how to use mkdir])

  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -Wall -Werror"
  AC_LANG_PUSH(C)

  AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/stat.h>
#include <sys/types.h>]],
    [[mkdir( "", 0 )]] )],
    [AC_MSG_RESULT(
     [with additional mode argument])
    ac_cv_libewf_mkdir_mode=yes],
    [ac_cv_libewf_mkdir_mode=no])

  AS_IF(
   [test "x$ac_cv_libewf_mkdir_mode" = xno],
   [AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
     [[#include <io.h>]],
     [[mkdir( "" )]] )],
    [AC_MSG_RESULT(
     [with single argument])
    ac_cv_libewf_mkdir=yes],
    [ac_cv_libewf_mkdir=no])
   ])

  AC_LANG_POP(C)
  CFLAGS="$SAVE_CFLAGS"

  AS_IF(
   [test "x$ac_cv_libewf_mkdir_mode" = xno && test "x$ac_cv_libewf_mkdir" = xno],
   [AC_MSG_WARN(
    [unknown])
   ac_cv_func_mkdir=no])

  AS_IF(
   [test "x$ac_cv_func_mkdir" = xyes],
   [AC_DEFINE(
    [HAVE_MKDIR],
    [1],
    [Define to 1 if you have the mkdir function.])
   ])

  AS_IF(
   [test "x$ac_cv_libewf_mkdir_mode" = xyes],
   [AC_DEFINE(
    [HAVE_MKDIR_MODE],
    [1],
    [Define to 1 if you have the mkdir function with a second mode argument.])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_func_mkdir" = xno],
  [AC_MSG_FAILURE(
   [Missing function: mkdir],
   [1])
 ])
])

dnl Function to determine the operating system
AC_DEFUN([LIBEWF_DETERMINE_OPERATING_SYSTEM],
 [ac_libewf_target_string="$target";

 AS_IF(
  [test "x$ac_libewf_target_string" = x],
  [ac_libewf_target_string="$build"])

 AS_CASE(
  [$ac_libewf_target_string],
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

