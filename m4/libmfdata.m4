dnl Functions for libmfdata
dnl
dnl Version: 20111007

dnl Function to detect if libmfdata available
dnl ac_libmfdata_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBMFDATA_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libmfdata" != x && test "x$ac_cv_with_libmfdata" != xno && test "x$ac_cv_with_libmfdata" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libmfdata"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libmfdata}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libmfdata}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libmfdata])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libmfdata" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libmfdata.h])
 
  AS_IF(
   [test "x$ac_cv_header_libmfdata_h" = xno],
   [ac_cv_libmfdata=no],
   [ac_cv_libmfdata=yes
   AC_CHECK_LIB(
    mfdata,
    libmfdata_get_version,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
  
   dnl Segment table functions
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_initialize,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
 
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_free,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_empty,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_resize,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_read_buffer,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_seek_offset,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_write_buffer,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
 
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_get_file_io_handle,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_get_maximum_segment_size,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_get_number_of_segments,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_get_value_offset,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_get_value_size,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_set_maximum_segment_size,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   AC_CHECK_LIB(
    mfdata,
    libmfdata_segment_table_set_segment_by_index,
    [ac_cv_libmfdata_dummy=yes],
    [ac_cv_libmfdata=no])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libmfdata" = xyes],
  [AC_DEFINE(
   [HAVE_LIBMFDATA],
   [1],
   [Define to 1 if you have the `mfdata' library (-lmfdata).])
  LIBS="-lmfdata $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libmfdata" = xyes],
  [AC_SUBST(
   [HAVE_LIBMFDATA],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBMFDATA],
   [0])
  ])
 ])

dnl Function to detect how to enable libmfdata
AC_DEFUN([AX_LIBMFDATA_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libmfdata],
  [libmfdata],
  [search for libmfdata in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBMFDATA_CHECK_LIB

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

 AS_IF(
  [test "x$ac_cv_libmfdata" = xyes],
  [AC_SUBST(
   [ax_libmfdata_pc_libs_private],
   [-lmfdata])
  ])

 AS_IF(
  [test "x$ac_cv_libmfdata" = xyes],
  [AC_SUBST(
   [ax_libmfdata_spec_requires],
   [libmfdata])
  AC_SUBST(
   [ax_libmfdata_spec_build_requires],
   [libmfdata-devel])
  ])
 ])

