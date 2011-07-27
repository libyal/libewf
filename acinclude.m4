dnl Function to test if a certain feature was enabled
AC_DEFUN([LIBEWF_TEST_ENABLE],
 [AC_ARG_ENABLE(
  [$1],
  [AS_HELP_STRING(
   [--enable-$1],
   [$3 (default is $4)])],
  [ac_cv_libewf_enable_$2=$enableval],
  [ac_cv_libewf_enable_$2=$4])dnl
  AC_CACHE_CHECK(
   [whether to enable $3],
   [ac_cv_libewf_enable_$2],
   [ac_cv_libewf_enable_$2=$4])dnl
 ])

dnl Function to test if the location of a certain feature was provided
AC_DEFUN([LIBEWF_TEST_WITH],
 [AC_ARG_WITH(
  [$1],
  [AS_HELP_STRING(
   [--with-$1=[$5]],
   [$3 [default is $4]])],
  [ac_cv_libewf_with_$2=$withval],
  [ac_cv_libewf_with_$2=$4])dnl
  AC_CACHE_CHECK(
   [whether to use $3],
   [ac_cv_libewf_with_$2],
   [ac_cv_libewf_with_$2=$4])dnl
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

dnl Function to detect if posix_fadvise is available
AC_DEFUN([LIBEWF_CHECK_FUNC_POSIX_FADVISE],
 [AC_CHECK_FUNCS([posix_fadvise])

 AS_IF(
  [test "x$ac_cv_func_posix_fadvise" = xyes],
  [AC_MSG_CHECKING(
    [whether posix_fadvise can be linked])

   SAVE_CFLAGS="$CFLAGS"
   CFLAGS="$CFLAGS -Wall -Werror"
   AC_LANG_PUSH(C)

   AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
     [[#include <fcntl.h>]],
     [[#if !defined( POSIX_FADV_SEQUENTIAL )
#define POSIX_FADV_SEQUENTIAL 2
#endif
posix_fadvise( 0, 0, 0, POSIX_FADV_SEQUENTIAL )]] )],
     [ac_cv_func_posix_fadvise=yes],
     [ac_cv_func_posix_fadvise=no])

   AC_LANG_POP(C)
   CFLAGS="$SAVE_CFLAGS"

   AS_IF(
    [test "x$ac_cv_func_posix_fadvise" = xyes],
    [AC_MSG_RESULT(
     [yes])
    AC_DEFINE(
     [HAVE_POSIX_FADVISE],
     [1],
     [Define to 1 if you have the posix_fadvise function.]) ],
    [AC_MSG_RESULT(
     [no]) ])
  ])
 ])

dnl Function to detect whether openssl/evp.h can be used in combination with zlib.h
AC_DEFUN([LIBEWF_CHECK_OPENSSL_EVP_ZLIB_COMPATIBILE],
 [AC_CACHE_CHECK(
  [if openssl/evp.h can be used in combination with zlib.h],
  [ac_cv_libewf_openssl_evp_zlib_compatible],
  [AC_LANG_PUSH(C)
  AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <zlib.h>
#include <openssl/evp.h>]],
    [[ ]] )],
   [ac_cv_libewf_openssl_evp_zlib_compatible=yes],
   [ac_cv_libewf_openssl_evp_zlib_compatible=no])
  AC_LANG_POP(C)])
 ])

dnl Function to detect if libuna available
AC_DEFUN([LIBEWF_CHECK_LIBUNA],
 [AC_CHECK_HEADERS([libuna.h])

 AS_IF(
  [test "x$ac_cv_header_libuna_h" = xno],
  [ac_libewf_have_libuna=no],
  [ac_libewf_have_libuna=yes
  AC_CHECK_LIB(
   una,
   libuna_get_version,
   [],
   [ac_libewf_have_libuna=no])
 
  dnl Byte stream functions
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_size_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_copy_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_size_from_utf16,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_copy_from_utf16,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_size_from_utf32,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_copy_from_utf32,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
 
  dnl UTF-8 stream functions
  AC_CHECK_LIB(
   una,
   libuna_utf8_stream_size_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_stream_copy_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_stream_size_from_utf16,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_stream_copy_from_utf16,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_stream_size_from_utf32,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_stream_copy_from_utf32,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])

  dnl UTF-16 stream functions
  AC_CHECK_LIB(
   una,
   libuna_utf16_stream_size_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_stream_copy_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_stream_size_from_utf16,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_stream_copy_from_utf16,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_stream_size_from_utf32,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_stream_copy_from_utf32,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])

  dnl UTF-32 stream functions
  AC_CHECK_LIB(
   una,
   libuna_utf32_stream_size_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_stream_copy_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_stream_size_from_utf16,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_stream_copy_from_utf16,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_stream_size_from_utf32,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_stream_copy_from_utf32,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])

  dnl UTF-8 string functions
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_size_from_byte_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_copy_from_byte_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_size_from_utf8_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_copy_from_utf8_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_size_from_utf16_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_copy_from_utf16_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_size_from_utf32_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_copy_from_utf32_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_size_from_utf16,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_copy_from_utf16,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_size_from_utf32,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_copy_from_utf32,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])

  dnl UTF-16 string functions
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_size_from_byte_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_copy_from_byte_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_size_from_utf8_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_copy_from_utf8_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_size_from_utf16_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_copy_from_utf16_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_size_from_utf32_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_copy_from_utf32_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_size_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_copy_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])

  dnl UTF-32 string functions
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_size_from_byte_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_copy_from_byte_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_size_from_utf8_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_copy_from_utf8_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_size_from_utf16_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_copy_from_utf16_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_size_from_utf32_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_copy_from_utf32_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_size_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_copy_from_utf8,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libuna=no])
  ])
 ])

dnl Function to detect if libbfio available
AC_DEFUN([LIBEWF_CHECK_LIBBFIO],
 [AC_CHECK_HEADERS([libbfio.h])

 AS_IF(
  [test "x$ac_cv_header_libbfio_h" = xno],
  [ac_libewf_have_libbfio=no],
  [ac_libewf_have_libbfio=yes
  AC_CHECK_LIB(
   bfio,
   libbfio_get_version,
   [],
   [ac_libewf_have_libbfio=no])
 
  dnl Handle functions
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_free,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_open,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_close,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_exists,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_read,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_write,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_seek_offset,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])

  AC_CHECK_LIB(
   bfio,
   libbfio_handle_is_open,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_get_offset,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_get_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_set_track_offsets_read,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_get_number_of_offsets_read,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_get_offset_read,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
 
  dnl File functions
  AC_CHECK_LIB(
   bfio,
   libbfio_file_initialize,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_file_get_name_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_file_get_name,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_file_set_name,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
 
  AS_IF(
   [test "x$ac_cv_libewf_enable_wide_character_type" != xno],
   [AC_CHECK_LIB(
    bfio,
    libbfio_file_get_name_size_wide,
    [ac_libewf_dummy=yes],
    [ac_libewf_have_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_file_get_name_wide,
    [ac_libewf_dummy=yes],
    [ac_libewf_have_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_file_set_name_wide,
    [ac_libewf_dummy=yes],
    [ac_libewf_have_libbfio=no])
   ])
 
  dnl Pool functions
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_initialize,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_free,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_clone,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_open,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_reopen,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_close,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_close_all,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_read,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_write,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_seek_offset,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
 
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_get_number_of_handles,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_get_handle,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_set_handle,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_append_handle,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_get_offset,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_get_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_pool_set_maximum_number_of_open_handles,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libbfio=no])
  ])
 ])

dnl Function to detect if libmfdata available
AC_DEFUN([LIBEWF_CHECK_LIBMFDATA],
 [AC_CHECK_HEADERS([libmfdata.h])

 AS_IF(
  [test "x$ac_cv_header_libmfdata_h" = xno],
  [ac_libewf_have_libmfdata=no],
  [ac_libewf_have_libmfdata=yes
  AC_CHECK_LIB(
   mfdata,
   libmfdata_get_version,
   [],
   [ac_libewf_have_libmfdata=no])
 
  dnl Segment table functions
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_initialize,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])

  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_free,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_empty,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_resize,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_read_buffer,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_seek_offset,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_write_buffer,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])

  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_get_file_io_handle,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_get_maximum_segment_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_get_number_of_segments,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_get_value_offset,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_get_value_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_set_maximum_segment_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  AC_CHECK_LIB(
   mfdata,
   libmfdata_segment_table_set_segment_by_index,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libmfdata=no])
  ])
 ])

dnl Function to detect if libfvalue available
AC_DEFUN([LIBEWF_CHECK_LIBFVALUE],
 [AC_CHECK_HEADERS([libfvalue.h])

 AS_IF(
  [test "x$ac_cv_header_libfvalue_h" = xno],
  [ac_libewf_have_libfvalue=no],
  [ac_libewf_have_libfvalue=yes
  AC_CHECK_LIB(
   fvalue,
   libfvalue_get_version,
   [],
   [ac_libewf_have_libfvalue=no])
 
  dnl String functions
  AC_CHECK_LIB(
   fvalue,
   libfvalue_utf8_string_decimal_copy_from_8bit,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_utf8_string_decimal_copy_from_16bit,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_utf8_string_decimal_copy_from_32bit,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_utf8_string_decimal_copy_to_64bit,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_utf8_string_hexadecimal_copy_to_64bit,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_utf16_string_decimal_copy_from_8bit,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_utf16_string_decimal_copy_from_16bit,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
 
  dnl Table functions
  AC_CHECK_LIB(
   fvalue,
   libfvalue_table_initialize,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_table_free,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_table_clone,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_table_get_number_of_values,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_table_get_value_by_identifier,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_table_get_value_by_index,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_table_set_value,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_table_set_value_by_index,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_table_copy_from_utf8_xml_string,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
 
  dnl Value functions
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_initialize,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_free,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_clone,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_get_identifier,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_set_identifier,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_get_data,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_set_data,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_has_data,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])

  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_copy_from_64bit,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_copy_to_64bit,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])

  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_get_utf8_string_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_copy_from_utf8_string,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_copy_to_utf8_string,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_get_utf16_string_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_copy_from_utf16_string,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_copy_to_utf16_string,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])

  AC_CHECK_LIB(
   fvalue,
   libfvalue_value_write_to_file_stream,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libfvalue=no])
  ])
 ])

dnl Function to detect if libhmac available
AC_DEFUN([LIBEWF_CHECK_LIBHMAC],
 [AC_CHECK_HEADERS([libhmac.h])

 AS_IF(
  [test "x$ac_cv_header_libhmac_h" = xno],
  [ac_libewf_have_libhmac=no],
  [ac_libewf_have_libhmac=yes
  AC_CHECK_LIB(
   hmac,
   libhmac_get_version,
   [],
   [ac_libewf_have_libhmac=no])

   ])
  ])
 ])

dnl Function to detect if libsmdev available
AC_DEFUN([LIBEWF_CHECK_LIBSMDEV],
 [AC_CHECK_HEADERS([libsmdev.h])

 AS_IF(
  [test "x$ac_cv_header_libsmdev_h" = xno],
  [ac_libewf_have_libsmdev=no],
  [ac_libewf_have_libsmdev=yes
  AC_CHECK_LIB(
   smdev,
   libsmdev_get_version,
   [],
   [ac_libewf_have_libsmdev=no])

  AC_CHECK_LIB(
   smdev,
   libsmdev_check_device,
   [],
   [ac_libewf_have_libsmdev=no])

  AS_IF(
   [test "x$ac_cv_libewf_enable_wide_character_type" != xno],
   [AC_CHECK_LIB(
    smdev,
    libsmdev_check_device_wide,
    [ac_libewf_dummy=yes],
    [ac_libewf_have_libsmdev=no])
   ])

  dnl Handle functions
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_initialize,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_free,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_signal_abort,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])

  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_open,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_close,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_read_buffer,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_seek_offset,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])

  AS_IF(
   [test "x$ac_cv_libewf_enable_wide_character_type" != xno],
   [AC_CHECK_LIB(
    smdev,
    libsmdev_handle_open_wide,
    [ac_libewf_dummy=yes],
    [ac_libewf_have_libsmdev=no])
   ])

  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_get_bus_type,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_get_bytes_per_sector,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_get_error,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_get_utf8_information_value,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_get_media_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_get_media_type,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_get_number_of_errors,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_get_number_of_sessions,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_get_session,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_get_number_of_tracks,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_get_track,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])

  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_set_error_flags,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_set_error_granularity,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  AC_CHECK_LIB(
   smdev,
   libsmdev_handle_set_number_of_error_retries,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmdev=no])
  ])
 ])

dnl Function to detect if libsmraw available
AC_DEFUN([LIBEWF_CHECK_LIBSMRAW],
 [AC_CHECK_HEADERS([libsmraw.h])

 AS_IF(
  [test "x$ac_cv_header_libsmraw_h" = xno],
  [ac_libewf_have_libsmraw=no],
  [ac_libewf_have_libsmraw=yes
  AC_CHECK_LIB(
   smraw,
   libsmraw_get_version,
   [],
   [ac_libewf_have_libsmraw=no])

  dnl Handle functions
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_initialize,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_free,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_signal_abort,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_open,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_close,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_read_buffer,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_seek_offset,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_write_buffer,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])

  AS_IF(
   [test "x$ac_cv_libewf_enable_wide_character_type" != xno],
   [AC_CHECK_LIB(
    smraw,
    libsmraw_handle_open_wide,
    [ac_libewf_dummy=yes],
    [ac_libewf_have_libsmraw=no])
   ])

  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_get_media_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_get_bytes_per_sector,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_get_utf8_information_value
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_get_utf16_information_value
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])

  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_set_maximum_segment_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_set_media_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_set_utf8_integrity_hash_value
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  AC_CHECK_LIB(
   smraw,
   libsmraw_handle_set_utf16_integrity_hash_value
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libsmraw=no])
  ])
 ])

dnl Function to detect if libodraw available
AC_DEFUN([LIBEWF_CHECK_LIBODRAW],
 [AC_CHECK_HEADERS([libodraw.h])

 AS_IF(
  [test "x$ac_cv_header_libodraw_h" = xno],
  [ac_libewf_have_libodraw=no],
  [ac_libewf_have_libodraw=yes
  AC_CHECK_LIB(
   odraw,
   libodraw_get_version,
   [],
   [ac_libewf_have_libodraw=no])

  dnl Handle functions
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_initialize,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_free,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_signal_abort,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_open,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_open_data_files,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_close,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_read_buffer,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_seek_offset,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])

  AS_IF(
   [test "x$ac_cv_libewf_enable_wide_character_type" != xno],
   [AC_CHECK_LIB(
    odraw,
    libodraw_handle_open_wide,
    [ac_libewf_dummy=yes],
    [ac_libewf_have_libodraw=no])
   ])

  AC_CHECK_LIB(
   odraw,
   libodraw_handle_get_bytes_per_sector,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_get_data_file,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_get_media_size,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_get_number_of_data_files,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_get_number_of_sessions,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_get_session,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_get_number_of_tracks,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_handle_get_track,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])

  dnl Data file functions
  AC_CHECK_LIB(
   odraw,
   libodraw_data_file_free,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])
  AC_CHECK_LIB(
   odraw,
   libodraw_data_file_set_filename,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libodraw=no])

  AS_IF(
   [test "x$ac_cv_libewf_enable_wide_character_type" != xno],
   [AC_CHECK_LIB(
    odraw,
    libodraw_data_file_set_filename_wide,
    [ac_libewf_dummy=yes],
    [ac_libewf_have_libodraw=no])
   ])
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

dnl Function to detect if Python build environment is available
AC_DEFUN([LIBEWF_CHECK_PYTHON_DEVEL],
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

