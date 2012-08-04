AC_PREREQ( 2.59 )

AC_INIT(
 [libewf],
 [20120804],
 [joachim.metz@gmail.com])

AC_CONFIG_SRCDIR(
 [include/libewf.h.in])

AM_INIT_AUTOMAKE([gnu 1.6])

AC_CONFIG_MACRO_DIR([m4])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check for libtool DLL support
AC_LIBTOOL_WIN32_DLL

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL

dnl Check for libtool
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Check for pkg-config
AC_PATH_PROG(PKGCONFIG,[pkg-config])

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

dnl Check for enabling libcstring for cross-platform C string functions
AX_LIBCSTRING_CHECK_ENABLE

dnl Check for enabling libcerror for cross-platform C error functions
AX_LIBCERROR_CHECK_ENABLE

dnl Check for enabling libclocale for cross-platform C locale functions
AX_LIBCLOCALE_CHECK_ENABLE

dnl Check for enabling libcnotify for cross-platform C notification functions
AX_LIBCNOTIFY_CHECK_ENABLE

dnl Check for enabling libcsplit for cross-platform C split string functions
AX_LIBCSPLIT_CHECK_ENABLE

dnl Check for enabling libuna for Unicode string support
AX_LIBUNA_CHECK_ENABLE

dnl Check for enabling libcfile for cross-platform C file functions
AX_LIBCFILE_CHECK_ENABLE

dnl Check for enabling libcpath for cross-platform C path functions
AX_LIBCPATH_CHECK_ENABLE

dnl Check for enabling libbfio for basic file IO support
AX_LIBBFIO_CHECK_ENABLE

dnl Check for enabling libfvalue for file value support
AX_LIBFVALUE_CHECK_ENABLE

dnl Check for enabling libmfcache for multi-file value cache support
AX_LIBMFCACHE_CHECK_ENABLE

dnl Check for enabling libmfdata for multi-file data types support
AX_LIBMFDATA_CHECK_ENABLE

dnl Check for zlib compression support
AX_ZLIB_CHECK_ENABLE

dnl Check for bz2lib compression support
AX_BZIP2_CHECK_ENABLE

dnl Check for enabling libhmac for message digest hash function support
AX_LIBHMAC_CHECK_ENABLE

dnl Check for library function support
AX_LIBEWF_CHECK_LOCAL

dnl Check for enabling libcsystem for cross-platform C generic system functions
AX_LIBCSYSTEM_CHECK_ENABLE

dnl Check for enabling GUID/UUID support
AX_LIBUUID_CHECK_ENABLE

dnl Check for enabling libodraw for optical disc (split) RAW support
AX_LIBODRAW_CHECK_ENABLE

dnl Check for enabling libsmdev for storage media device support
AX_LIBSMDEV_CHECK_ENABLE

dnl Check for enabling libsmraw for storage media (split) RAW support
AX_LIBSMRAW_CHECK_ENABLE

dnl Check for libfuse Filesystem in Userspace (FUSE) support
AX_LIBFUSE_CHECK_ENABLE

dnl Check for headers and functions
AS_IF(
 [test "x$ac_cv_enable_winapi" = xno],
 [AC_CHECK_HEADERS([sys/resource.h sys/utsname.h])
 AC_CHECK_FUNCS([getegid geteuid getrlimit getuid uname])
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

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

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

dnl Check if requires and build requires should be set in spec file
AS_IF(
 [test "x$ac_cv_libcstring" = xyes || test "x$ac_cv_libcerror" = xyes || test "x$ac_cv_libclocale" = xyes || test "x$ac_cv_libcnotify" = xyes || test "x$ac_cv_libcsplit" = xyes || test "x$ac_cv_libuna" = xyes || test "x$ac_cv_libcfile" = xyes || test "x$ac_cv_libcpath" = xyes || test "x$ac_cv_libbfio" = xyes || test "x$ac_cv_libfvalue" = xyes || test "x$ac_cv_libmfcache" = xyes || test "x$ac_cv_libmfdata" = xyes || test "x$ac_cv_zlib" != xno || test "x$ac_cv_bzip2" != xno],
 [AC_SUBST(
  [libewf_spec_requires],
  [Requires:])
 AC_SUBST(
  [libewf_spec_build_requires],
  [BuildRequires:])
 ])

dnl Set the date for dpkg files
AC_SUBST(
 [DATE],
 [`date -R 2> /dev/null`])

dnl Set the current working directory for PackageMaker files
AC_SUBST(
 [PWD],
 [`pwd`])

dnl Have configure make the Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([common/Makefile])
AC_CONFIG_FILES([libcstring/Makefile])
AC_CONFIG_FILES([libcerror/Makefile])
AC_CONFIG_FILES([libclocale/Makefile])
AC_CONFIG_FILES([libcnotify/Makefile])
AC_CONFIG_FILES([libcsplit/Makefile])
AC_CONFIG_FILES([libuna/Makefile])
AC_CONFIG_FILES([libcfile/Makefile])
AC_CONFIG_FILES([libcpath/Makefile])
AC_CONFIG_FILES([libbfio/Makefile])
AC_CONFIG_FILES([libfvalue/Makefile])
AC_CONFIG_FILES([libmfcache/Makefile])
AC_CONFIG_FILES([libmfdata/Makefile])
AC_CONFIG_FILES([libhmac/Makefile])
AC_CONFIG_FILES([libewf/Makefile])
AC_CONFIG_FILES([libodraw/Makefile])
AC_CONFIG_FILES([libsmdev/Makefile])
AC_CONFIG_FILES([libsmraw/Makefile])
AC_CONFIG_FILES([libcsystem/Makefile])
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
AC_CONFIG_FILES([dpkg/changelog])
AC_CONFIG_FILES([dpkg/copyright])
AC_CONFIG_FILES([libewf/libewf.rc])
AC_CONFIG_FILES([ewf.net/ewf.net.rc])
AC_CONFIG_FILES([macosx/libewf.pmdoc/01bin.xml])
AC_CONFIG_FILES([macosx/libewf.pmdoc/02include.xml])
AC_CONFIG_FILES([macosx/libewf.pmdoc/03lib.xml])
AC_CONFIG_FILES([macosx/libewf.pmdoc/04share.xml])
AC_CONFIG_FILES([macosx/libewf.pmdoc/index.xml])
AC_CONFIG_FILES([macosx/Introduction.rtf])
AC_CONFIG_FILES([libewf.pc])
AC_CONFIG_FILES([libewf.spec])
dnl Have configure write its configuration
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libcstring support:                              $ac_cv_libcstring
   libcerror support:                               $ac_cv_libcerror
   libclocale support:                              $ac_cv_libclocale
   libcnotify support:                              $ac_cv_libcnotify
   libcsplit support:                               $ac_cv_libcsplit
   libuna support:                                  $ac_cv_libuna
   libcfile support:                                $ac_cv_libcfile
   libcpath support:                                $ac_cv_libcpath
   libbfio support:                                 $ac_cv_libbfio
   libfvalue support:                               $ac_cv_libfvalue
   libmfcache support:                              $ac_cv_libmfcache
   libmfdata support:                               $ac_cv_libmfdata
   ADLER32 checksum support:                        $ac_cv_with_adler32
   DEFLATE compression support:                     $ac_cv_zlib
   BZIP2 compression support:                       $ac_cv_bzip2
   libhmac support:                                 $ac_cv_libhmac
   MD5 support:                                     $ac_cv_libhmac_md5
   SHA1 support:                                    $ac_cv_libhmac_sha1
   SHA256 support:                                  $ac_cv_libhmac_sha256
   libodraw support:                                $ac_cv_libodraw
   libsmdev support:                                $ac_cv_libsmdev
   libsmraw support:                                $ac_cv_libsmraw
   libcsystem support:                              $ac_cv_libcsystem
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

