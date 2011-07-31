dnl Functions for libmfdata

dnl Function to detect if libmfdata available
AC_DEFUN([AC_CHECK_LIBMFDATA],
 [AC_CHECK_HEADERS([libmfdata.h])

 AS_IF(
  [test "x$ac_cv_header_libmfdata_h" = xno],
  [ac_cv_libmfdata=no],
  [ac_cv_libmfdata=yes
  AC_CHECK_LIB(
   mfdata,
   libmfdata_get_version,
   [],
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

