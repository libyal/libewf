dnl Functions for libsmdev

dnl Function to detect if libsmdev is available
AC_DEFUN([AC_CHECK_LIBSMDEV],
 [dnl Check if parameters were provided
 AS_IF(
  [test x"$ac_cv_with_libsmdev" != x && test "x$ac_cv_with_libsmdev" != xno && test "x$ac_cv_with_libsmdev" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libsmdev"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libsmdev}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libsmdev}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libsmdev])
   ])
  ])

 AS_IF(
  [test x"$ac_cv_with_libsmdev" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libsmdev.h])
 
  AS_IF(
   [test "x$ac_cv_header_libsmdev_h" = xno],
   [ac_cv_libsmdev=no],
   [ac_cv_libsmdev=yes
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
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libsmdev" = xyes],
  [AC_DEFINE(
   [HAVE_LIBSMDEV],
   [1],
   [Define to 1 if you have the `smdev' library (-lsmdev).])
  LIBS="-lsmdev $LIBS"
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
AC_DEFUN([AC_CHECK_FUNC_POSIX_FADVISE],
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
AC_DEFUN([AC_CHECK_HEADER_WINIOCTL_H_STORAGE_BUS_TYPE],
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
AC_DEFUN([AC_CHECK_LOCAL_LIBSMDEV],
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
 AC_CHECK_FUNCS(
  [close],
  [],
  [AC_MSG_FAILURE(
   [Missing function: close],
   [1])
  ])
 
 AC_CHECK_FUNCS(
  [fstat],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fstat],
   [1])
  ])
 
 AC_CHECK_FUNCS(
  [ftruncate],
  [],
  [AC_MSG_FAILURE(
   [Missing function: ftruncate],
   [1])
  ])
 
 AC_CHECK_FUNCS(
  [lseek],
  [],
  [AC_MSG_FAILURE(
   [Missing function: lseek],
   [1])
  ])
 
 AC_CHECK_FUNCS(
  [open],
  [],
  [AC_MSG_FAILURE(
   [Missing function: open],
   [1])
  ])
 
 AC_CHECK_FUNCS(
  [read],
  [],
  [AC_MSG_FAILURE(
   [Missing function: read],
   [1])
  ])
 
 AC_CHECK_FUNCS(
  [stat],
  [],
  [AC_MSG_FAILURE(
   [Missing function: stat],
   [1])
  ])
 
 AC_CHECK_FUNCS(
  [write],
  [],
  [AC_MSG_FAILURE(
   [Missing function: write],
   [1])
  ])

 AC_CHECK_FUNC_POSIX_FADVISE

 dnl Check for error string functions used in libsmdev/libsmdev_error_string.c
 AC_FUNC_STRERROR_R()

 AS_IF(
  [test "x$ac_cv_have_decl_strerror_r" = xno],
  [AC_CHECK_FUNCS(
   [strerror],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: strerror_r and strerror],
    [1])
   ])
  ])

 dnl Check if winioctl.h defines STORAGE_BUS_TYPE
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xyes],
  [AC_CHECK_HEADER_WINIOCTL_H_STORAGE_BUS_TYPE])

 ])

