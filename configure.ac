AC_PREREQ( 2.59 )

AC_INIT(
 [libewf],
 [20090606],
 [forensics@hoffmannbv.nl])

AC_CONFIG_SRCDIR(
 [include/libewf.h.in])

AM_INIT_AUTOMAKE(
 [libewf],
 [20090606])

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
 [static-executables],
 [static_executables],
 [build the ewftools as static executables],
 [no])
LIBEWF_TEST_ENABLE(
 [low-level-functions],
 [low_level_functions],
 [use libewf's low level read and write functions in the ewftools],
 [no])
LIBEWF_TEST_ENABLE(
 [low-memory-usage],
 [low_memory_usage],
 [enable low memory usage],
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
LIBEWF_TEST_ENABLE(
 [v2-api],
 [v2_api],
 [enable experimental version 2 API],
 [no])

dnl Check for libtool DLL support
AC_LIBTOOL_WIN32_DLL

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

dnl Check for headers
dnl Headers included in include/libewf/types.h
AC_CHECK_HEADER(
 [sys/types.h],
 [AC_DEFINE(
  [HAVE_SYS_TYPES_H],
  [1],
  [Define to 1 if you have the <sys/types.h> header file.])
 AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [0])])

AC_CHECK_HEADER(
 [inttypes.h],
 [AC_DEFINE(
  [HAVE_INTTYPES_H],
  [1],
  [Define to 1 if you have the <inttypes.h> header file.])
 AC_SUBST(
  [HAVE_INTTYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_INTTYPES_H],
  [0])])

AC_CHECK_HEADER(
 [stdint.h],
 [AC_DEFINE(
  [HAVE_STDINT_H],
  [1],
  [Define to 1 if you have the <stdint.h> header file.])
 AC_SUBST(
  [HAVE_STDINT_H],
  [1])],
 [AC_SUBST(
  [HAVE_STDINT_H],
  [0])])

dnl Headers included in common/types.h
AC_CHECK_HEADERS([limits.h])

dnl Headers included in common/narrow_string.h
AC_CHECK_HEADERS([string.h])

dnl Headers included in common/memory.h
AC_CHECK_HEADERS([stdlib.h])

dnl Headers included in libbfio/libbfio_file.h and ewftools/file_io.h
AC_CHECK_HEADERS([sys/stat.h])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([unistd.h])

dnl Headers included in libewf/libewf_date_time.h
AC_HEADER_TIME

dnl Headers included in libewf/libewf_notify.h
AC_CHECK_HEADERS([stdarg.h])
AC_CHECK_HEADERS([varargs.h])

dnl Headers included in ewftools/glob.h
AC_CHECK_HEADERS([glob.h])

dnl Headers included in ewftools/ewfsignal.h
AC_CHECK_HEADERS([signal.h])
AC_CHECK_HEADERS([sys/signal.h])

dnl Headers included in ewftools/system_string.h
AC_CHECK_HEADERS([langinfo.h])
AC_CHECK_HEADERS([locale.h])

dnl Headers included in ewftools/device_handle.c
dnl and ewftools/io_*.[ch]
AC_CHECK_HEADERS([cygwin/fs.h])
AC_CHECK_HEADERS([cygwin/hdreg.h])

AC_CHECK_HEADERS([linux/cdrom.h])
AC_CHECK_HEADERS([linux/fs.h])
AC_CHECK_HEADERS([linux/hdreg.h])
AC_CHECK_HEADERS([linux/usbdevice_fs.h])
AC_CHECK_HEADERS([linux/usb/ch9.h])

AC_CHECK_HEADERS([scsi/scsi.h])
AC_CHECK_HEADERS([scsi/scsi_ioctl.h])
AC_CHECK_HEADERS([scsi/sg.h])

AC_CHECK_HEADERS([sys/disk.h])
AC_CHECK_HEADERS([sys/disklabel.h])
AC_CHECK_HEADERS([sys/ioctl.h])

dnl Other headers
AC_CHECK_HEADERS([errno.h])

AC_CHECK_HEADERS([sys/utsname.h])

AC_CHECK_HEADERS([openssl/opensslv.h])
AC_CHECK_HEADERS([zlib.h])

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

AC_CHECK_FUNCS(
 [realloc],
 [],
 [AC_MSG_FAILURE(
  [Missing function: realloc],
  [1]) ])

dnl Narrow character string functions used in common/narrow_string.h
AC_CHECK_FUNCS(
 [snprintf],
 [],
 [AC_CHECK_FUNCS(
  [sprintf],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: snprintf and sprintf],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strlen],
 [],
 [AC_MSG_FAILURE(
  [Missing function: strlen],
  [1]) ])

AC_CHECK_FUNCS(
 [strchr],
 [],
 [AC_CHECK_FUNCS(
  [memchr],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strchr and memchr],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strncmp],
 [],
 [AC_CHECK_FUNCS(
  [strcmp],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strncmp and strcmp],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strncpy],
 [],
 [AC_CHECK_FUNCS(
  [strcpy],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strncpy and strcpy],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strrchr],
 [],
 [AC_CHECK_FUNCS(
  [memrchr],
  [AC_CHECK_DECLS(
   [memrchr],
   [],
   [AC_MSG_FAILURE(
    [Missing declaration for function memrchr],
    [1]) ]) ],
  [AC_MSG_FAILURE(
   [Missing functions: strrchr and memrchr],
   [1]) ]) ])

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

dnl File input/output functions used in libbdio/libbfio_file.h
AC_CHECK_FUNCS(
 [close],
 [],
 [AC_MSG_FAILURE(
  [Missing function: close],
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
 [write],
 [],
 [AC_MSG_FAILURE(
  [Missing function: write],
  [1]) ])

AC_CHECK_FUNCS(
 [fstat],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fstat],
  [1]) ])

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

dnl File input/output functions used in libewf/libewf_notify.h
AC_CHECK_FUNCS(
 [vfprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: vfprintf],
  [1]) ])

dnl Date and time functions used in ewftools/date_time.h
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

dnl Check for error string functions used in ewftools/error_string.h
AC_FUNC_STRERROR_R(
 [],
 [AC_CHECK_FUNCS(
  [strerror],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strerror_r and strerror],
   [1]) ]) ])

dnl Check for error string functions used in ewftools/ewfgetopt.h
AC_CHECK_FUNCS([getopt])

dnl File input/output functions used in ewftools/file_stream_io.h
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
 [fwrite],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fwrite],
  [1]) ])

dnl Check for locale functions
AC_CHECK_FUNCS(
 [setlocale],
 [],
 [AC_MSG_FAILURE(
  [Missing function: setlocale],
  [1]) ])

LIBEWF_LANGINFO_CODESET

dnl System functions
AC_CHECK_FUNCS(
 [fstat],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fstat],
  [1]) ])

AC_CHECK_FUNCS(
 [setlocale],
 [],
 [AC_MSG_FAILURE(
  [Missing function: setlocale],
  [1]) ])

AC_CHECK_FUNCS([uname])

dnl Check for printf conversion specifier support
LIBEWF_CHECK_PRINTF_JD
LIBEWF_CHECK_PRINTF_ZD

dnl Checks for libraries and library functions
dnl ac_libewf_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_CHECK_LIB(
 z,
 adler32,
 [],
 [AC_MSG_FAILURE(
  [Missing function: adler32 in library: zlib.],
  [1])])

AC_CHECK_LIB(
 z,
 compress2,
 [ac_libewf_dummy=yes],
 [AC_MSG_FAILURE(
  [Missing function: compress2 in library: zlib.],
  [1])])

AC_CHECK_LIB(
 z,
 compressBound,
 [AC_DEFINE(
  [HAVE_COMPRESS_BOUND],
  [1],
  [Defines if compressBound funtion is present in zlib.])])

AC_CHECK_LIB(
 z,
 uncompress,
 [ac_libewf_dummy=yes],
 [AC_MSG_FAILURE(
  [Missing function: uncompress in library: zlib.],
  [1])])

dnl Check for enabling libuna for Unicode string support
AC_CHECK_HEADERS(
 [libuna.h],
 [],
 [ac_libewf_have_libuna=no])

AS_IF(
	[test "x$ac_libewf_have_libuna" != xno],
	[ac_libewf_have_libuna=yes
	AC_CHECK_LIB(
	 una,
	 libuna_get_version,
	 [],
	 [ac_libewf_have_libuna=no])

	dnl Byte stream functions
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
	 libuna_utf16_stream_size_from_utf32,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_copy_from_utf32,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libuna=no])

	dnl UTF-8 string functions
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
	 libuna_utf32_string_size_from_utf8,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_copy_from_utf8,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libuna=no])

	dnl compare string functions
	AC_CHECK_LIB(
	 una,
	 libuna_compare_utf8_with_utf16,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_compare_utf8_with_utf32,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libuna=no])
	])

AS_IF(
	[test "x$ac_libewf_have_libuna" = xyes],
	[AC_SUBST(
	 [libewf_pc_libs_private_libuna],
	 [-luna])
	AC_SUBST(
	 [libewf_spec_requires_libuna],
	 [libuna])
	AC_SUBST(
	 [libewf_spec_build_requires_libuna],
	 [libuna-devel]) ],
	[dnl If libuna is not available use the (local) version that came with libewf
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
AC_CHECK_HEADERS(
 [libbfio.h],
 [],
 [ac_libewf_have_libbfio=no])

AS_IF(
	[test "x$ac_libewf_have_libbfio" != xno],
	[ac_libewf_have_libbfio=yes
	AC_CHECK_LIB(
	 bfio,
	 libbfio_get_version,
	 [],
	 [ac_libewf_have_libbfio=no])

	dnl Basic file IO functions
	AC_CHECK_LIB(
	 bfio,
	 libbfio_initialize,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libbfio=no])
	AC_CHECK_LIB(
	 bfio,
	 libbfio_free,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libbfio=no])
	AC_CHECK_LIB(
	 bfio,
	 libbfio_open,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libbfio=no])
	AC_CHECK_LIB(
	 bfio,
	 libbfio_close,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libbfio=no])
	AC_CHECK_LIB(
	 bfio,
	 libbfio_seek_offset,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libbfio=no])
	AC_CHECK_LIB(
	 bfio,
	 libbfio_read,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libbfio=no])
	])

AS_IF(
	[test "x$ac_libewf_have_libbfio" = xyes],
	[AC_SUBST(
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
	 []) ],
	[dnl If libbfio is not available use the (local) version that came with libewf
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
	ac_libewf_have_libbfio=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBBFIO],
 [test "x$ac_libewf_have_libbfio" = xlocal])

dnl Check for enabling libcrypto EVP support
AC_CHECK_HEADERS(
 [openssl/evp.h],
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
	 ac_libewf_have_libcrypto_evp=yes],
	[AC_MSG_RESULT(
	 [no])
	ac_libewf_have_libcrypto_evp=no])
 AC_LANG_POP(C)],
 [ac_libewf_have_libcrypto_evp=no])

AS_IF(
	[test "x$ac_libewf_have_libcrypto_evp" != xno],
	[ac_libewf_have_libcrypto_evp=yes
	AC_CHECK_LIB(
	 crypto,
	 EVP_md5,
	 [],
	 [ac_libewf_have_libcrypto_evp=no])
	AC_CHECK_LIB(
	 crypto,
	 EVP_sha1,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libcrypto_evp=no])
	AC_CHECK_LIB(
	 crypto,
	 EVP_MD_CTX_init,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libcrypto_evp=no])
	AC_CHECK_LIB(
	 crypto,
	 EVP_MD_CTX_cleanup,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libcrypto_evp=no])
	AC_CHECK_LIB(
	 crypto,
	 EVP_DigestInit_ex,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libcrypto_evp=no])
	AC_CHECK_LIB(
	 crypto,
	 EVP_DigestUpdate,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libcrypto_evp=no])
	AC_CHECK_LIB(
	 crypto,
	 EVP_DigestFinal_ex,
	 [ac_libewf_dummy=yes],
	 [ac_libewf_have_libcrypto_evp=no])
	])

AS_IF(
	[test "x$ac_libewf_have_libcrypto_evp" = xno],
	[AC_SUBST(
	 [HAVE_OPENSSL_EVP_H],
	 [0])] )

AS_IF(
	[test "x$ac_libewf_have_libcrypto_evp" = xyes],
	[ac_libewf_have_libcrypto_md5=evp
	ac_libewf_have_libcrypto_sha1=evp])

dnl Check for libcrypto MD5 support
AS_IF(
	[test "x$ac_libewf_have_libcrypto_md5" != xevp ],
	[AC_CHECK_HEADERS(
	 [openssl/md5.h],
	 [],
	 [ac_libewf_have_libcrypto_md5=no]) ])

AS_IF(
	[test "x$ac_libewf_have_libcrypto_md5" != xno && test "x$ac_libewf_have_libcrypto_md5" != xevp ],
	[ac_libewf_have_libcrypto_md5=yes
	AC_CHECK_LIB(
	 crypto,
	 MD5_Init,
	 [],
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

AS_IF(
	[test "x$ac_libewf_have_libcrypto_md5" = xno],
	[AC_MSG_NOTICE(
	 [No support for calculating MD5 hashes, if required install (openssl) libcrypto with MD5 support.])
	])

dnl Check for libcrypto SHA1 support
AS_IF(
	[test "x$ac_libewf_have_libcrypto_sha1" != xevp ],
	[AC_CHECK_HEADERS(
	 [openssl/sha.h],
	 [],
	 [ac_libewf_have_libcrypto_sha1=no]) ])

AS_IF(
	[test "x$ac_libewf_have_libcrypto_sha1" != xno && test "x$ac_libewf_have_libcrypto_sha1" != xevp ],
	[ac_libewf_have_libcrypto_sha1=yes
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

AS_IF(
	[test "x$ac_libewf_have_libcrypto_sha1" = xno],
	[AC_MSG_NOTICE(
	 [No support for calculating SHA1 hashes, if required install (openssl) libcrypto with SHA1 support.])
	])

dnl Check for enabling guid support
AC_CHECK_HEADERS(
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
	  [],
	  [ac_libewf_have_guid_support=no])
	 ])
	])

AS_IF(
	[test "x$ac_libewf_have_guid_support" != xno],
	[AC_DEFINE(
	 [HAVE_GUID_SUPPORT],
	 [1],
	 [Define to 1 if GUID support is available.])
	])

AS_IF(
	[test "x$ac_libewf_have_guid_support" = xlibuuid],
	[AC_CHECK_LIB(
	 [uuid],
	 [uuid_generate_random],
	 [AC_DEFINE(
	  [HAVE_UUID_GENERATE_RANDOM],
	  [1],
	  [Defines if uuid_generate_random funtion is present in libuuid.])
	 ])
	AC_CHECK_LIB(
	 [uuid],
	 [uuid_generate_time],
	 [AC_DEFINE(
	  [HAVE_UUID_GENERATE_TIME],
	  [1],
	  [Defines if uuid_generate_time funtion is present in libuuid.])
	 ])
	])

AS_IF(
	[test "x$ac_libewf_have_guid_support" = xnative],
	[AC_CHECK_FUNC(
	 [uuid_generate_random],
	 [AC_DEFINE(
	  [HAVE_UUID_GENERATE_RANDOM],
	  [1],
	  [Defines if uuid_generate_random funtion is present.])
	 ])
	AC_CHECK_FUNC(
	 [uuid_generate_time],
	 [AC_DEFINE(
	  [HAVE_UUID_GENERATE_TIME],
	  [1],
	  [Defines if uuid_generate_time funtion is present.])
	 ])
	])

dnl Check if libewf should be build with wide character type support
AS_IF(
	[test "x$ac_cv_libewf_enable_wide_character_type" != xno ],
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

	dnl Wide character string functions used in common/wide_string.h
	AC_CHECK_FUNCS(
	 [swprintf],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing function: swprintf],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [wcschr],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemchr],
	  [],
	  [AC_MSG_FAILURE(
	   [Missing functions: wcschr and wmemchr],
	   [1]) ]) ])

	AC_CHECK_FUNCS(
	 [wcsncmp],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemcmp],
	  [],
	  [AC_CHECK_FUNCS(
	   [wcscmp],
	   [],
	   [AC_MSG_FAILURE(
	    [Missing functions: wcsncmp, wmemcmp and wcscmp],
	    [1]) ]) ]) ])

	AC_CHECK_FUNCS(
	 [wcsncpy],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemcpy],
	  [],
	  [AC_CHECK_FUNCS(
	   [wcscpy],
	   [],
	   [AC_MSG_FAILURE(
	    [Missing functions: wcsncpy, wmemcpy and wcscpy],
	    [1]) ]) ]) ])

	AC_CHECK_FUNCS(
	 [wcslen],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing function: wcslen],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [wcsrchr],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemrchr],
	  [],
	  [AC_MSG_FAILURE(
	   [Missing functions: wcsrchr and wmemrchr],
	   [1]) ]) ])

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

	dnl Wide character string functions used in ewftools/file_stream_io.h
	AC_CHECK_FUNCS(
	 [fgetws],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing function: fgetws],
	  [1]) ])

	dnl Make the necessary definitions
	AC_DEFINE(
	 [HAVE_WIDE_CHARACTER_TYPE],
	 [1],
	 [Defines if wide character type should be used])
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
	ac_cv_libewf_enable_static_executables=yes])

dnl Check if ewftools should be build with low level read and write functions
AS_IF(
	[test "x$ac_cv_libewf_enable_low_level_functions" != xno],
	[AC_DEFINE(
	 [HAVE_LOW_LEVEL_FUNCTIONS],
	 [1],
	 [Defines if libewf's low level read and write functions should be used in the ewf tools] )
	ac_cv_libewf_enable_low_level_functions=yes])

dnl Check if libewf should be build with verbose output
AS_IF(
	[test "x$ac_cv_libewf_enable_verbose_output" != xno],
	[AC_DEFINE(
	 [HAVE_VERBOSE_OUTPUT],
	 [1],
	 [Defines if verbose output should be used] )
	ac_cv_libewf_enable_verbose_output=yes])

dnl Check if libewf should be build with debug output
AS_IF(
	[test "x$ac_cv_libewf_enable_debug_output" != xno],
	[AC_DEFINE(
	 [HAVE_DEBUG_OUTPUT],
	 [1],
	 [Defines if debug output should be used] )
	ac_cv_libewf_enable_debug_output=yes])

dnl Check if libewf python bindings (pyewf) should be built
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
	 [Defines if experimental version 1 API should be used] )
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

dnl Check if libewf should be build with experimental version 2 API
AS_IF(
	[test "x$ac_cv_libewf_enable_v2_api" != xno],
	[AC_DEFINE(
	 [HAVE_V2_API],
	 [1],
	 [Defines if experimental version 2 API should be used] )
	AC_SUBST(
	 [HAVE_V2_API],
	 [1])
	ac_cv_libewf_enable_v2_api=yes],
	[AC_SUBST(
	 [HAVE_V2_API],
	 [0]) ])

AM_CONDITIONAL(
 HAVE_V2_API,
 [test "x$ac_cv_libewf_enable_v2_api" != xno])

dnl Check for the operation system will be used as a fall back in the ewftools
LIBEWF_DETERMINE_OPERATING_SYSTEM

dnl Set memwatch support
ac_libewf_have_memwatch=no

AS_IF(
	[test "x$ac_libewf_have_memwatch" != xno ],
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
AC_CONFIG_FILES([liberror/Makefile])
AC_CONFIG_FILES([libuna/Makefile])
AC_CONFIG_FILES([libbfio/Makefile])
AC_CONFIG_FILES([libewf/Makefile])
AC_CONFIG_FILES([ewftools/Makefile])
AC_CONFIG_FILES([pyewf/Makefile])
AC_CONFIG_FILES([documents/Makefile])
AC_CONFIG_FILES([manuals/Makefile])
AC_CONFIG_FILES([macosx/Makefile])
AC_CONFIG_FILES([msvscpp/Makefile])
dnl Have configure make the include files
AC_CONFIG_FILES([include/libewf.h])
AC_CONFIG_FILES([include/libewf/definitions.h])
AC_CONFIG_FILES([include/libewf/features.h])
AC_CONFIG_FILES([include/libewf/types.h])
AC_CONFIG_FILES([libewf/libewf_definitions.h])
dnl Have configure make distribution specific files
AC_CONFIG_FILES([common/types.h])
AC_CONFIG_FILES([debian/changelog])
AC_CONFIG_FILES([debian/shlibs.local.ex])
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
   libcrypto EVP support:                           $ac_libewf_have_libcrypto_evp
   libcrypto MD5 support:                           $ac_libewf_have_libcrypto_md5
   libcrypto SHA1 support:                          $ac_libewf_have_libcrypto_sha1
   guid support:                                    $ac_libewf_have_guid_support

Features:
   Wide character type support:                     $ac_cv_libewf_enable_wide_character_type
   ewftools are build as static executables:        $ac_cv_libewf_enable_static_executables
   ewftools use low level read and write functions: $ac_cv_libewf_enable_low_level_functions
   Python (pyewf) support:                          $ac_cv_libewf_enable_python
   Verbose output:                                  $ac_cv_libewf_enable_verbose_output
   Debug output:                                    $ac_cv_libewf_enable_debug_output
   Version 1 API:                                   $ac_cv_libewf_enable_v1_api
   Experimental version 2 API:                      $ac_cv_libewf_enable_v2_api
]);

