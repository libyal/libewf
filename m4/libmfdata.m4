dnl Functions for libmfdata
dnl
dnl Version: 20120519

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
  [test "x$ac_cv_with_libmfdata" = xno],
  [ac_cv_libmfdata=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [libmfdata],
    [libmfdata >= 20120425],
    [ac_cv_libmfdata=yes],
    [ac_cv_libmfdata=no])
   ])

  AS_IF(
   [test "x$ac_cv_libmfdata" = xyes],
   [ac_cv_libmfdata_CPPFLAGS="$pkg_cv_libmfdata_CFLAGS"
   ac_cv_libmfdata_LIBADD="$pkg_cv_libmfdata_LIBS"],
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
 
    dnl Cache functions
    AC_CHECK_LIB(
     mfdata,
     libmfdata_cache_initialize,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_cache_free,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_cache_clone,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
 
    dnl File functions
    AC_CHECK_LIB(
     mfdata,
     libmfdata_file_set_file_value,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
 
    dnl File list functions
    AC_CHECK_LIB(
     mfdata,
     libmfdata_file_list_initialize,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_file_list_free,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_file_list_resize,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_file_list_clone,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
 
    AC_CHECK_LIB(
     mfdata,
     libmfdata_file_list_get_number_of_files,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_file_list_get_file_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_file_list_get_file_value_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
 
    AC_CHECK_LIB(
     mfdata,
     libmfdata_file_list_set_file_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_file_list_set_file_value_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
 
    AC_CHECK_LIB(
     mfdata,
     libmfdata_file_list_append_file,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
 
    dnl File functions
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_initialize,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_free,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_resize,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_clone,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
 
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_is_group,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_is_set,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
 
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_get_number_of_elements,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_get_data_range_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_get_element_value_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_get_group_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
 
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_set_data_range_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_set_backup_data_range_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_set_element_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_set_element_value_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_set_group_by_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
 
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_append_group,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
 
    dnl List element functions
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_element_get_element_index,
     [ac_cv_libmfdata_dummy=yes],
     [ac_cv_libmfdata=no])
    AC_CHECK_LIB(
     mfdata,
     libmfdata_list_element_set_element_value,
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

    ac_cv_libmfdata_LIBADD="-lmfdata"
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libmfdata" = xyes],
  [AC_DEFINE(
   [HAVE_LIBMFDATA],
   [1],
   [Define to 1 if you have the `mfdata' library (-lmfdata).])
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

 dnl Check for a shared library version
 AX_LIBMFDATA_CHECK_LIB

 dnl Check if the dependencies for the local library version
 AS_IF(
  [test "x$ac_cv_libmfdata" != xyes],
  [AC_DEFINE(
   [HAVE_LOCAL_LIBMFDATA],
   [1],
   [Define to 1 if the local version of libmfdata is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBMFDATA],
   [1])

  ac_cv_libmfdata_CPPFLAGS="-I../libmfdata";
  ac_cv_libmfdata_LIBADD="../libmfdata/libmfdata.la";

  ac_cv_libmfdata=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBMFDATA],
  [test "x$ac_cv_libmfdata" = xlocal])
 AS_IF(
  [test "x$ac_cv_libmfdata_CPPFLAGS" != "x"],
  [AC_SUBST(
   [LIBMFDATA_CPPFLAGS],
   [$ac_cv_libmfdata_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_libmfdata_LIBADD" != "x"],
  [AC_SUBST(
   [LIBMFDATA_LIBADD],
   [$ac_cv_libmfdata_LIBADD])
  ])

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

