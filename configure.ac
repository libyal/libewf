AC_PREREQ( 2.59 )

AC_INIT(
 [libewf],
 [20110908],
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
COMMON_ARG_ENABLE(
 [wide-character-type],
 [wide_character_type],
 [enable wide character type support],
 [no])
COMMON_ARG_ENABLE(
 [winapi],
 [winapi],
 [enable WINAPI support for cross-compilation],
 [auto-detect])
COMMON_ARG_ENABLE(
 [static-executables],
 [static_executables],
 [build the ewftools as static executables],
 [no])
COMMON_ARG_ENABLE(
 [guid],
 [guid],
 [enable GUID/UUID support in the ewftools],
 [yes])
COMMON_ARG_ENABLE(
 [low-level-functions],
 [low_level_functions],
 [use libewf's low level read and write functions in the ewftools],
 [no])
COMMON_ARG_ENABLE(
 [verbose-output],
 [verbose_output],
 [enable verbose output],
 [no])
COMMON_ARG_ENABLE(
 [debug-output],
 [debug_output],
 [enable debug output],
 [no])
COMMON_ARG_ENABLE(
 [python],
 [python],
 [build python bindings (pyewf)],
 [no])
COMMON_ARG_ENABLE(
 [v1-api],
 [v1_api],
 [enable version 1 API],
 [yes])

COMMON_ARG_WITH(
 [libuna],
 [libuna],
 [search for libuna in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [libbfio],
 [libbfio],
 [search for libbfio in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [libfvalue],
 [libfvalue],
 [search for libfvalue in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [libmfdata],
 [libmfdata],
 [search for libmfdata in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [zlib],
 [zlib],
 [search for zlib in includedir and libdir or in the specified DIR],
 [auto-detect],
 [DIR])

COMMON_ARG_WITH(
 [libhmac],
 [libhmac],
 [search for libhmac in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [openssl],
 [openssl],
 [search for openssl in includedir and libdir or in the specified DIR, or no if not to use openssl],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [libodraw],
 [libodraw],
 [search for libodraw in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [libmsdev],
 [libmsdev],
 [search for libmsdev in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [libmsraw],
 [libmsraw],
 [search for libmsdev in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [libuuid],
 [libuuid],
 [search for libuuid in includedir and libdir or in the specified DIR, or no if not to use libuuid],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [libfuse],
 [libfuse],
 [search for libfuse in includedir and libdir or in the specified DIR, or no if not to use libfuse],
 [auto-detect],
 [DIR])

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
AC_CHECK_WINAPI

dnl Check for type definitions
AC_CHECK_LOCAL_TYPES

dnl TODO move to sub m4 file, add comment where it's used or remove
AC_STRUCT_TM

dnl Check for headers
dnl Headers included in libewf/libewf_date_time.h
AC_HEADER_TIME

dnl Headers included in ewftools/platform.c
AC_CHECK_HEADERS([sys/utsname.h])

dnl Other headers

dnl Check for functions
dnl Date and time functions used in libewf/libewf_date_time.h
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

dnl Check for internationalization functions in libewf/libewf_i18n.c 
AC_CHECK_FUNCS([bindtextdomain])

dnl System functions
AS_IF(
 [test "x$ac_cv_enable_winapi" = xno],
 [AC_CHECK_FUNCS(
  [geteuid],
  [],
  [AC_MSG_FAILURE(
   [Missing function: geteuid],
   [1]) ])

 AC_CHECK_FUNCS(
  [getuid],
  [],
  [AC_MSG_FAILURE(
   [Missing function: getuid],
   [1]) ])
 ])

AC_CHECK_FUNCS([uname])

dnl Check for printf conversion specifier support
AC_CHECK_FUNC_PRINTF_JD
AC_CHECK_FUNC_PRINTF_ZD

dnl Checks for libraries and library functions
dnl ac_libewf_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_CHECK_LOCAL_COMMON
AC_CHECK_LOCAL_LIBCSTRING
AC_CHECK_LOCAL_LIBERROR
AC_CHECK_LOCAL_LIBNOTIFY
AC_CHECK_LOCAL_LIBSYSTEM

dnl Check for zlib compression support
AS_IF(
 [test x"$ac_cv_with_zlib" != x && test "x$ac_cv_with_zlib" != xauto-detect],
 [AS_IF(
  [test -d "$ac_cv_with_zlib"],
  [CFLAGS="$CFLAGS -I${ac_cv_with_zlib}/include"
  LDFLAGS="$LDFLAGS -L${ac_cv_with_zlib}/lib"],
  [AC_MSG_WARN([no such directory: $ac_cv_with_zlib])
  ])
 ])

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
AC_CHECK_LIBUNA

AS_IF(
 [test "x$ac_cv_libuna" = xyes],
 [AC_SUBST(
  [libewf_pc_libs_private_libuna],
  [-luna])
 ])

AS_IF(
 [test "x$ac_cv_libuna" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libuna],
  [libuna])
 AC_SUBST(
  [libewf_spec_build_requires_libuna],
  [libuna-devel])
 ])

AS_IF(
 [test "x$ac_cv_libuna" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBUNA],
  [1],
  [Define to 1 if the local version of libuna is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBUNA],
  [1])
 AC_SUBST(
  [LIBUNA_CPPFLAGS],
  [-I../libuna])
 AC_SUBST(
  [LIBUNA_LIBADD],
  [../libuna/libuna.la])
 ac_cv_libuna=local
 ])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBUNA],
 [test "x$ac_cv_libuna" = xlocal])

dnl Check for enabling libbfio for basic file IO support
AC_CHECK_LIBBFIO

AS_IF(
 [test "x$ac_cv_libbfio" = xyes],
 [AC_SUBST(
  [libewf_pc_libs_private_libbfio],
  [-lbfio])
 ])

AS_IF(
 [test "x$ac_cv_libbfio" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libbfio],
  [libbfio])
 AC_SUBST(
  [libewf_spec_build_requires_libbfio],
  [libbfio-devel])
 ])

AS_IF(
 [test "x$ac_cv_libbfio" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBBFIO],
  [1],
  [Define to 1 if the local version of libbfio is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBBFIO],
  [1])
 AC_SUBST(
  [LIBBFIO_CPPFLAGS],
  [-I../libbfio])
 AC_SUBST(
  [LIBBFIO_LIBADD],
  [../libbfio/libbfio.la])
 ac_cv_libbfio=local
 ])

AS_IF(
 [test "x$ac_cv_libbfio" = xlocal],
 [AC_CHECK_LOCAL_LIBBFIO])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBBFIO],
 [test "x$ac_cv_libbfio" = xlocal])

dnl Check for enabling libmfdata for multi-file data support
AC_CHECK_LIBMFDATA

AS_IF(
 [test "x$ac_cv_libmfdata" = xyes],
 [AC_SUBST(
  [libewf_pc_libs_private_libmfdata],
  [-lmfdata])
 ])

AS_IF(
 [test "x$ac_cv_libmfdata" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libmfdata],
  [libmfdata])
 AC_SUBST(
  [libewf_spec_build_requires_libmfdata],
  [libmfdata-devel])
 ])

AS_IF(
 [test "x$ac_cv_libmfdata" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBMFDATA],
  [1],
  [Define to 1 if the local version of libmfdata is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBMFDATA],
  [1])
 AC_SUBST(
  [LIBMFDATA_CPPFLAGS],
  [-I../libmfdata])
 AC_SUBST(
  [LIBMFDATA_LIBADD],
  [../libmfdata/libmfdata.la])
 ac_cv_libmfdata=local
 ])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBMFDATA],
 [test "x$ac_cv_libmfdata" = xlocal])

dnl Check for enabling libfvalue for file value support
AC_CHECK_LIBFVALUE

AS_IF(
 [test "x$ac_cv_libfvalue" = xyes],
 [AC_SUBST(
  [libewf_pc_libs_private_libfvalue],
  [-lfvalue])
 ])

AS_IF(
 [test "x$ac_cv_libfvalue" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libfvalue],
  [libfvalue])
 AC_SUBST(
  [libewf_spec_build_requires_libfvalue],
  [libfvalue-devel])
 ])

AS_IF(
 [test "x$ac_cv_libfvalue" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBFVALUE],
  [1],
  [Define to 1 if the local version of libfvalue is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBFVALUE],
  [1])
 AC_SUBST(
  [LIBFVALUE_CPPFLAGS],
  [-I../libfvalue])
 AC_SUBST(
  [LIBFVALUE_LIBADD],
  [../libfvalue/libfvalue.la])
 ac_cv_libfvalue=local
 ])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBFVALUE],
 [test "x$ac_cv_libfvalue" = xlocal])

dnl Check for enabling libhmac for message digest hash function support
AC_CHECK_LIBHMAC

AS_IF(
 [test "x$ac_cv_libhmac" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libhmac],
  [libhmac])
 AC_SUBST(
  [libewf_spec_build_requires_libhmac],
  [libhmac-devel])
 ])

AS_IF(
 [test "x$ac_cv_libhmac" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBHMAC],
  [1],
  [Define to 1 if the local version of libhmac is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBHMAC],
  [1])
 AC_SUBST(
  [LIBHMAC_CPPFLAGS],
  [-I../libhmac])
 AC_SUBST(
  [LIBHMAC_LIBADD],
  [../libhmac/libhmac.la])
 ac_cv_libhmac=local
 ])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBHMAC],
 [test "x$ac_cv_libhmac" = xlocal])

AS_IF(
 [test "x$ac_cv_libhmac" = xlocal],
 [AC_CHECK_LOCAL_LIBHMAC])

AS_IF(
 [test "x$ac_cv_libcrypto" != xno],
 [AC_SUBST(
  [libewf_spec_requires_libcrypto],
  [openssl])
 AC_SUBST(
  [libewf_spec_build_requires_libcrypto],
  [openssl-devel])
 ])

dnl Check for enabling libodraw for optical disc (split) RAW support
AC_CHECK_LIBODRAW

AS_IF(
 [test "x$ac_cv_libodraw" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libodraw],
  [libodraw])
 AC_SUBST(
  [libewf_spec_build_requires_libodraw],
  [libodraw-devel])
 ])

AS_IF(
 [test "x$ac_cv_libodraw" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBODRAW],
  [1],
  [Define to 1 if the local version of libodraw is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBODRAW],
  [1])
 AC_SUBST(
  [LIBODRAW_CPPFLAGS],
  [-I../libodraw])
 AC_SUBST(
  [LIBODRAW_LIBADD],
  [../libodraw/libodraw.la])
 ac_cv_libodraw=local
 ])

AS_IF(
 [test "x$ac_cv_libodraw" = xlocal],
 [AC_CHECK_LOCAL_LIBODRAW])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBODRAW],
 [test "x$ac_cv_libodraw" = xlocal])

dnl Check for enabling libsmdev for storage media device support
AC_CHECK_LIBSMDEV

AS_IF(
 [test "x$ac_cv_libsmdev" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libsmdev],
  [libsmdev])
 AC_SUBST(
  [libewf_spec_build_requires_libsmdev],
  [libsmdev-devel])
 ])

AS_IF(
 [test "x$ac_cv_libsmdev" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBSMDEV],
  [1],
  [Define to 1 if the local version of libsmdev is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBSMDEV],
  [1])
 AC_SUBST(
  [LIBSMDEV_CPPFLAGS],
  [-I../libsmdev])
 AC_SUBST(
  [LIBSMDEV_LIBADD],
  [../libsmdev/libsmdev.la])
 ac_cv_libsmdev=local
 ])

AS_IF(
 [test "x$ac_cv_libsmdev" = xlocal],
 [AC_CHECK_LOCAL_LIBSMDEV])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBSMDEV],
 [test "x$ac_cv_libsmdev" = xlocal])

dnl Check for enabling libsmraw for storage media (split) RAW support
AC_CHECK_LIBSMRAW

AS_IF(
 [test "x$ac_cv_libsmraw" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libsmraw],
  [libsmraw])
 AC_SUBST(
  [libewf_spec_build_requires_libsmraw],
  [libsmraw-devel])
 ])

AS_IF(
 [test "x$ac_cv_libsmraw" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBSMRAW],
  [1],
  [Define to 1 if the local version of libsmraw is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBSMRAW],
  [1])
 AC_SUBST(
  [LIBSMRAW_CPPFLAGS],
  [-I../libsmraw])
 AC_SUBST(
  [LIBSMRAW_LIBADD],
  [../libsmraw/libsmraw.la])
 ac_cv_libsmraw=local
 ])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBSMRAW],
 [test "x$ac_cv_libsmraw" = xlocal])

dnl Check for enabling GUID/UUID support
AS_IF(
 [test "x$ac_cv_enable_guid" = xyes],
 [AC_CHECK_LIBUUID])

AS_IF(
 [test "x$ac_cv_libuuid" = xlibuuid],
 [AC_SUBST(
  [libewf_spec_requires_libuuid],
  [libuuid])
 AC_SUBST(
  [libewf_spec_build_requires_libuuid],
  [libuuid-devel])
 ])

dnl Check for libfuse Filesystem in Userspace (FUSE) support
AC_CHECK_LIBFUSE

AS_IF(
 [test "x$ac_cv_libfuse" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libfuse],
  [libfuse])
 AC_SUBST(
  [libewf_spec_build_requires_libfuse],
  [libfuse-devel])
 ])

dnl Check if ewftools should be build as static executables
AS_IF(
 [test "x$ac_cv_enable_static_executables" != xno],
 [STATIC_LDFLAGS="-all-static";
 AC_SUBST(
  [STATIC_LDFLAGS])
 AC_DEFINE(
  [HAVE_STATIC_EXECUTABLES],
  [1],
  [Define if executables should be static compiled.])
 ac_cv_enable_static_executables=yes])

dnl Check if ewftools should be build with low level read and write functions
AS_IF(
 [test "x$ac_cv_enable_low_level_functions" != xno],
 [AC_DEFINE(
  [HAVE_LOW_LEVEL_FUNCTIONS],
  [1],
  [Define to 1 if libewfs low level read and write functions should be used in the ewf tools.])
 ac_cv_enable_low_level_functions=yes])

dnl Check if libewf should be build with verbose output
AS_IF(
 [test "x$ac_cv_enable_verbose_output" != xno],
 [AC_DEFINE(
  [HAVE_VERBOSE_OUTPUT],
  [1],
  [Define to 1 if verbose output should be used.])
 ac_cv_enable_verbose_output=yes])

dnl Check if libewf should be build with debug output
AS_IF(
 [test "x$ac_cv_enable_debug_output" != xno],
 [AC_DEFINE(
  [HAVE_DEBUG_OUTPUT],
  [1],
  [Define to 1 if debug output should be used.])
 ac_cv_enable_debug_output=yes])

dnl Check if libewf python bindings (pyewf) should be build
AS_IF(
 [test "x$ac_cv_enable_python" != xno],
 [AM_PATH_PYTHON([2.5])
 AC_CHECK_PYTHON_DEVEL
 ac_cv_enable_python=yes])

AS_IF(
 [test "x$ac_cv_enable_python" = xyes],
 [AC_DEFINE(
  [HAVE_PYTHON],
  [1],
  [Have Python bindings])
 ])

AM_CONDITIONAL(
 HAVE_PYTHON,
 [test "x$ac_cv_enable_python" = xyes])

dnl Check if libewf should be build with version 1 API
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
  [../common/libmemwatch.la])
 ])

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
AC_CONFIG_FILES([libhmac/Makefile])
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
   libuna support:                                  $ac_cv_libuna
   libbfio support:                                 $ac_cv_libbfio
   libmfdata support:                               $ac_cv_libmfdata
   libfvalue support:                               $ac_cv_libfvalue
   libhmac support:                                 $ac_cv_libhmac
   libodraw support:                                $ac_cv_libodraw
   libsmdev support:                                $ac_cv_libsmdev
   libsmraw support:                                $ac_cv_libsmraw
   MD5 support:                                     $ac_cv_libhmac_md5
   SHA1 support:                                    $ac_cv_libhmac_sha1
   SHA256 support:                                  $ac_cv_libhmac_sha256
   GUID/UUID support:                               $ac_cv_libuuid
   FUSE support:                                    $ac_cv_libfuse

Features:
   Wide character type support:                     $ac_cv_enable_wide_character_type
   ewftools are build as static executables:        $ac_cv_enable_static_executables
   ewftools use low level read and write functions: $ac_cv_enable_low_level_functions
   Python (pyewf) support:                          $ac_cv_enable_python
   Verbose output:                                  $ac_cv_enable_verbose_output
   Debug output:                                    $ac_cv_enable_debug_output
   Version 1 API compatibility:                     $ac_cv_enable_v1_api
]);

