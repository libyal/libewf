AC_PREREQ( 2.59 )

AC_INIT(
 [libewf],
 [20111020],
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
AX_COMMON_ARG_ENABLE(
 [guid],
 [guid],
 [enable GUID/UUID support in the ewftools],
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

dnl Check if WINAPI support should be enabled
AX_COMMON_CHECK_ENABLE_WINAPI

dnl Check if wide character type should be enabled
AX_COMMON_CHECK_ENABLE_WIDE_CHARACTER_TYPE

dnl Check for type definitions
AX_TYPES_CHECK_LOCAL

dnl Checks for libraries and library functions
AX_COMMON_CHECK_LOCAL
AX_LIBCSTRING_CHECK_LOCAL
AX_LIBERROR_CHECK_LOCAL
AX_LIBNOTIFY_CHECK_LOCAL
AX_LIBSYSTEM_CHECK_LOCAL

dnl Check for zlib compression support
dnl ac_zlib_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AX_COMMON_ARG_WITH(
 [zlib],
 [zlib],
 [search for zlib in includedir and libdir or in the specified DIR],
 [auto-detect],
 [DIR])

AS_IF(
 [test "x$ac_cv_with_zlib" != x && test "x$ac_cv_with_zlib" != xauto-detect],
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
 [ac_zlib_dummy=yes],
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
 [ac_zlib_dummy=yes],
 [AC_MSG_FAILURE(
  [Missing function: uncompress in library: zlib.],
  [1])
 ])

dnl Check for enabling libuna for Unicode string support
AX_LIBUNA_CHECK_ENABLE

dnl Check for enabling libbfio for basic file IO support
AX_LIBBFIO_CHECK_ENABLE

dnl Check for enabling libmfdata for multi-file data support
AX_LIBMFDATA_CHECK_ENABLE

dnl Check for enabling libfvalue for file value support
AX_LIBFVALUE_CHECK_ENABLE

dnl Check for enabling libhmac for message digest hash function support
AX_LIBHMAC_CHECK_ENABLE

dnl Check for enabling libodraw for optical disc (split) RAW support
AX_LIBODRAW_CHECK_ENABLE

dnl Check for enabling libsmdev for storage media device support
AX_LIBSMDEV_CHECK_ENABLE

dnl Check for enabling libsmraw for storage media (split) RAW support
AX_LIBSMRAW_CHECK_ENABLE

dnl Check for enabling GUID/UUID support
AS_IF(
 [test "x$ac_cv_enable_guid" = xno],
 [ac_cv_libuuid=no],
 [AX_LIBUUID_CHECK_ENABLE])

dnl Check for libfuse Filesystem in Userspace (FUSE) support
AX_LIBFUSE_CHECK_ENABLE

dnl Check for library function support
AX_LIBEWF_CHECK_LOCAL

dnl Check for headers
AC_CHECK_HEADERS([sys/utsname.h])

dnl Check for functions
AS_IF(
 [test "x$ac_cv_enable_winapi" = xno],
 [AC_CHECK_FUNCS([getegid geteuid getuid uname])
 ])

dnl Check if ewftools should be build as static executables
AX_COMMON_CHECK_ENABLE_STATIC_EXECUTABLES

dnl Check if ewftools should be build with low level read and write functions
AX_LIBEWF_CHECK_ENABLE_LOW_LEVEL_FUNCTIONS

dnl Check if libewf should be build with verbose output
AX_COMMON_CHECK_ENABLE_VERBOSE_OUTPUT

dnl Check if libewf should be build with debug output
AX_COMMON_CHECK_ENABLE_DEBUG_OUTPUT

dnl Check if libewf python bindings (pyewf) should be build
AX_PYTHON_CHECK_ENABLE

dnl Check if libewf should be build with version 1 API
AX_LIBEWF_CHECK_ENABLE_V1_API

dnl Check for the host operation system will be used as a fall back in the ewftools
AX_LIBEWF_CHECK_HOST_OPERATING_SYSTEM

dnl Set memwatch support
ac_cv_enable_memwatch=no

AS_IF(
 [test "x$ac_cv_enable_memwatch" != xno],
 [AC_SUBST(
  [MEMWATCH_CPPFLAGS],
  ["-DMEMWATCH -DMEMWATCH_STDIO"])
 AC_SUBST(
  [MEMWATCH_LIBADD],
  [../common/libmemwatch.la])
 ])

AM_CONDITIONAL(
 [HAVE_MEMWATCH],
 [test "x$ac_cv_enable_memwatch" != xno])

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

dnl Check if DLL support is needed in ewftools
AS_IF(
 [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
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

