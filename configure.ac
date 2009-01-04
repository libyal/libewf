AC_PREREQ( 2.59 )

AC_INIT([libewf], [20070512], [forensics@hoffmannbv.nl])
AM_INIT_AUTOMAKE([libewf], [20070512])

dnl Function to test if the current directory contains spaces
AC_DEFUN([LIBEWF_TEST_PWD],
	[ac_libewf_pwd=`pwd`;
	 ac_libewf_test_pwd=`pwd | grep ' '`;
	 AS_IF([test "x$ac_libewf_test_pwd" != x],
		[AC_MSG_FAILURE([cannot handle space(s) in current directory: $ac_libewf_pwd])])
])

dnl Function to test if a certain feature was enabled
AC_DEFUN([LIBEWF_TEST_ENABLE],
	[AC_ARG_ENABLE([$1], [AS_HELP_STRING([--enable-$1], [$3 (default is $4)])], [ac_libewf_enable_$2=$enableval], [ac_libewf_enable_$2=$4])dnl
	 AC_CACHE_CHECK([whether to enable $3], [ac_libewf_enable_$2], [ac_libewf_enable_$2=$4])dnl
])

dnl Function to detect if ctime_r is defined according to the POSIX standard
AC_DEFUN([LIBEWF_FUNC_CTIME_R],
	[AC_CHECK_FUNC([ctime_r],
		[AC_DEFINE( [HAVE_CTIME_R], [1], [Define to 1 if you have the ctime_r function.] )
		AC_LANG_PUSH(C)
		AC_MSG_CHECKING([if ctime_r is defined according to the POSIX definion])
		AC_LINK_IFELSE(
			AC_LANG_PROGRAM( [[#include <time.h>]], [[ctime_r(NULL,NULL,0)]] ),
			[AC_MSG_RESULT([ctime_r with additional size argument detected])
			AC_DEFINE( [HAVE_CTIME_R_SIZE], [1], [Define to 1 if you have the ctime_r function with a third size argument.] )],
			[AC_LINK_IFELSE(
				AC_LANG_PROGRAM( [[#include <time.h>]], [[ctime_r(NULL,NULL)]] ),
				[AC_MSG_RESULT(yes)],
				[ CPPFLAGS="$CPPFLAGS -D_POSIX_PTHREAD_SEMANTICS"
				AC_LINK_IFELSE(
					AC_LANG_PROGRAM( [[#include <time.h>]], [[ctime_r(NULL,NULL)]] ),
					[AC_MSG_RESULT([ctime_r requires additional compile flags])],
					[AC_MSG_WARN([Unable to determine how to compile ctime_r])]
				)]
			)]
		)
		AC_LANG_POP(C)
	])
])

dnl Function to determine the operating system
AC_DEFUN([LIBEWF_DETERMINE_OPERATING_SYSTEM],
[ac_libewf_target_string="$target";
if test "x$ac_libewf_target_string" = x;
then
	ac_libewf_target_string="$build";
fi
case "$ac_libewf_target_string" in
	*cygwin* )
		ac_libewf_operating_system="Cygwin";
		;;
	*darwin* )
		ac_libewf_operating_system="Darwin";
		;;
	*freebsd* )
		ac_libewf_operating_system="FreeBSD";
		;;
	*netbsd* )
		ac_libewf_operating_system="NetBSD";
		;;
	*openbsd* )
		ac_libewf_operating_system="OpenBSD";
		;;
	*linux* )
		ac_libewf_operating_system="Linux";
		;;
	*solaris* )
		ac_libewf_operating_system="SunOS";
		;;
	* )
		ac_libewf_operating_system="Unknown";
		;;
esac
AC_DEFINE_UNQUOTED( LIBEWF_OPERATING_SYSTEM, "$ac_libewf_operating_system", [Defines the fallback operating system string.] )
])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check if current directory does not contain spaces
LIBEWF_TEST_PWD

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check for configure arguments
LIBEWF_TEST_ENABLE([wide-character-type], [wide_character_type], [enable wide character type support], [no])
LIBEWF_TEST_ENABLE([buffer-passthrough], [buffer_passthrough], [minimize the usage of the chunk cache by directly accessing data buffers], [no])
LIBEWF_TEST_ENABLE([chunk-cache-passthrough], [chunk_cache_passthrough], [use the chunk cache as data buffer in the ewf tools], [no])
LIBEWF_TEST_ENABLE([verbose-output], [verbose_output], [enable verbose output], [no])
LIBEWF_TEST_ENABLE([debug-output], [debug_output], [enable debug output], [no])

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Checks for libraries and library functions
dnl ac_libewf_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_CHECK_LIB( z, adler32, [], [ AC_MSG_FAILURE( [Missing function: adler32 in library: zlib.], 1 ) ] )
AC_CHECK_LIB( z, compress2, [ ac_libewf_dummy=yes ], [ AC_MSG_FAILURE( [Missing function: compress2 in library: zlib.], 1 ) ] )
AC_CHECK_LIB( z, compressBound, [AC_DEFINE( [HAVE_COMPRESS_BOUND], [1], [Defines if compressBound funtion is present in zlib.] ) ] )
AC_CHECK_LIB( z, uncompress, [ ac_libewf_dummy=yes ], [ AC_MSG_FAILURE( [Missing function: uncompress in library: zlib.], 1 ) ] )

AC_CHECK_LIB( crypto, MD5_Init, [], [ ac_libewf_have_libcrypto_md5=no ] )
AC_CHECK_LIB( crypto, MD5_Update, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_md5=no ] )
AC_CHECK_LIB( crypto, MD5_Final, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_md5=no ] )

AC_CHECK_LIB( crypto, SHA1_Init, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_sha1=no ] )
AC_CHECK_LIB( crypto, SHA1_Update, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_sha1=no ] )
AC_CHECK_LIB( crypto, SHA1_Final, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_sha1=no ] )

AC_CHECK_LIB( crypto, EVP_md5, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_evp=no ] )
AC_CHECK_LIB( crypto, EVP_sha1, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_evp=no ] )
AC_CHECK_LIB( crypto, EVP_MD_CTX_init, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_evp=no ] )
AC_CHECK_LIB( crypto, EVP_MD_CTX_cleanup, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_evp=no ] )
AC_CHECK_LIB( crypto, EVP_DigestInit_ex, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_evp=no ] )
AC_CHECK_LIB( crypto, EVP_DigestUpdate, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_evp=no ] )
AC_CHECK_LIB( crypto, EVP_DigestFinal_ex, [ ac_libewf_dummy=yes ], [ ac_libewf_have_libcrypto_evp=no ] )

AC_CHECK_LIB( uuid, uuid_generate )
AC_CHECK_LIB( uuid, uuid_generate_random, [AC_DEFINE( [HAVE_UUID_GENERATE_RANDOM], [1], [Defines if uuid_generate_random funtion is present in libuuid.] ) ] )
AC_CHECK_LIB( uuid, uuid_generate_time, [AC_DEFINE( [HAVE_UUID_GENERATE_TIME], [1], [Defines if uuid_generate_time funtion is present in libuuid.] ) ] )

dnl Check for headers
AC_CHECK_HEADERS([sys/types.h],
	[AC_DEFINE([HAVE_SYS_TYPES_H],[1],[Define to 1 if you have the <sys/types.h> header file.])
	AC_SUBST([HAVE_SYS_TYPES_H],[1])],
	[AC_SUBST([HAVE_SYS_TYPES_H],[0])])

AC_CHECK_HEADERS([inttypes.h],
	[AC_DEFINE([HAVE_INTTYPES_H],[1],[Define to 1 if you have the <inttypes.h> header file.])
	AC_SUBST([HAVE_INTTYPES_H],[1])],
	[AC_SUBST([HAVE_INTTYPES_H],[0])])

AC_CHECK_HEADERS([stdint.h],
	[AC_DEFINE([HAVE_STDINT_H],[1],[Define to 1 if you have the <stdint.h> header file.])
	AC_SUBST([HAVE_STDINT_H],[1])],
	[AC_SUBST([HAVE_STDINT_H],[0])])

AC_CHECK_HEADERS( cygwin/fs.h fcntl.h glob.h limits.h linux/fs.h openssl/opensslv.h signal.h stdarg.h stdlib.h string.h sys/disk.h sys/disklabel.h sys/ioctl.h sys/signal.h sys/utsname.h unistd.h uuid/uuid.h varargs.h zlib.h )

AC_HEADER_STDC

dnl Check for declarations and type definitions
AC_CHECK_DECLS( memrchr )

AC_STRUCT_TM

AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_CHECK_TYPE(ssize_t, signed)

dnl Check for functions
AC_CHECK_FUNCS( atol ctime fgets getopt memchr memcmp memcpy memrchr memset localtime localtime_r snprintf sprintf strchr strcmp strcpy strdup strerror strerror_r strlen strncmp strncpy strrchr strtol strtoul uname )

AC_FUNC_MEMCMP
AC_FUNC_STRERROR_R
AC_FUNC_VPRINTF

LIBEWF_FUNC_CTIME_R

dnl Check for enabling support
AS_IF([test "x$ac_libewf_have_libcrypto_md5" == xno],
	[AC_MSG_NOTICE([No support for calculating MD5 hashes, if required install libcrypto with MD5 support.])],
	[AC_MSG_NOTICE([Support for calculating MD5 hashes will be build.])
	AC_CHECK_HEADERS([openssl/md5.h])] )

AS_IF([test "x$ac_libewf_have_libcrypto_sha1" == xno],
	[AC_MSG_NOTICE([No support for calculating SHA1 hashes, if required install libcrypto with SHA1 support.])],
	[AC_MSG_NOTICE([Support for calculating SHA1 hashes will be build.])
	AC_CHECK_HEADERS([openssl/sha.h])] )

AS_IF([test "x$ac_libewf_have_libcrypto_evp" == xno],
	[AC_MSG_NOTICE([No support for calculating cryptographic hashes, if required install libcrypto support.])],
	[AC_MSG_NOTICE([Support for calculating libcrypto EVP will be build.])
	AC_CHECK_HEADERS([openssl/evp.h])] )

AS_IF([test "x$ac_libewf_enable_wide_character_type" != xno ],
	[AC_DEFINE([HAVE_WIDE_CHARACTER_TYPE],[1],[Defines if wide character type should be used])
		AC_CHECK_FUNCS([btowc fgetws mbsinit mbsrtowcs swprintf wmain wmemchr wmemcmp wmemcpy wmemrchr wctob wcschr wcscmp wcscpy wcsdup wcslen wcsncmp wcsncpy wcsrchr wcsrtombs wcstol wcstoul])
		AC_CHECK_HEADERS([wchar.h],
			[AC_DEFINE([HAVE_WCHAR_H],[1],[Define to 1 if you have the <wchar.h> header file.])
			AC_SUBST([HAVE_WCHAR_H],[1])],
			[AC_SUBST([HAVE_WCHAR_H],[0])])

		AC_SUBST([HAVE_WIDE_CHARACTER_TYPE],[1])],
	[AC_SUBST([HAVE_WIDE_CHARACTER_TYPE],[0])
	AC_SUBST([HAVE_WCHAR_H],[0])])

AS_IF( [test "x$ac_libewf_enable_buffer_passthrough" != xno ], [AC_DEFINE( [HAVE_BUFFER_PASSTHROUGH], [1], [Defines if buffer passthrough should be used] ) ] )
AS_IF( [test "x$ac_libewf_enable_chunk_cache_passthrough" != xno ], [AC_DEFINE( [HAVE_CHUNK_CACHE_PASSTHROUGH], [1], [Defines if chunk cache passthrough should be used] ) ] )
AS_IF( [test "x$ac_libewf_enable_verbose_output" != xno ], [AC_DEFINE( [HAVE_VERBOSE_OUTPUT], [1], [Defines if verbose output should be used] ) ] )
AS_IF( [test "x$ac_libewf_enable_debug_output" != xno ], [AC_DEFINE( [HAVE_DEBUG_OUTPUT], [1], [Defines if debug output should be used] ) ] )

dnl Check for the operation system will be used as a fall back in the ewftools
LIBEWF_DETERMINE_OPERATING_SYSTEM

CFLAGS="$CFLAGS -Wall";

AC_CONFIG_FILES([Makefile libewf/Makefile ewftools/Makefile manuals/Makefile libewf.pc libewf.spec include/libewf.h include/libewf/libewf_definitions.h include/libewf/libewf_types.h])
AC_CONFIG_HEADERS([libewf/libewf_config.h])

AC_OUTPUT

