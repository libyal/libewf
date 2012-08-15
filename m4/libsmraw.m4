dnl Functions for libsmraw
dnl
dnl Version: 20120630

dnl Function to detect if libsmraw is available
dnl ac_libsmraw_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBSMRAW_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libsmraw" != x && test "x$ac_cv_with_libsmraw" != xno && test "x$ac_cv_with_libsmraw" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libsmraw"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libsmraw}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libsmraw}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libsmraw])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libsmraw" = xno],
  [ac_cv_libsmraw=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [libsmraw],
    [libsmraw >= 20120630],
    [ac_cv_libsmraw=yes],
    [ac_cv_libsmraw=no])
   ])

  AS_IF(
   [test "x$ac_cv_libsmraw" = xyes],
   [ac_cv_libsmraw_CPPFLAGS="$pkg_cv_libsmraw_CFLAGS"
   ac_cv_libsmraw_LIBADD="$pkg_cv_libsmraw_LIBS"],
   [dnl Check for headers
   AC_CHECK_HEADERS([libsmraw.h])
  
   AS_IF(
    [test "x$ac_cv_header_libsmraw_h" = xno],
    [ac_cv_libsmraw=no],
    [dnl Check for the individual functions
    ac_cv_libsmraw=yes
    AC_CHECK_LIB(
     smraw,
     libsmraw_get_version,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
  
    dnl Handle functions
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_initialize,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_free,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_signal_abort,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_open,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_close,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_read_buffer,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_seek_offset,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_write_buffer,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
  
    AS_IF(
     [test "x$ac_cv_enable_wide_character_type" != xno],
     [AC_CHECK_LIB(
      smraw,
      libsmraw_handle_open_wide,
      [ac_cv_libsmraw_dummy=yes],
      [ac_cv_libsmraw=no])
     ])
  
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_get_media_size,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_get_bytes_per_sector,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_get_utf8_information_value
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_get_utf16_information_value
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
  
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_set_maximum_segment_size,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_set_media_size,
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_set_utf8_integrity_hash_value
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])
    AC_CHECK_LIB(
     smraw,
     libsmraw_handle_set_utf16_integrity_hash_value
     [ac_cv_libsmraw_dummy=yes],
     [ac_cv_libsmraw=no])

    ac_cv_libsmraw_LIBADD="-lsmraw"
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libsmraw" = xyes],
  [AC_DEFINE(
   [HAVE_LIBSMRAW],
   [1],
   [Define to 1 if you have the `smraw' library (-lsmraw).])
  ])

 AS_IF(
  [test "x$ac_cv_libsmraw" = xyes],
  [AC_SUBST(
   [HAVE_LIBSMRAW],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBSMRAW],
   [0])
  ])
 ])

dnl Function to detect how to enable libsmraw
AC_DEFUN([AX_LIBSMRAW_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libmsraw],
  [libmsraw],
  [search for libmsraw in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBSMRAW_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libsmraw" != xyes],
  [AC_DEFINE(
   [HAVE_LOCAL_LIBSMRAW],
   [1],
   [Define to 1 if the local version of libsmraw is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBSMRAW],
   [1])

  ac_cv_libsmraw_CPPFLAGS="-I../libsmraw";
  ac_cv_libsmraw_LIBADD="../libsmraw/libsmraw.la";

  ac_cv_libsmraw=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBSMRAW],
  [test "x$ac_cv_libsmraw" = xlocal])
 AS_IF(
  [test "x$ac_cv_libsmraw_CPPFLAGS" != "x"],
  [AC_SUBST(
   [LIBSMRAW_CPPFLAGS],
   [$ac_cv_libsmraw_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_libsmraw_LIBADD" != "x"],
  [AC_SUBST(
   [LIBSMRAW_LIBADD],
   [$ac_cv_libsmraw_LIBADD])
  ])

 AS_IF(
  [test "x$ac_cv_libsmraw" = xyes],
  [AC_SUBST(
   [ax_libsmraw_pc_libs_private],
   [-lsmraw])
  ])

 AS_IF(
  [test "x$ac_cv_libsmraw" = xyes],
  [AC_SUBST(
   [ax_libsmraw_spec_requires],
   [libsmraw])
  AC_SUBST(
   [ax_libsmraw_spec_build_requires],
   [libsmraw-devel])
  ])
 ])

