dnl Checks for libodraw required headers and functions
dnl
dnl Version: 20240520

dnl Function to detect if libodraw is available
dnl ac_libodraw_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBODRAW_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libodraw" = xno],
    [ac_cv_libodraw=no],
    [ac_cv_libodraw=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libodraw which returns "yes" and --with-libodraw= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libodraw" != x && test "x$ac_cv_with_libodraw" != xauto-detect && test "x$ac_cv_with_libodraw" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libodraw])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libodraw],
          [libodraw >= 20120630],
          [ac_cv_libodraw=yes],
          [ac_cv_libodraw=check])
        ])
      AS_IF(
        [test "x$ac_cv_libodraw" = xyes],
        [ac_cv_libodraw_CPPFLAGS="$pkg_cv_libodraw_CFLAGS"
        ac_cv_libodraw_LIBADD="$pkg_cv_libodraw_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libodraw" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libodraw.h])

      AS_IF(
        [test "x$ac_cv_header_libodraw_h" = xno],
        [ac_cv_libodraw=no],
        [ac_cv_libodraw=yes

        AX_CHECK_LIB_FUNCTIONS(
          [libodraw],
          [odraw],
          [[libodraw_get_version],
           [libodraw_handle_initialize],
           [libodraw_handle_free],
           [libodraw_handle_signal_abort],
           [libodraw_handle_open],
           [libodraw_handle_open_data_files],
           [libodraw_handle_close],
           [libodraw_handle_read_buffer],
           [libodraw_handle_seek_offset],
           [libodraw_handle_get_bytes_per_sector],
           [libodraw_handle_get_data_file],
           [libodraw_handle_get_media_size],
           [libodraw_handle_get_number_of_data_files],
           [libodraw_handle_get_number_of_sessions],
           [libodraw_handle_get_session],
           [libodraw_handle_get_number_of_tracks],
           [libodraw_handle_get_track],
           [libodraw_data_file_free],
           [libodraw_data_file_set_filename]])

        AS_IF(
          [test "x$ac_cv_enable_wide_character_type" != xno],
          [AX_CHECK_LIB_FUNCTIONS(
            [libodraw],
            [odraw],
            [[libodraw_handle_open_wide],
             [libodraw_data_file_set_filename_wide]])
          ])

        ac_cv_libodraw_LIBADD="-lodraw"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libodraw])
    ])

  AS_IF(
    [test "x$ac_cv_libodraw" = xyes],
    [AC_DEFINE(
      [HAVE_LIBODRAW],
      [1],
      [Define to 1 if you have the `odraw' library (-lodraw).])
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
AC_DEFUN([AX_LIBODRAW_CHECK_LOCAL],
  [AC_PROG_LEX(noyywrap)
  AC_PROG_YACC

  ac_cv_libodraw_CPPFLAGS="-I../libodraw -I\$(top_srcdir)/libodraw";
  ac_cv_libodraw_LIBADD="../libodraw/libodraw.la";

  ac_cv_libodraw=local
  ])

dnl Function to detect how to enable libodraw
AC_DEFUN([AX_LIBODRAW_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libodraw],
    [libodraw],
    [search for libodraw in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBODRAW_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libodraw" != xyes],
    [AX_LIBODRAW_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBODRAW],
      [1],
      [Define to 1 if the local version of libodraw is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBODRAW],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBODRAW],
    [test "x$ac_cv_libodraw" = xlocal])
  AS_IF(
    [test "x$ac_cv_libodraw_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBODRAW_CPPFLAGS],
      [$ac_cv_libodraw_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libodraw_LIBADD" != "x"],
    [AC_SUBST(
      [LIBODRAW_LIBADD],
      [$ac_cv_libodraw_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libodraw" = xyes],
    [AC_SUBST(
      [ax_libodraw_pc_libs_private],
      [-lodraw])
    ])

  AS_IF(
    [test "x$ac_cv_libodraw" = xyes],
    [AC_SUBST(
      [ax_libodraw_spec_requires],
      [libodraw])
    AC_SUBST(
      [ax_libodraw_spec_build_requires],
      [libodraw-devel])
    ])
  ])

