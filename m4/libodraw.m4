dnl Functions for libodraw

dnl Function to detect if libodraw is available
AC_DEFUN([AC_CHECK_LIBODRAW],
 [dnl Check if parameters were provided
 AS_IF(
  [test x"$ac_cv_with_libodraw" != x && test "x$ac_cv_with_libodraw" != xno && test "x$ac_cv_with_libodraw" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libodraw"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libodraw}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libodraw}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libodraw])
   ])
  ])

 AS_IF(
  [test x"$ac_cv_with_libodraw" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libodraw.h])
 
  AS_IF(
   [test "x$ac_cv_header_libodraw_h" = xno],
   [ac_cv_libodraw=no],
   [ac_cv_libodraw=yes
   AC_CHECK_LIB(
    odraw,
    libodraw_get_version,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
 
   dnl Handle functions
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_initialize,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_free,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_signal_abort,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_open,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_open_data_files,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_close,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_read_buffer,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_seek_offset,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
 
   AS_IF(
    [test "x$ac_cv_enable_wide_character_type" != xno],
    [AC_CHECK_LIB(
     odraw,
     libodraw_handle_open_wide,
     [ac_cv_libodraw_dummy=yes],
     [ac_cv_libodraw=no])
    ])
 
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_get_bytes_per_sector,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_get_data_file,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_get_media_size,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_get_number_of_data_files,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_get_number_of_sessions,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_get_session,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_get_number_of_tracks,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_handle_get_track,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
 
   dnl Data file functions
   AC_CHECK_LIB(
    odraw,
    libodraw_data_file_free,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
   AC_CHECK_LIB(
    odraw,
    libodraw_data_file_set_filename,
    [ac_cv_libodraw_dummy=yes],
    [ac_cv_libodraw=no])
 
   AS_IF(
    [test "x$ac_cv_enable_wide_character_type" != xno],
    [AC_CHECK_LIB(
     odraw,
     libodraw_data_file_set_filename_wide,
     [ac_cv_libodraw_dummy=yes],
     [ac_cv_libodraw=no])
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libodraw" = xyes],
  [AC_DEFINE(
   [HAVE_LIBODRAW],
   [1],
   [Define to 1 if you have the `odraw' library (-lodraw).])
  LIBS="-lodraw $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libodraw" = xyes],
  [AC_SUBST(
   [HAVE_LIBODRAW],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBODRAW],
   [0])
  ])
 ])

dnl Function to detect if libodraw dependencies are available
AC_DEFUN([AC_CHECK_LOCAL_LIBODRAW],
 [AM_PROG_LEX
 AC_PROG_YACC])

