dnl Functions for libsmdev
dnl
dnl Version: 20120630

dnl Function to detect if libsmdev is available
dnl ac_libsmdev_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBSMDEV_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libsmdev" != x && test "x$ac_cv_with_libsmdev" != xno && test "x$ac_cv_with_libsmdev" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libsmdev"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libsmdev}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libsmdev}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libsmdev])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libsmdev" = xno],
  [ac_cv_libsmdev=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [libsmdev],
    [libsmdev >= 20120630],
    [ac_cv_libsmdev=yes],
    [ac_cv_libsmdev=no])
   ])

  AS_IF(
   [test "x$ac_cv_libsmdev" = xyes],
   [ac_cv_libsmdev_CPPFLAGS="$pkg_cv_libsmdev_CFLAGS"
   ac_cv_libsmdev_LIBADD="$pkg_cv_libsmdev_LIBS"],
   [dnl Check for headers
   AC_CHECK_HEADERS([libsmdev.h])
  
   AS_IF(
    [test "x$ac_cv_header_libsmdev_h" = xno],
    [ac_cv_libsmdev=no],
    [dnl Check for the individual functions
    ac_cv_libsmdev=yes
    AC_CHECK_LIB(
     smdev,
     libsmdev_get_version,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
  
    AC_CHECK_LIB(
     smdev,
     libsmdev_check_device,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
  
    AS_IF(
     [test "x$ac_cv_enable_wide_character_type" != xno],
     [AC_CHECK_LIB(
      smdev,
      libsmdev_check_device_wide,
      [ac_cv_libsmdev_dummy=yes],
      [ac_cv_libsmdev=no])
     ])
  
    dnl Handle functions
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_initialize,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_free,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_signal_abort,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
  
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_open,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_close,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_read_buffer,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_seek_offset,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
  
    AS_IF(
     [test "x$ac_cv_enable_wide_character_type" != xno],
     [AC_CHECK_LIB(
      smdev,
      libsmdev_handle_open_wide,
      [ac_cv_libsmdev_dummy=yes],
      [ac_cv_libsmdev=no])
     ])
  
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_bus_type,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_bytes_per_sector,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_error,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_utf8_information_value,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_utf16_information_value,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_media_size,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_media_type,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_number_of_errors,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_number_of_sessions,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_session,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_number_of_tracks,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_get_track,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
  
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_set_error_flags,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_set_error_granularity,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])
    AC_CHECK_LIB(
     smdev,
     libsmdev_handle_set_number_of_error_retries,
     [ac_cv_libsmdev_dummy=yes],
     [ac_cv_libsmdev=no])

    ac_cv_libsmdev_LIBADD="-lsmdev"
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libsmdev" = xyes],
  [AC_DEFINE(
   [HAVE_LIBSMDEV],
   [1],
   [Define to 1 if you have the `smdev' library (-lsmdev).])
  ])

 AS_IF(
  [test "x$ac_cv_libsmdev" = xyes],
  [AC_SUBST(
   [HAVE_LIBSMDEV],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBSMDEV],
   [0])
  ])
 ])

dnl Function to detect if posix_fadvise is available
AC_DEFUN([AX_LIBSMDEV_CHECK_FUNC_POSIX_FADVISE],
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

dnl Check if winioctl.h defines STORAGE_BUS_TYPE
AC_DEFUN([AX_LIBSMDEV_CHECK_HEADER_WINIOCTL_H_STORAGE_BUS_TYPE],
 [AC_CACHE_CHECK(
  [whether winioctl.h defines STORAGE_BUS_TYPE],
  [ac_cv_header_winioctl_h_storage_bus_type],
  [AC_LANG_PUSH(C)
  AC_COMPILE_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <windows.h>
#include <winioctl.h>]],
    [[STORAGE_BUS_TYPE storage_bus_type;
storage_bus_type = BusTypeUnknown;]] )],
   [ac_cv_header_winioctl_h_storage_bus_type=yes],
   [ac_cv_header_winioctl_h_storage_bus_type=no])
  AC_LANG_POP(C)],
  [ac_cv_header_winioctl_h_storage_bus_type=no])

 AS_IF(
  [test "x$ac_cv_header_winioctl_h_storage_bus_type" = xyes],
  [AC_DEFINE(
   [HAVE_WINIOCTL_H_STORAGE_BUS_TYPE],
   [1],
   [Define to 1 if STORAGE_BUS_TYPE is defined.])
  ])
 ])

dnl Function to detect if libsmdev dependencies are available
AC_DEFUN([AX_LIBSMDEV_CHECK_LOCAL],
 [dnl Headers included in libsmdev/libsmdev_handle.c and libsmdev/libsmdev_support.c
 AC_CHECK_HEADERS([errno.h fcntl.h sys/stat.h unistd.h])

 dnl Headers included in libsmdev/libsmdev_metadata.c
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xno],
  [AC_CHECK_HEADERS([cygwin/fs.h linux/fs.h sys/disk.h sys/disklabel.h sys/ioctl.h])
 ])

 dnl Headers included in libsmdev/libsmdev_ata.c
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xno],
  [AC_CHECK_HEADERS([cygwin/hdreg.h linux/hdreg.h])
 ])

 dnl Headers included in libsmdev/libsmdev_scsi.c
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xno],
  [AC_CHECK_HEADERS([scsi/scsi.h scsi/scsi_ioctl.h scsi/sg.h])
 ])

 dnl Headers included in libsmdev/libsmdev_optical_disk.c
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xno],
  [AC_CHECK_HEADERS([linux/cdrom.h])
 ])

 dnl Headers included in libsmdev/libsmdev_usb.c
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xno],
  [AC_CHECK_HEADERS([linux/usbdevice_fs.h linux/usb/ch9.h])
 ])

 dnl File input/output functions used in libbfio/libbfio_file.h
 AC_CHECK_FUNCS([close fstat ftruncate lseek open read stat write])

 AS_IF(
  [test "x$ac_cv_func_close" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: close],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_fstat" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: fstat],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_ftruncate" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: ftruncate],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_lseek" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: lseek],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_open" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: open],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_read" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: read],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_stat" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: stat],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_write" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: write],
   [1])
  ])
 
 AX_LIBSMDEV_CHECK_FUNC_POSIX_FADVISE

 dnl Check for error string functions used in libsmdev/libsmdev_error_string.c
 AC_FUNC_STRERROR_R()

 AS_IF(
  [test "x$ac_cv_have_decl_strerror_r" = xno],
  [AC_CHECK_FUNCS([strerror])

  AS_IF(
   [test "x$ac_cv_func_strerror" != xyes],
   [AC_MSG_FAILURE(
    [Missing functions: strerror and strerror_r],
    [1])
   ])
  ])

 dnl Check if winioctl.h defines STORAGE_BUS_TYPE
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xyes],
  [AX_LIBSMDEV_CHECK_HEADER_WINIOCTL_H_STORAGE_BUS_TYPE])

 ac_cv_libsmdev_CPPFLAGS="-I../libsmdev";
 ac_cv_libsmdev_LIBADD="../libsmdev/libsmdev.la";

 ac_cv_libsmdev=local
 ])

dnl Function to detect how to enable libsmdev
AC_DEFUN([AX_LIBSMDEV_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libmsdev],
  [libmsdev],
  [search for libmsdev in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBSMDEV_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libsmdev" != xyes],
  [AX_LIBSMDEV_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBSMDEV],
   [1],
   [Define to 1 if the local version of libsmdev is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBSMDEV],
   [1])
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBSMDEV],
  [test "x$ac_cv_libsmdev" = xlocal])
 AS_IF(
  [test "x$ac_cv_libsmdev_CPPFLAGS" != "x"],
  [AC_SUBST(
   [LIBSMDEV_CPPFLAGS],
   [$ac_cv_libsmdev_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_libsmdev_LIBADD" != "x"],
  [AC_SUBST(
   [LIBSMDEV_LIBADD],
   [$ac_cv_libsmdev_LIBADD])
  ])


 AS_IF(
  [test "x$ac_cv_libsmdev" = xyes],
  [AC_SUBST(
   [ax_libsmdev_pc_libs_private],
   [-lsmdev])
  ])

 AS_IF(
  [test "x$ac_cv_libsmdev" = xyes],
  [AC_SUBST(
   [ax_libsmdev_spec_requires],
   [libsmdev])
  AC_SUBST(
   [ax_libsmdev_spec_build_requires],
   [libsmdev-devel])
  ])
 ])

