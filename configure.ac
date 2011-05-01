AC_PREREQ( 2.59 )

AC_INIT(
 [libewf],
 [20110501],
 [jbmetz@users.sourceforge.net])

AC_CONFIG_SRCDIR(
 [include/libewf.h.in])

AM_INIT_AUTOMAKE([gnu 1.6])

AC_CONFIG_MACRO_DIR([m4])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check for configure arguments
LIBEWF_TEST_ENABLE(
 [wide-character-type],
 [wide_character_type],
 [enable wide character type support],
 [no])
LIBEWF_TEST_ENABLE(
 [winapi],
 [winapi],
 [enable WINAPI support for cross-compilation],
 [auto-detect])
LIBEWF_TEST_ENABLE(
 [static-executables],
 [static_executables],
 [build the ewftools as static executables],
 [no])
LIBEWF_TEST_ENABLE(
 [guid],
 [guid],
 [enable GUID/UUID support in the ewftools],
 [yes])
LIBEWF_TEST_ENABLE(
 [low-level-functions],
 [low_level_functions],
 [use libewf's low level read and write functions in the ewftools],
 [no])
LIBEWF_TEST_ENABLE(
 [verbose-output],
 [verbose_output],
 [enable verbose output],
 [no])
LIBEWF_TEST_ENABLE(
 [debug-output],
 [debug_output],
 [enable debug output],
 [no])
LIBEWF_TEST_ENABLE(
 [python],
 [python],
 [build python bindings (pyewf)],
 [no])
LIBEWF_TEST_ENABLE(
 [v1-api],
 [v1_api],
 [enable version 1 API],
 [yes])

dnl Check for libtool DLL support
AC_LIBTOOL_WIN32_DLL

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Support of internationalization (i18n)
AM_GNU_GETTEXT([external])

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Automatically enable winapi support for mingw
AS_IF(
 [test "x$ac_cv_libewf_enable_winapi" = xauto-detect],
 [ac_libewf_target_string="$target";

 AS_IF(
  [test "x$ac_libewf_target_string" = x],
  [ac_libewf_target_string="$build"])

 AS_CASE(
  [$ac_libewf_target_string],
  [*mingw*],[AC_MSG_NOTICE(
             [Detected MinGW enabling WINAPI support for cross-compilation])
            ac_cv_libewf_enable_winapi=yes],
  [*],[ac_cv_libewf_enable_winapi=no])
 ])

dnl Check for headers
dnl Headers included in include/libewf/types.h
AC_CHECK_HEADERS([sys/types.h inttypes.h stdint.h])

AS_IF(
 [test "x$ac_cv_header_sys_types_h" = xyes],
 [AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [0])])

AS_IF(
 [test "x$ac_cv_header_inttypes_h" = xyes],
 [AC_SUBST(
  [HAVE_INTTYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_INTTYPES_H],
  [0])])

AS_IF(
 [test "x$ac_cv_header_stdint_h" = xyes],
 [AC_SUBST(
  [HAVE_STDINT_H],
  [1])],
 [AC_SUBST(
  [HAVE_STDINT_H],
  [0])])

dnl Headers included in common/common.h
AS_IF(
 [test "x$ac_cv_libewf_enable_winapi" = xyes],
 [AC_CHECK_HEADERS([windows.h])

 AS_IF(
  [test "x$ac_cv_header_windows_h" = xno],
  [AC_MSG_FAILURE(
   [Missing header: windows.h header is required to compile with winapi support],
   [1]) ])
 ])

AS_IF(
 [test "x$ac_cv_libewf_enable_winapi" = xno],
 [AC_CHECK_HEADERS([libintl.h]) ])

dnl Headers included in common/types.h
AC_CHECK_HEADERS([limits.h])

dnl Headers included in common/memory.h and libcstring/libcstring_narrow_string.h
AC_CHECK_HEADERS([stdlib.h string.h])

dnl Headers included in libcstring/libcstring_locale.c
AC_CHECK_HEADERS([langinfo.h locale.h])

dnl Headers included in liberror/liberror_error.c and libnotify/libnotify_print.c
AC_CHECK_HEADERS([stdarg.h])

AS_IF(
 [test "x$ac_cv_header_stdarg_h" != xyes],
 [AC_CHECK_HEADERS([varargs.h]) ])

AS_IF(
 [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
 [AC_MSG_FAILURE(
  [Missing headers: stdarg.h and varargs.h],
  [1]) ])

dnl Headers included in libnotify/libnotify_stream.c, libbfio/libbfio_file.c
dnl libsystem/libsystem_file.h and libsystem/libsystem_string.h
AC_CHECK_HEADERS([errno.h])

dnl Headers included in libbfio/libbfio_file.c, libsystem/libsystem_directory_io.h, libsystem/libsystem_file_io.h
dnl libsmdev/libsmdev_handle.c and libsmdev/libsmdev_support.c
AC_CHECK_HEADERS([sys/stat.h])

dnl Headers included in libbfio/libbfio_file.c, libsystem/libsystem_file_io.h
dnl libsmdev/libsmdev_handle.c and libsmdev/libsmdev_support.c
AC_CHECK_HEADERS([fcntl.h unistd.h])

dnl Headers included in libewf/libewf_date_time.h
AC_HEADER_TIME

dnl Headers included in libsystem/libsystem_glob.h
AC_CHECK_HEADERS([glob.h])

AS_IF(
 [test "x$ac_cv_header_glob_h" = xno],
 [AC_CHECK_HEADERS([io.h]) ])

dnl Headers included in libsystem/libsystem_signal.h
AC_CHECK_HEADERS([signal.h sys/signal.h])

dnl Headers included in ewftools/platform.c
AC_CHECK_HEADERS([sys/utsname.h])

dnl Other headers

dnl Check for type definitions
AC_STRUCT_TM

AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

AC_CHECK_TYPE(
 [size32_t],
 [AC_SUBST(
  [HAVE_SIZE32_T],
  [1])],
 [AC_SUBST(
  [HAVE_SIZE32_T],
  [0])])

AC_CHECK_TYPE(
 [ssize32_t],
 [AC_SUBST(
  [HAVE_SSIZE32_T],
  [1])],
 [AC_SUBST(
  [HAVE_SSIZE32_T],
  [0])])

AC_CHECK_TYPE(
 [size64_t],
 [AC_SUBST(
  [HAVE_SIZE64_T],
  [1])],
 [AC_SUBST(
  [HAVE_SIZE64_T],
  [0])])

AC_CHECK_TYPE(
 [ssize64_t],
 [AC_SUBST(
  [HAVE_SSIZE64_T],
  [1])],
 [AC_SUBST(
  [HAVE_SSIZE64_T],
  [0])])

AC_CHECK_TYPE(
 [off64_t],
 [AC_SUBST(
  [HAVE_OFF64_T],
  [1])],
 [AC_SUBST(
  [HAVE_OFF64_T],
  [0])])

AC_CHECK_TYPE([ssize_t])
AC_CHECK_TYPE([u64])

AC_CHECK_SIZEOF([off_t])
AC_CHECK_SIZEOF([size_t])

dnl Check for functions
dnl File stream functions used in common/file_stream.h
AC_CHECK_FUNCS(
 [fclose],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fclose],
  [1]) ])

AC_CHECK_FUNCS(
 [feof],
 [],
 [AC_MSG_FAILURE(
  [Missing function: feof],
  [1]) ])

AC_CHECK_FUNCS(
 [fgets],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fgets],
  [1]) ])

AC_CHECK_FUNCS(
 [fopen],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fopen],
  [1]) ])

AC_CHECK_FUNCS(
 [fread],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fread],
  [1]) ])

AC_CHECK_FUNCS(
 [fseeko],
 [],
 [AC_CHECK_FUNCS(
  [fseeko64],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fseeko and fseeko64],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [fwrite],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fwrite],
  [1]) ])

AC_CHECK_FUNCS(
 [vfprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: vfprintf],
  [1]) ])

dnl Memory functions used in common/memory.h
AC_CHECK_FUNCS(
 [free],
 [],
 [AC_MSG_FAILURE(
  [Missing function: free],
  [1]) ])

AC_CHECK_FUNCS(
 [malloc],
 [],
 [AC_MSG_FAILURE(
  [Missing function: malloc],
  [1]) ])

AC_CHECK_FUNCS(
 [memcmp],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memcmp],
  [1]) ])

AC_CHECK_FUNCS(
 [memcpy],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memcpy],
  [1]) ])

AC_CHECK_FUNCS(
 [memset],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memset],
  [1]) ])

dnl TODO re-enable in the future
dnl AC_CHECK_FUNCS([posix_memalign])

AC_CHECK_FUNCS(
 [realloc],
 [],
 [AC_MSG_FAILURE(
  [Missing function: realloc],
  [1]) ])

dnl Check for environment functions in libcstring/libcstring_locale.c
AC_CHECK_FUNCS(
 [getenv],
 [],
 [AC_MSG_FAILURE(
  [Missing function: getenv],
  [1]) ])

dnl Check for localization functions in libcstring/libcstring_locale.c
AS_IF(
 [test "x$ac_cv_libewf_enable_winapi" = xno],
 [AC_CHECK_FUNCS(
  [localeconv],
  [],
  [AC_MSG_FAILURE(
   [Missing function: localeconv],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [setlocale],
 [],
 [AC_MSG_FAILURE(
  [Missing function: setlocale],
  [1]) ])

LIBEWF_CHECK_FUNC_LANGINFO_CODESET

dnl Narrow character string functions used in libcstring/libcstring_narrow_string.h
AC_CHECK_FUNCS(
 [fgets],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fgets],
  [1]) ])

AC_CHECK_FUNCS(
 [memchr],
 [],
 [AC_CHECK_FUNCS(
  [strchr],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: memchr and strchr],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [memcmp],
 [],
 [AC_CHECK_FUNCS(
  [strncmp],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: memcmp and strncmp],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [memcpy],
 [],
 [AC_CHECK_FUNCS(
  [strncpy],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: memcpy and strncpy],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [memrchr],
 [AC_CHECK_DECLS(
  [memrchr],
  [ac_libewf_have_memrchr=yes],
  [ac_libewf_have_memrchr=no]) ],
 [ac_libewf_have_memrchr=no])

AS_IF(
 [test "x$ac_libewf_have_memrchr" = xno],
 [AC_CHECK_FUNCS(
  [strrchr],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strrchr and memrchr],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [snprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: snprintf],
  [1]) ])

AC_CHECK_FUNCS(
 [sscanf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: sscanf],
  [1]) ])

AC_CHECK_FUNCS(
 [strlen],
 [],
 [AC_MSG_FAILURE(
  [Missing function: strlen],
  [1]) ])

AC_CHECK_FUNCS(
 [strncasecmp],
 [],
 [AC_CHECK_FUNCS(
  [strcasecmp],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strncasecmp and strcasecmp],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strstr],
 [],
 [AC_MSG_FAILURE(
  [Missing function: strstr],
  [1]) ])

AC_CHECK_FUNCS(
 [strtoll],
 [],
 [AC_CHECK_FUNCS(
  [atoll],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strtoll and atoll],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strtoull],
 [],
 [AC_CHECK_FUNCS(
  [atoll],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strtoull and atoll],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [vsnprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: vsnprintf],
  [1]) ])

dnl File input/output functions used in libbfio/libbfio_file.h and libsystem/libsystem_file_io.h
AC_CHECK_FUNCS(
 [close],
 [],
 [AC_MSG_FAILURE(
  [Missing function: close],
  [1]) ])

AC_CHECK_FUNCS(
 [fstat],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fstat],
  [1]) ])

AC_CHECK_FUNCS(
 [ftruncate],
 [],
 [AC_MSG_FAILURE(
  [Missing function: ftruncate],
  [1]) ])

AC_CHECK_FUNCS(
 [lseek],
 [],
 [AC_MSG_FAILURE(
  [Missing function: lseek],
  [1]) ])

AC_CHECK_FUNCS(
 [open],
 [],
 [AC_MSG_FAILURE(
  [Missing function: open],
  [1]) ])

AC_CHECK_FUNCS(
 [read],
 [],
 [AC_MSG_FAILURE(
  [Missing function: read],
  [1]) ])

AC_CHECK_FUNCS(
 [stat],
 [],
 [AC_MSG_FAILURE(
  [Missing function: stat],
  [1]) ])

AC_CHECK_FUNCS(
 [write],
 [],
 [AC_MSG_FAILURE(
  [Missing function: write],
  [1]) ])

dnl Date and time functions used in libewf/libewf_date_time.h and libsystem/libsystem_date_time.c
AC_CHECK_FUNCS(
 [localtime_r],
 [],
 [AC_CHECK_FUNCS(
  [localtime],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: localtime_r and localtime],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [mktime],
 [],
 [AC_MSG_FAILURE(
  [Missing function: mktime],
  [1]) ])

dnl Date and time functions used in libsystem/libsystem_date_time.h
LIBEWF_CHECK_FUNC_CTIME

AC_CHECK_FUNCS(
 [gmtime_r],
 [],
 [AC_CHECK_FUNCS(
  [gmtime],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: gmtime_r and gmtime],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [time],
 [],
 [AC_MSG_FAILURE(
  [Missing function: time],
  [1]) ])

dnl Directory functions used in libsystem/libsystem_directory_io.h
AC_CHECK_FUNCS(
 [chdir],
 [],
 [AC_MSG_FAILURE(
  [Missing functions: chdir],
  [1]) ])

LIBEWF_CHECK_FUNC_MKDIR

dnl Check for error string functions used in libbfio/libbfio_error_string.c and libsystem/libsystem_error_string.c
AC_FUNC_STRERROR_R()

AS_IF(
 [test "x$ac_cv_have_decl_strerror_r" = xno],
 [AC_CHECK_FUNCS(
  [strerror],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strerror_r and strerror],
   [1]) ]) ])

dnl Commandline argument/option parsing functions in libsystem/libsystem_getopt.h
AC_CHECK_FUNCS([getopt])

dnl Check for internationalization functions in <library>/<library>_i18n.c and libsystem/libsystem_support.c
AC_CHECK_FUNCS([bindtextdomain])

dnl Check for internationalization functions in libsystem/libsystem_support.c
AC_CHECK_FUNCS([textdomain])

dnl System functions
AC_CHECK_FUNCS([uname])

dnl Check for printf conversion specifier support
LIBEWF_CHECK_FUNC_PRINTF_JD
LIBEWF_CHECK_FUNC_PRINTF_ZD

dnl Checks for libraries and library functions
dnl ac_libewf_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments

dnl Check for zlib compression support
AC_CHECK_HEADERS([zlib.h])

AC_CHECK_LIB(
 z,
 adler32,
 [],
 [AC_MSG_FAILURE(
  [Missing function: adler32 in library: zlib.],
  [1])
 ])

AC_CHECK_LIB(
 z,
 compress2,
 [ac_libewf_dummy=yes],
 [AC_MSG_FAILURE(
  [Missing function: compress2 in library: zlib.],
  [1])
 ])

AC_CHECK_LIB(
 z,
 compressBound,
 [AC_DEFINE(
  [HAVE_COMPRESS_BOUND],
  [1],
  [Define to 1 if compressBound funtion is available in zlib.])
 ])

AC_CHECK_LIB(
 z,
 uncompress,
 [ac_libewf_dummy=yes],
 [AC_MSG_FAILURE(
  [Missing function: uncompress in library: zlib.],
  [1])
 ])

dnl Check for enabling libuna for Unicode string support
LIBEWF_CHECK_LIBUNA

AS_IF(
 [test "x$ac_libewf_have_libuna" = xyes],
 [dnl libuna is available
 AC_SUBST(
  [libewf_pc_libs_private_libuna],
  [-luna])
 AC_SUBST(
  [libewf_spec_requires_libuna],
  [libuna])
 AC_SUBST(
  [libewf_spec_build_requires_libuna],
  [libuna-devel]) ],
 [dnl libuna is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBUNA],
  [1],
  [Define to 1 if the local version of libuna is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBUNA],
  [1])
 AC_SUBST(
  [libewf_pc_libs_private_libuna],
  [])
 AC_SUBST(
  [libewf_spec_requires_libuna],
  [])
 AC_SUBST(
  [libewf_spec_build_requires_libuna],
  [])
 AC_SUBST(
  [LIBUNA_CPPFLAGS],
  [-I../libuna])
 AC_SUBST(
  [LIBUNA_LIBADD],
  [../libuna/libuna.la])
 ac_libewf_have_libuna=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBUNA],
 [test "x$ac_libewf_have_libuna" = xlocal])

dnl Check for enabling libbfio for basic file IO support
LIBEWF_CHECK_LIBBFIO

AS_IF(
 [test "x$ac_libewf_have_libbfio" = xyes],
 [dnl libbfio is available
 AC_SUBST(
  [libewf_pc_libs_private_libbfio],
  [-lbfio])
 AC_SUBST(
  [libewf_spec_requires_libbfio],
  [libbfio])
 AC_SUBST(
  [libewf_spec_build_requires_libbfio],
  [libbfio-devel])
 AC_SUBST(
  [LIBBFIO_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBBFIO_LIBADD],
  [])
 AC_SUBST(
  [HAVE_LIBBFIO],
  [1]) ],
 [dnl libbfio is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBBFIO],
  [1],
  [Define to 1 if the local version of libbfio is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBBFIO],
  [1])
 AC_SUBST(
  [libewf_pc_libs_private_libbfio],
  [])
 AC_SUBST(
  [libewf_spec_requires_libbfio],
  [])
 AC_SUBST(
  [libewf_spec_build_requires_libbfio],
  [])
 AC_SUBST(
  [LIBBFIO_CPPFLAGS],
  [-I../libbfio])
 AC_SUBST(
  [LIBBFIO_LIBADD],
  [../libbfio/libbfio.la])
 AC_SUBST(
  [HAVE_LIBBFIO],
  [0])
 ac_libewf_have_libbfio=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBBFIO],
 [test "x$ac_libewf_have_libbfio" = xlocal])

dnl Check for enabling libmfdata for multi-file data support
LIBEWF_CHECK_LIBMFDATA

AS_IF(
[test "x$ac_libewf_have_libmfdata" = xyes],
 [dnl libmfdata is available
 AC_SUBST(
  [libewf_pc_libs_private_libmfdata],
  [-lmfdata])
 AC_SUBST(
  [libewf_spec_requires_libmfdata],
  [libmfdata])
 AC_SUBST(
  [libewf_spec_build_requires_libmfdata],
  [libmfdata-devel])
 AC_SUBST(
  [LIBMFDATA_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBMFDATA_LIBADD],
  []) ],
 [dnl libmfdata is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBMFDATA],
  [1],
  [Define to 1 if the local version of libmfdata is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBMFDATA],
  [1])
 AC_SUBST(
  [libewf_pc_libs_private_libmfdata],
  [])
 AC_SUBST(
  [libewf_spec_requires_libmfdata],
  [])
 AC_SUBST(
  [libewf_spec_build_requires_libmfdata],
  [])
 AC_SUBST(
  [LIBMFDATA_CPPFLAGS],
  [-I../libmfdata])
 AC_SUBST(
  [LIBMFDATA_LIBADD],
  [../libmfdata/libmfdata.la])
 ac_libewf_have_libmfdata=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBMFDATA],
 [test "x$ac_libewf_have_libmfdata" = xlocal])

dnl Check for enabling libfvalue for file value support
LIBEWF_CHECK_LIBFVALUE

AS_IF(
[test "x$ac_libewf_have_libfvalue" = xyes],
 [dnl libfvalue is available
 AC_SUBST(
  [libewf_pc_libs_private_libfvalue],
  [-lfvalue])
 AC_SUBST(
  [libewf_spec_requires_libfvalue],
  [libfvalue])
 AC_SUBST(
  [libewf_spec_build_requires_libfvalue],
  [libfvalue-devel])
 AC_SUBST(
  [LIBFVALUE_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBFVALUE_LIBADD],
  []) ],
 [dnl libfvalue is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBFVALUE],
  [1],
  [Define to 1 if the local version of libfvalue is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBFVALUE],
  [1])
 AC_SUBST(
  [libewf_pc_libs_private_libfvalue],
  [])
 AC_SUBST(
  [libewf_spec_requires_libfvalue],
  [])
 AC_SUBST(
  [libewf_spec_build_requires_libfvalue],
  [])
 AC_SUBST(
  [LIBFVALUE_CPPFLAGS],
  [-I../libfvalue])
 AC_SUBST(
  [LIBFVALUE_LIBADD],
  [../libfvalue/libfvalue.la])
 ac_libewf_have_libfvalue=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBFVALUE],
 [test "x$ac_libewf_have_libfvalue" = xlocal])

dnl Check for enabling libsmdev for storage media device support
LIBEWF_CHECK_LIBSMDEV

AS_IF(
 [test "x$ac_libewf_have_libsmdev" = xyes],
 [dnl libsmdev is available
 AC_SUBST(
  [libewf_spec_requires_libsmdev],
  [libsmdev])
 AC_SUBST(
  [libewf_spec_build_requires_libsmdev],
  [libsmdev-devel])
 AC_SUBST(
  [LIBSMDEV_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBSMDEV_LIBADD],
  [])
 AC_SUBST(
  [HAVE_LIBSMDEV],
  [1]) ],
 [dnl libsmdev is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBSMDEV],
  [1],
  [Define to 1 if the local version of libsmdev is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBSMDEV],
  [1])
 AC_SUBST(
  [libewf_spec_requires_libsmdev],
  [])
 AC_SUBST(
  [libewf_spec_build_requires_libsmdev],
  [])
 AC_SUBST(
  [LIBSMDEV_CPPFLAGS],
  [-I../libsmdev])
 AC_SUBST(
  [LIBSMDEV_LIBADD],
  [../libsmdev/libsmdev.la])
 AC_SUBST(
  [HAVE_LIBSMDEV],
  [0])
 ac_libewf_have_libsmdev=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBSMDEV],
 [test "x$ac_libewf_have_libsmdev" = xlocal])

AS_IF(
 [test "x$ac_libewf_have_libsmdev" = xlocal],
 [dnl Headers included in libsmdev/libsmdev_metadata.c
 AS_IF(
  [test "x$ac_cv_libewf_enable_winapi" = xno],
  [AC_CHECK_HEADERS([cygwin/fs.h linux/fs.h sys/disk.h sys/disklabel.h sys/ioctl.h])
 ])

 dnl Headers included in libsmdev/libsmdev_ata.c
 AS_IF(
  [test "x$ac_cv_libewf_enable_winapi" = xno],
  [AC_CHECK_HEADERS([cygwin/hdreg.h linux/hdreg.h])
 ])

 dnl Headers included in libsmdev/libsmdev_scsi.c
 AS_IF(
  [test "x$ac_cv_libewf_enable_winapi" = xno],
  [AC_CHECK_HEADERS([scsi/scsi.h scsi/scsi_ioctl.h scsi/sg.h])
 ])

 dnl Headers included in libsmdev/libsmdev_optical_disk.c
 AS_IF(
  [test "x$ac_cv_libewf_enable_winapi" = xno],
  [AC_CHECK_HEADERS([linux/cdrom.h])
 ])

 dnl Headers included in libsmdev/libsmdev_usb.c
 AS_IF(
  [test "x$ac_cv_libewf_enable_winapi" = xno],
  [AC_CHECK_HEADERS([linux/usbdevice_fs.h linux/usb/ch9.h])
 ])

 LIBEWF_CHECK_FUNC_POSIX_FADVISE

 dnl Check if winioctl.h defines STORAGE_BUS_TYPE
 AS_IF(
  [test "x$ac_cv_libewf_enable_winapi" = xyes],
  [AC_CACHE_CHECK(
   [whether winioctl.h defines STORAGE_BUS_TYPE],
   [ac_cv_libewf_have_winioctl_storage_bus_type],
   [AC_LANG_PUSH(C)
   AC_COMPILE_IFELSE(
    AC_LANG_PROGRAM(
     [[#include <windows.h>
#include <winioctl.h>]],
     [[STORAGE_BUS_TYPE storage_bus_type;
storage_bus_type = BusTypeUnknown;]]),
    [ac_cv_libewf_have_winioctl_storage_bus_type=yes],
    [ac_cv_libewf_have_winioctl_storage_bus_type=no])
   AC_LANG_POP(C)],
   [ac_cv_libewf_have_winioctl_storage_bus_type=no])
  AS_IF(
   [test "x$ac_cv_libewf_have_winioctl_storage_bus_type" = xyes],
   [AC_DEFINE(
    [HAVE_STORAGE_BUS_TYPE],
    [1],
    [Define to 1 if STORAGE_BUS_TYPE is defined.])
   ])
  ])
 ])

dnl Check for enabling libsmraw for storage media (split) RAW support
LIBEWF_CHECK_LIBSMRAW

AS_IF(
 [test "x$ac_libewf_have_libsmraw" = xyes],
 [dnl libsmraw is available
 AC_SUBST(
  [libewf_spec_requires_libsmraw],
  [libsmraw])
 AC_SUBST(
  [libewf_spec_build_requires_libsmraw],
  [libsmraw-devel])
 AC_SUBST(
  [LIBSMRAW_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBSMRAW_LIBADD],
  [])
 AC_SUBST(
  [HAVE_LIBSMRAW],
  [1]) ],
 [dnl libsmraw is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBSMRAW],
  [1],
  [Define to 1 if the local version of libsmraw is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBSMRAW],
  [1])
 AC_SUBST(
  [libewf_spec_requires_libsmraw],
  [])
 AC_SUBST(
  [libewf_spec_build_requires_libsmraw],
  [])
 AC_SUBST(
  [LIBSMRAW_CPPFLAGS],
  [-I../libsmraw])
 AC_SUBST(
  [LIBSMRAW_LIBADD],
  [../libsmraw/libsmraw.la])
 AC_SUBST(
  [HAVE_LIBSMRAW],
  [0])
 ac_libewf_have_libsmraw=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBSMRAW],
 [test "x$ac_libewf_have_libsmraw" = xlocal])

dnl Check for enabling libodraw for optical disc (split) RAW support support
LIBEWF_CHECK_LIBODRAW

AS_IF(
 [test "x$ac_libewf_have_libodraw" = xyes],
 [dnl libodraw is available
 AC_SUBST(
  [libewf_spec_requires_libodraw],
  [libodraw])
 AC_SUBST(
  [libewf_spec_build_requires_libodraw],
  [libodraw-devel])
 AC_SUBST(
  [LIBODRAW_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBODRAW_LIBADD],
  [])
 AC_SUBST(
  [HAVE_LIBODRAW],
  [1]) ],
 [dnl libodraw is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBODRAW],
  [1],
  [Define to 1 if the local version of libodraw is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBODRAW],
  [1])
 AC_SUBST(
  [libewf_spec_requires_libodraw],
  [])
 AC_SUBST(
  [libewf_spec_build_requires_libodraw],
  [])
 AC_SUBST(
  [LIBODRAW_CPPFLAGS],
  [-I../libodraw])
 AC_SUBST(
  [LIBODRAW_LIBADD],
  [../libodraw/libodraw.la])
 AC_SUBST(
  [HAVE_LIBODRAW],
  [0])
 ac_libewf_have_libodraw=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBODRAW],
 [test "x$ac_libewf_have_libodraw" = xlocal])

AS_IF(
 [test "x$ac_libewf_have_libodraw" = xlocal],
 [AM_PROG_LEX
 AC_PROG_YACC])

dnl Check for enabling crypographic digest hash support
ac_libewf_have_libcrypto=no

AS_IF(
 [test "x$ac_cv_libewf_enable_winapi" = xyes],
 [AC_SUBST(
  [LIBCRYPTO_LIBADD],
  [-ladvapi32])
  ac_libewf_have_libcrypto=libadvapi32])

dnl Check for libcrypto openssl EVP support
AS_IF(
 [test "x$ac_libewf_have_libcrypto" = xno],
 [AC_CHECK_HEADERS([openssl/evp.h openssl/opensslv.h])

 AS_IF(
  [test "x$ac_cv_header_openssl_evp_h" = xyes],
  [AC_LANG_PUSH(C)
  AC_MSG_CHECKING(
   [if openssl/evp.h can be used in combination with zlib.h])
  AC_LINK_IFELSE(
   AC_LANG_PROGRAM(
    [[#include <zlib.h>
#include <openssl/evp.h>]],
    [[ ]]),
    [AC_MSG_RESULT(
     [yes])
    AC_DEFINE(
     [HAVE_OPENSSL_EVP_H],
     [1],
     [Define to 1 if you have the <openssl/evp.h> header file.])
    AC_SUBST(
     [HAVE_OPENSSL_EVP_H],
     [1])
    ac_libewf_have_libcrypto=libcrypto_evp],
   [AC_MSG_RESULT(
    [no])
   ac_libewf_have_libcrypto=no])
  AC_LANG_POP(C)],
  [ac_libewf_have_libcrypto=no])
 ])

dnl Check if all required libcrypto openssl EVP functions are available
AS_IF(
 [test "x$ac_libewf_have_libcrypto" = xlibcrypto_evp],
 [AC_CHECK_LIB(
  crypto,
  EVP_md5,
  [ac_libewf_dummy=yes],
  [ac_libewf_have_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_sha1,
  [ac_libewf_dummy=yes],
  [ac_libewf_have_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_MD_CTX_init,
  [ac_libewf_dummy=yes],
  [ac_libewf_have_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_MD_CTX_cleanup,
  [ac_libewf_dummy=yes],
  [ac_libewf_have_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestInit_ex,
  [ac_libewf_dummy=yes],
  [ac_libewf_have_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestUpdate,
  [ac_libewf_dummy=yes],
  [ac_libewf_have_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestFinal_ex,
  [ac_libewf_dummy=yes],
  [ac_libewf_have_libcrypto=no])
 ])

AS_IF(
 [test "x$ac_libewf_have_libcrypto" = xlibadvapi32],
 [ac_libewf_have_libcrypto_md5=libadvapi32
 ac_libewf_have_libcrypto_sha1=libadvapi32])

AS_IF(
 [test "x$ac_libewf_have_libcrypto" = xlibcrypto_evp],
 [ac_libewf_have_libcrypto_md5=libcrypto_evp
 ac_libewf_have_libcrypto_sha1=libcrypto_evp],
 [AC_SUBST(
  [HAVE_OPENSSL_EVP_H],
  [0])
 ])

AS_IF(
 [test "x$ac_libewf_have_libcrypto" = xno],
 [ac_libewf_have_libcrypto_md5=no
 ac_libewf_have_libcrypto_sha1=no])

dnl Check for libcrypto openssl MD5 support
AS_IF(
 [test "x$ac_libewf_have_libcrypto_md5" = xno],
 [AC_CHECK_HEADERS([openssl/md5.h])

 AS_IF(
  [test "x$ac_cv_header_openssl_md5_h" = xyes],
  [ac_libewf_have_libcrypto_md5=libcrypto
  AC_CHECK_LIB(
   crypto,
   MD5_Init,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libcrypto_md5=no])
  AC_CHECK_LIB(
   crypto,
   MD5_Update,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libcrypto_md5=no])
  AC_CHECK_LIB(
   crypto,
   MD5_Final,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libcrypto_md5=no])
  ])
 ])

AS_IF(
 [test "x$ac_libewf_have_libcrypto_md5" = xno],
 [AC_MSG_NOTICE(
  [No support for calculating MD5 hashes, if required install (openssl) libcrypto with MD5 support or Windows Crypto API.])
 ])

dnl Check for libcrypto SHA1 support
AS_IF(
 [test "x$ac_libewf_have_libcrypto_sha1" = xno],
 [AC_CHECK_HEADERS([openssl/sha.h])

 AS_IF(
  [test "x$ac_cv_header_openssl_sha1_h" = xyes],
  [ac_libewf_have_libcrypto_sha1=libcrypto
  AC_CHECK_LIB(
   crypto,
   SHA1_Init,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libcrypto_sha1=no])
  AC_CHECK_LIB(
   crypto,
   SHA1_Update,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libcrypto_sha1=no])
  AC_CHECK_LIB(
   crypto,
   SHA1_Final,
   [ac_libewf_dummy=yes],
   [ac_libewf_have_libcrypto_sha1=no])
  ])
 ])

AS_IF(
 [test "x$ac_libewf_have_libcrypto_sha1" = xno],
 [AC_MSG_NOTICE(
  [No support for calculating SHA1 hashes, if required install (openssl) libcrypto with SHA1 support or Windows Crypto API.])
 ])

AS_IF(
 [test "x$ac_libewf_have_libcrypto_md5" = xlibcrypto_evp || test "x$ac_libewf_have_libcrypto_md5" = xlibcrypto],
 [AC_SUBST(
  [LIBCRYPTO_LIBADD],
  ["-lcrypto"])
 AC_DEFINE(
  [HAVE_LIBCRYPTO],
  [1],
  [Define to 1 if you have the 'crypto' library (-lcrypto).])
 dnl Enforce the dynamic loader library to be included if available
 AC_CHECK_LIB(
  dl,
  dlopen,
  [AC_SUBST(
   [LIBDL_LIBADD],
   ["-ldl"])],
  [])
 ])

dnl Check for enabling GUID/UUID support
AS_IF(
 [test "x$ac_cv_libewf_enable_guid" = xyes],
 [AS_IF(
  [test "x$ac_cv_libewf_enable_winapi" = xyes],
  [AC_SUBST(
   [LIBUUID_LIBADD],
   [-lrpcrt4])
   ac_libewf_have_guid_support=librpcrt4],
  [AC_CHECK_HEADERS(
   [uuid/uuid.h],
   [],
   [ac_libewf_have_guid_support=no])
  AS_IF(
   [test "x$ac_libewf_have_guid_support" != xno],
   [ac_libewf_have_guid_support=native
   AC_CHECK_FUNC(
    [uuid_generate],
    [],
    [ac_libewf_have_guid_support=libuuid
     AC_CHECK_LIB(
     [uuid],
     [uuid_generate],
     [AC_SUBST(
      [LIBUUID_LIBADD],
      [-luuid])
     AC_DEFINE(
      [HAVE_LIBUUID],
      [1],
      [Define to 1 if you have the 'uuid' library (-luuid).]) ],
     [ac_libewf_have_guid_support=no])
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_libewf_have_guid_support" != xno],
  [AC_DEFINE(
   [HAVE_GUID_SUPPORT],
   [1],
   [Define to 1 if GUID/UUID support is available.])
  ])
 ],
 [ac_libewf_have_guid_support=disabled])

AS_IF(
 [test "x$ac_libewf_have_guid_support" = xnative],
 [AC_CHECK_FUNC(
  [uuid_generate_random],
  [AC_DEFINE(
   [HAVE_UUID_GENERATE_RANDOM],
   [1],
   [Define to 1 if uuid_generate_random funtion is available.])
  ])
 AC_CHECK_FUNC(
  [uuid_generate_time],
  [AC_DEFINE(
   [HAVE_UUID_GENERATE_TIME],
   [1],
   [Define to 1 if uuid_generate_time funtion is available.])
  ])
 ])

AS_IF(
 [test "x$ac_libewf_have_guid_support" = xlibuuid],
 [AC_CHECK_LIB(
  [uuid],
  [uuid_generate_random],
  [AC_DEFINE(
   [HAVE_UUID_GENERATE_RANDOM],
   [1],
   [Define to 1 if uuid_generate_random funtion is available in libuuid.])
  ])
 AC_CHECK_LIB(
  [uuid],
  [uuid_generate_time],
  [AC_DEFINE(
   [HAVE_UUID_GENERATE_TIME],
   [1],
   [Define to 1 if uuid_generate_time funtion is available in libuuid.])
  ])
 AC_SUBST(
  [libewf_spec_requires_libuuid],
  [libuuid])
 AC_SUBST(
  [libewf_spec_build_requires_libuuid],
  [libuuid-devel]) ],
 [AC_SUBST(
  [libewf_spec_requires_libuuid],
  [])
 AC_SUBST(
  [libewf_spec_build_requires_libuuid],
  [])
 ac_libewf_dummy=yes])

dnl Check if libewf should be build with wide character type support
AS_IF(
 [test "x$ac_cv_libewf_enable_winapi" = xyes],
 [ac_cv_libewf_enable_wide_character_type=yes])

AS_IF(
 [test "x$ac_cv_libewf_enable_wide_character_type" != xno],
 [dnl Check for the wide character header
 AC_CHECK_HEADERS(
  [wchar.h],
  [AC_DEFINE(
   [HAVE_WCHAR_H],
   [1],
   [Define to 1 if you have the <wchar.h> header file.])
  AC_SUBST(
   [HAVE_WCHAR_H],
   [1]) ],
  [AC_MSG_FAILURE(
   [Missing header: wchar.h],
   [1]) ])

 dnl Determine wide character byte size
 AC_CHECK_SIZEOF([wchar_t])

 dnl Check for wide character string functions

 dnl Wide character string functions used in common/file_stream.h
 AC_CHECK_FUNCS(
  [fgetws],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fgetws],
   [1]) ])

 dnl Wide character string functions used in libcstring/libcstring_wide_string.h
 AC_CHECK_FUNCS(
  [swprintf],
  [],
  [AC_MSG_FAILURE(
   [Missing function: swprintf],
   [1]) ])

 AC_CHECK_FUNCS(
  [wmemchr],
  [],
  [AC_CHECK_FUNCS(
   [wcschr],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: wmemchr and wcschr],
    [1]) ]) ])

 AC_CHECK_FUNCS(
  [wmemcmp],
  [],
  [AC_CHECK_FUNCS(
   [wcsncmp],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: wmemcmp and wcsncmp],
    [1]) ]) ])

 AC_CHECK_FUNCS(
  [wmemcpy],
  [],
  [AC_CHECK_FUNCS(
   [wcsncpy],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: wmemcpy and wcsncpy],
    [1]) ]) ])

 AC_CHECK_FUNCS(
  [wmemrchr],
  [],
  [AC_CHECK_FUNCS(
   [wcsrchr],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: wmemrchr and wcsrchr],
    [1]) ]) ])

 AC_CHECK_FUNCS(
  [wcslen],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcslen],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [wcsncasecmp],
  [],
  [AC_CHECK_FUNCS(
   [wcscasecmp],
   [],
   [AC_CHECK_FUNCS(
    [towlower],
    [],
    [AC_MSG_FAILURE(
     [Missing functions: wcsncasecmp, wcscasecmp and towlower],
     [1]) ]) ]) ])

 AC_CHECK_FUNCS(
  [wcsstr],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcsstr],
   [1]) ])

 AC_CHECK_FUNCS(
  [wcstoll],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcstoll],
   [1]) ])

 AC_CHECK_FUNCS(
  [wcstoull],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcstoull],
   [1]) ])

 dnl Wide character string functions used in liberror/liberror_error.h
 AC_CHECK_FUNCS(
  [wcstombs],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcstombs],
   [1]) ])

 dnl Make the necessary definitions
 AC_DEFINE(
  [HAVE_WIDE_CHARACTER_TYPE],
  [1],
  [Define to 1 if wide character type should be used.])
 AC_SUBST(
  [HAVE_WIDE_CHARACTER_TYPE],
  [1])
 ac_cv_libewf_enable_wide_character_type=yes],

 [AC_SUBST(
  [HAVE_WIDE_CHARACTER_TYPE],
  [0])
 AC_SUBST(
  [HAVE_WCHAR_H],
  [0]) ])

dnl Check if ewftools should be build as static executables
AS_IF(
 [test "x$ac_cv_libewf_enable_static_executables" != xno],
 [STATIC_LDFLAGS="-all-static";
 AC_SUBST(
  [STATIC_LDFLAGS])
 AC_DEFINE(
  [HAVE_STATIC_EXECUTABLES],
  [1],
  [Define if executables should be static compiled.])
 ac_cv_libewf_enable_static_executables=yes])

dnl Check if ewftools should be build with low level read and write functions
AS_IF(
 [test "x$ac_cv_libewf_enable_low_level_functions" != xno],
 [AC_DEFINE(
  [HAVE_LOW_LEVEL_FUNCTIONS],
  [1],
  [Define to 1 if libewfs low level read and write functions should be used in the ewf tools.])
 ac_cv_libewf_enable_low_level_functions=yes])

dnl Check if libewf should be build with verbose output
AS_IF(
 [test "x$ac_cv_libewf_enable_verbose_output" != xno],
 [AC_DEFINE(
  [HAVE_VERBOSE_OUTPUT],
  [1],
  [Define to 1 if verbose output should be used.])
 ac_cv_libewf_enable_verbose_output=yes])

dnl Check if libewf should be build with debug output
AS_IF(
 [test "x$ac_cv_libewf_enable_debug_output" != xno],
 [AC_DEFINE(
  [HAVE_DEBUG_OUTPUT],
  [1],
  [Define to 1 if debug output should be used.])
 ac_cv_libewf_enable_debug_output=yes])

dnl Check if libewf python bindings (pyewf) should be build
AS_IF(
 [test "x$ac_cv_libewf_enable_python" != xno],
 [AM_PATH_PYTHON([2.5])
 LIBEWF_CHECK_PYTHON_DEVEL()
 AC_DEFINE(
  [HAVE_PYTHON],
  [1],
  [Have Python bindings])
 ac_cv_libewf_enable_python=yes])

AM_CONDITIONAL(
 HAVE_PYTHON,
 [test "x$ac_cv_libewf_enable_python" != xno])

dnl Check if libewf should be build with version 1 API
AS_IF(
 [test "x$ac_cv_libewf_enable_v1_api" != xno],
 [AC_DEFINE(
  [HAVE_V1_API],
  [1],
  [Define to 1 if the version 1 API should be available.])
 AC_SUBST(
  [HAVE_V1_API],
  [1])
 ac_cv_libewf_enable_v1_api=yes],
 [AC_SUBST(
  [HAVE_V1_API],
  [0]) ])

AM_CONDITIONAL(
 HAVE_V1_API,
 [test "x$ac_cv_libewf_enable_v1_api" != xno])

dnl Check for the operation system will be used as a fall back in the ewftools
LIBEWF_DETERMINE_OPERATING_SYSTEM

dnl Set memwatch support
ac_libewf_have_memwatch=no

AS_IF(
 [test "x$ac_libewf_have_memwatch" != xno],
 [AC_SUBST(
  [MEMWATCH_CPPFLAGS],
  ["-DMEMWATCH -DMEMWATCH_STDIO"])
 AC_SUBST(
  [MEMWATCH_LIBADD],
  [../common/libmemwatch.la]) ])

AM_CONDITIONAL(
 [HAVE_MEMWATCH],
 [test "x$ac_libewf_have_memwatch" != xno])

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

dnl Check if DLL support is needed in ewftools
AS_IF(
 [test x"$enable_shared" = xyes],
 [AS_CASE(
  [$host],
  [*cygwin* | *mingw*],
  [AC_SUBST(
   [LIBEWF_DLL_IMPORT],
   ["-DLIBEWF_DLL_IMPORT"])
  ])
 ])

dnl Have configure make the Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([common/Makefile])
AC_CONFIG_FILES([libcstring/Makefile])
AC_CONFIG_FILES([liberror/Makefile])
AC_CONFIG_FILES([libnotify/Makefile])
AC_CONFIG_FILES([libuna/Makefile])
AC_CONFIG_FILES([libbfio/Makefile])
AC_CONFIG_FILES([libmfdata/Makefile])
AC_CONFIG_FILES([libfvalue/Makefile])
AC_CONFIG_FILES([libewf/Makefile])
AC_CONFIG_FILES([libodraw/Makefile])
AC_CONFIG_FILES([libsmdev/Makefile])
AC_CONFIG_FILES([libsmraw/Makefile])
AC_CONFIG_FILES([libsystem/Makefile])
AC_CONFIG_FILES([ewftools/Makefile])
AC_CONFIG_FILES([ewf.net/Makefile])
AC_CONFIG_FILES([pyewf/Makefile])
AC_CONFIG_FILES([po/Makefile.in])
AC_CONFIG_FILES([po/Makevars])
AC_CONFIG_FILES([documents/Makefile])
AC_CONFIG_FILES([manuals/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([examples/Makefile])
AC_CONFIG_FILES([macosx/Makefile])
AC_CONFIG_FILES([borlandc/Makefile])
AC_CONFIG_FILES([msvscpp/Makefile])
dnl Have configure make the include files
AC_CONFIG_FILES([include/libewf.h])
AC_CONFIG_FILES([include/libewf/definitions.h])
AC_CONFIG_FILES([include/libewf/features.h])
AC_CONFIG_FILES([include/libewf/types.h])
AC_CONFIG_FILES([libewf/libewf_definitions.h])
dnl Have configure make distribution specific files
AC_CONFIG_FILES([common/types.h])
AC_CONFIG_FILES([libewf/libewf.rc])
AC_CONFIG_FILES([ewf.net/ewf.net.rc])
AC_CONFIG_FILES([macosx/Introduction.rtf])
AC_CONFIG_FILES([libewf.pc])
AC_CONFIG_FILES([libewf.spec])
dnl Have configure write its configuration
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libuna support:                                  $ac_libewf_have_libuna
   libbfio support:                                 $ac_libewf_have_libbfio
   libmfdata support:                               $ac_libewf_have_libmfdata
   libfvalue support:                               $ac_libewf_have_libfvalue
   libodraw support:                                $ac_libewf_have_libodraw
   libsmdev support:                                $ac_libewf_have_libsmdev
   libsmraw support:                                $ac_libewf_have_libsmraw
   MD5 support:                                     $ac_libewf_have_libcrypto_md5
   SHA1 support:                                    $ac_libewf_have_libcrypto_sha1
   GUID/UUID support:                               $ac_libewf_have_guid_support

Features:
   Wide character type support:                     $ac_cv_libewf_enable_wide_character_type
   ewftools are build as static executables:        $ac_cv_libewf_enable_static_executables
   ewftools use low level read and write functions: $ac_cv_libewf_enable_low_level_functions
   Python (pyewf) support:                          $ac_cv_libewf_enable_python
   Verbose output:                                  $ac_cv_libewf_enable_verbose_output
   Debug output:                                    $ac_cv_libewf_enable_debug_output
   Version 1 API compatibility:                     $ac_cv_libewf_enable_v1_api
]);

